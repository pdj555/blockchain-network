#!/usr/bin/env python3
import argparse
import collections
import json
import os
import sys
import urllib.error
import urllib.request
from dataclasses import dataclass
from typing import Dict, Iterable, List, Tuple


@dataclass(frozen=True)
class Transaction:
    node: int
    transaction_id: int
    from_id: int
    to_id: int
    amount: int
    timestamp: str


def _read_tokens(stream: Iterable[str]) -> List[str]:
    return "".join(stream).split()


def parse_network(tokens: List[str]) -> Tuple[int, int, int, int, List[Tuple[int, int]], List[Transaction]]:
    it = iter(tokens)

    def next_token(name: str) -> str:
        try:
            return next(it)
        except StopIteration as exc:
            raise ValueError(f"Missing {name}") from exc

    def next_int(name: str) -> int:
        val = next_token(name)
        try:
            return int(val)
        except ValueError as exc:
            raise ValueError(f"Invalid {name}: {val!r}") from exc

    num_nodes = next_int("num_nodes")
    tx_per_block = next_int("transactions_per_block")
    total_transactions_declared = next_int("total_transactions")
    num_edges = next_int("num_edges")

    edges: List[Tuple[int, int]] = []
    for i in range(num_edges):
        u = next_int(f"edge[{i}].u")
        v = next_int(f"edge[{i}].v")
        edges.append((u, v))

    transactions: List[Transaction] = []
    while True:
        try:
            node_token = next(it)
        except StopIteration:
            break

        try:
            node = int(node_token)
        except ValueError as exc:
            raise ValueError(f"Invalid transaction.node: {node_token!r}") from exc

        try:
            transaction_id = next_int("transaction.id")
            from_id = next_int("transaction.from_id")
            to_id = next_int("transaction.to_id")
            amount = next_int("transaction.amount")
            timestamp = next_token("transaction.timestamp")
        except StopIteration:
            break

        transactions.append(
            Transaction(
                node=node,
                transaction_id=transaction_id,
                from_id=from_id,
                to_id=to_id,
                amount=amount,
                timestamp=timestamp,
            )
        )

    return num_nodes, tx_per_block, total_transactions_declared, num_edges, edges, transactions


def summarize(
    num_nodes: int,
    tx_per_block: int,
    total_transactions_declared: int,
    num_edges: int,
    edges: List[Tuple[int, int]],
    transactions: List[Transaction],
) -> Dict:
    tx_count_by_node = [0] * max(num_nodes, 0)
    volume_by_node = [0] * max(num_nodes, 0)

    balances: Dict[int, int] = collections.defaultdict(lambda: 100)
    negative_events: List[Dict] = []
    invalid_edges: List[Dict] = []
    invalid_transactions: List[Dict] = []
    net_flow: Dict[int, int] = collections.defaultdict(int)

    for (u, v) in edges:
        if not (0 <= u < num_nodes) or not (0 <= v < num_nodes):
            invalid_edges.append({"u": u, "v": v, "reason": "node_out_of_range"})

    for t in transactions:
        if not (0 <= t.node < num_nodes):
            invalid_transactions.append(
                {"transaction_id": t.transaction_id, "node": t.node, "reason": "node_out_of_range"}
            )
            continue
        if t.amount <= 0:
            invalid_transactions.append(
                {"transaction_id": t.transaction_id, "node": t.node, "reason": "non_positive_amount"}
            )
            continue

        tx_count_by_node[t.node] += 1
        volume_by_node[t.node] += t.amount

        from_before = balances[t.from_id]
        to_before = balances[t.to_id]
        from_after = from_before - t.amount
        to_after = to_before + t.amount

        balances[t.from_id] = from_after
        balances[t.to_id] = to_after
        net_flow[t.from_id] -= t.amount
        net_flow[t.to_id] += t.amount

        if from_after < 0:
            negative_events.append(
                {
                    "transaction_id": t.transaction_id,
                    "node": t.node,
                    "id": t.from_id,
                    "balance_before": from_before,
                    "amount": t.amount,
                    "balance_after": from_after,
                    "timestamp": t.timestamp,
                }
            )

    def blocks_for_tx_count(count: int) -> int:
        if tx_per_block <= 0:
            return 0
        return max(1, (count + tx_per_block - 1) // tx_per_block)

    node_summaries = []
    for n in range(num_nodes):
        node_summaries.append(
            {
                "node": n,
                "transactions": tx_count_by_node[n],
                "blocks": blocks_for_tx_count(tx_count_by_node[n]),
                "volume": volume_by_node[n],
            }
        )

    top_net_receivers = sorted(net_flow.items(), key=lambda kv: kv[1], reverse=True)[:5]
    top_net_senders = sorted(net_flow.items(), key=lambda kv: kv[1])[:5]

    return {
        "nodes": num_nodes,
        "transactions_per_block": tx_per_block,
        "total_transactions_declared": total_transactions_declared,
        "total_transactions_parsed": len(transactions),
        "edges_declared": num_edges,
        "edges_parsed": len(edges),
        "invalid_edges": invalid_edges,
        "invalid_transactions": invalid_transactions,
        "unique_ids": len(balances),
        "negative_balance_events": negative_events,
        "node_summaries": node_summaries,
        "top_net_receivers": [{"id": i, "net_in": v} for i, v in top_net_receivers],
        "top_net_senders": [{"id": i, "net_out": -v} for i, v in top_net_senders],
    }


def _extract_response_text(response: Dict) -> str:
    if isinstance(response.get("output_text"), str) and response["output_text"].strip():
        return response["output_text"].strip()

    chunks: List[str] = []
    for item in response.get("output", []):
        for content in item.get("content", []):
            text = content.get("text")
            if isinstance(text, str) and text.strip():
                chunks.append(text.strip())
    return "\n\n".join(chunks).strip()


def generate_ai_report(summary: Dict, model: str, api_key: str, base_url: str) -> str:
    prompt = (
        "You are an expert blockchain ledger auditor. "
        "Write a concise, high-signal audit summary from the JSON below.\n\n"
        "Include:\n"
        "1) Executive summary (2-4 bullets)\n"
        "2) Key risks/anomalies (focus on negative balances and outliers)\n"
        "3) Recommended next steps to make this production-grade\n\n"
        f"JSON:\n{json.dumps(summary, indent=2, sort_keys=True)}\n"
    )

    payload = {"model": model, "input": prompt}
    req = urllib.request.Request(
        url=base_url.rstrip("/") + "/v1/responses",
        data=json.dumps(payload).encode("utf-8"),
        headers={
            "Authorization": f"Bearer {api_key}",
            "Content-Type": "application/json",
        },
        method="POST",
    )

    try:
        with urllib.request.urlopen(req, timeout=60) as resp:
            body = resp.read().decode("utf-8")
    except urllib.error.HTTPError as exc:
        detail = exc.read().decode("utf-8", errors="replace") if exc.fp else str(exc)
        raise RuntimeError(f"OpenAI API HTTP {exc.code}: {detail}") from exc
    except urllib.error.URLError as exc:
        raise RuntimeError(f"OpenAI API request failed: {exc}") from exc

    data = json.loads(body)
    text = _extract_response_text(data)
    if not text:
        raise RuntimeError("OpenAI API returned no text output")
    return text


def main() -> int:
    parser = argparse.ArgumentParser(description="Audit blockchain-network inputs and optionally generate an AI report.")
    parser.add_argument("input", nargs="?", default="-", help="Input file (default: stdin)")
    parser.add_argument("--format", choices=["text", "json"], default="text", help="Output format (default: text)")
    parser.add_argument("--openai", action="store_true", help="Generate an AI report using the OpenAI API")
    parser.add_argument("--model", default="gpt-4.1-mini", help="OpenAI model for --openai (default: gpt-4.1-mini)")
    parser.add_argument(
        "--base-url",
        default=os.environ.get("OPENAI_BASE_URL", "https://api.openai.com"),
        help="OpenAI API base URL (default: https://api.openai.com)",
    )
    args = parser.parse_args()

    if args.input == "-" or args.input == "":
        tokens = _read_tokens(sys.stdin)
    else:
        with open(args.input, "r", encoding="utf-8") as f:
            tokens = f.read().split()

    num_nodes, tx_per_block, total_declared, num_edges, edges, txs = parse_network(tokens)
    summary = summarize(num_nodes, tx_per_block, total_declared, num_edges, edges, txs)

    if args.format == "json":
        output: Dict = dict(summary)
        if args.openai:
            api_key = os.environ.get("OPENAI_API_KEY")
            if not api_key:
                raise RuntimeError("OPENAI_API_KEY is required for --openai")
            output["ai_report"] = generate_ai_report(output, args.model, api_key, args.base_url)
        print(json.dumps(output, indent=2, sort_keys=True))
        return 0

    print(f"Nodes: {summary['nodes']}, tx/block: {summary['transactions_per_block']}, edges: {summary['edges_parsed']}")
    print(
        f"Transactions: parsed={summary['total_transactions_parsed']} (declared={summary['total_transactions_declared']})"
    )
    print(f"Unique IDs seen: {summary['unique_ids']}")

    if summary["invalid_edges"]:
        print(f"Invalid edges: {len(summary['invalid_edges'])} (first 3 shown)")
        for e in summary["invalid_edges"][:3]:
            print(f"- u={e['u']} v={e['v']} reason={e['reason']}")
    else:
        print("Invalid edges: 0")

    if summary["invalid_transactions"]:
        print(f"Invalid transactions: {len(summary['invalid_transactions'])} (first 3 shown)")
        for e in summary["invalid_transactions"][:3]:
            print(f"- tx={e['transaction_id']} node={e['node']} reason={e['reason']}")
    else:
        print("Invalid transactions: 0")

    neg = summary["negative_balance_events"]
    if neg:
        print(f"Negative balance events: {len(neg)} (first 3 shown)")
        for e in neg[:3]:
            print(
                f"- id={e['id']} tx={e['transaction_id']} node={e['node']} "
                f"before={e['balance_before']} amount={e['amount']} after={e['balance_after']} ts={e['timestamp']}"
            )
    else:
        print("Negative balance events: 0")

    print("Top net receivers:")
    for r in summary["top_net_receivers"]:
        print(f"- id={r['id']} net_in={r['net_in']}")

    print("Top net senders:")
    for s in summary["top_net_senders"]:
        print(f"- id={s['id']} net_out={s['net_out']}")

    if args.openai:
        api_key = os.environ.get("OPENAI_API_KEY")
        if not api_key:
            raise RuntimeError("OPENAI_API_KEY is required for --openai")
        print("\nAI report:\n")
        print(generate_ai_report(summary, args.model, api_key, args.base_url))

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
