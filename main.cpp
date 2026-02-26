#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "blockNetwork.h"

namespace {

struct Options {
    bool quiet = false;
    bool verify = false;
    bool display = true;
    bool json = false;
    bool originOnly = false;
    bool strict = false;
    std::string inputPath;
};

void printUsage(const char *argv0) {
    std::cout << "Usage: " << argv0 << " [options] [input_file]\n\n"
              << "Reads the network description from stdin by default.\n\n"
              << "Options:\n"
              << "  -h, --help       Show this help\n"
              << "  --quiet          Suppress per-transaction insert logs\n"
              << "  --verify         Verify all chains and set exit code\n"
              << "  --no-display     Do not print the full ledger output\n"
              << "  --json           Print a machine-readable JSON summary\n"
              << "  --origin-only    Disable propagation; insert only into source node\n"
              << "  --strict         Exit non-zero if any transaction is rejected\n";
}

enum class ParseResult { ok, help, error };

ParseResult parseArgs(int argc, char **argv, Options &opts) {
    for (int i = 1; i < argc; ++i) {
        const std::string arg(argv[i]);
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return ParseResult::help;
        }
        if (arg == "--quiet") {
            opts.quiet = true;
            continue;
        }
        if (arg == "--verify") {
            opts.verify = true;
            continue;
        }
        if (arg == "--no-display") {
            opts.display = false;
            continue;
        }
        if (arg == "--json") {
            opts.json = true;
            opts.quiet = true;
            opts.verify = true;
            opts.display = false;
            continue;
        }
        if (arg == "--origin-only") {
            opts.originOnly = true;
            continue;
        }
        if (arg == "--strict") {
            opts.strict = true;
            continue;
        }
        if (!arg.empty() && arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << std::endl;
            return ParseResult::error;
        }
        if (!opts.inputPath.empty()) {
            std::cerr << "Multiple input files provided; expected at most one." << std::endl;
            return ParseResult::error;
        }
        opts.inputPath = arg;
    }
    return ParseResult::ok;
}

} // namespace

int main(int argc, char **argv)
{
    Options opts;
    const ParseResult parseResult = parseArgs(argc, argv, opts);
    if (parseResult == ParseResult::help) {
        return 0;
    }
    if (parseResult == ParseResult::error) {
        return 1;
    }

    std::ifstream inputFile;
    std::istream *in = &std::cin;
    if (!opts.inputPath.empty()) {
        inputFile.open(opts.inputPath);
        if (!inputFile.is_open()) {
            std::cerr << "Failed to open input file: " << opts.inputPath << std::endl;
            return 1;
        }
        in = &inputFile;
    }

    int numNodesInNetwork;
    int numEdges;
    if (!(*in >> numNodesInNetwork) || numNodesInNetwork <= 0) {
        std::cerr << "Invalid number of nodes" << std::endl;
        return 1;
    }
    if (!opts.json) {
        std::cout << "Number of nodes: " << numNodesInNetwork << std::endl;
    }

    int numTransactionsPerBlock;
    if (!(*in >> numTransactionsPerBlock) || numTransactionsPerBlock <= 0) {
        std::cerr << "Invalid number of transactions per block" << std::endl;
        return 1;
    }
    if (!opts.json) {
        std::cout << "Number of transactions per block: " << numTransactionsPerBlock << std::endl;
    }

    int totalNumTransactions;
    if (!(*in >> totalNumTransactions) || totalNumTransactions < 0) {
        std::cerr << "Invalid total number of transactions" << std::endl;
        return 1;
    }
    if (!opts.json) {
        std::cout << "Total number of transactions: " << totalNumTransactions << std::endl;
    }

    blockNetwork n1(numNodesInNetwork, numTransactionsPerBlock);
    if (opts.quiet) {
        n1.setLogStream(nullptr);
    }
    if (opts.originOnly) {
        n1.setPropagationEnabled(false);
    }

    int uNode;
    int vNode;
    if (!(*in >> numEdges) || numEdges < 0) {
        std::cerr << "Invalid number of edges" << std::endl;
        return 1;
    }
    for (int i = 0; i < numEdges; i++) {
        if (!(*in >> uNode >> vNode)) {
            std::cerr << "Unexpected end of edge list" << std::endl;
            return 1;
        }
        if (uNode < 0 || uNode >= numNodesInNetwork ||
            vNode < 0 || vNode >= numNodesInNetwork) {
            std::cerr << "Invalid edge " << uNode << " -> " << vNode << std::endl;
            return 1;
        }
        n1.addEdge(uNode, vNode);
    }

    int node;
    int transactionID;
    int fromID;
    int toID;
    int amountTrans;
    std::string timeStamp;
    int acceptedTransactions = 0;
    int rejectedTransactions = 0;

    while (true) {
        if (!(*in >> node >> transactionID >> fromID >> toID >> amountTrans >> timeStamp)) {
            if (in->eof()) {
                break;
            }
            std::cerr << "Malformed transaction entry" << std::endl;
            return 1;
        }
        if (node < 0 || node >= numNodesInNetwork) {
            std::cerr << "Invalid node in transaction" << std::endl;
            return 1;
        }
        if (amountTrans <= 0) {
            std::cerr << "Invalid transaction amount" << std::endl;
            return 1;
        }
        transaction t(node, transactionID, fromID, toID, amountTrans, timeStamp);
        if (n1.insertTranToNode(node, t)) {
            ++acceptedTransactions;
        } else {
            ++rejectedTransactions;
        }
    }

    if (acceptedTransactions + rejectedTransactions != totalNumTransactions) {
        std::cerr << "Declared transaction count does not match parsed entries" << std::endl;
        return 1;
    }

    if (opts.display) {
        n1.display();
    }

    bool verified = true;
    if (opts.verify) {
        verified = n1.verifyAllChains();
    }

    if (opts.json) {
        const int edgeCount = n1.getNumEdges();
        const int isolatedNodeCount = n1.getIsolatedNodeCount();
        const int maxReachableNodeCount = n1.getMaxReachableNodeCount();
        const double maxBroadcastCoverage = numNodesInNetwork > 0
            ? static_cast<double>(maxReachableNodeCount) / static_cast<double>(numNodesInNetwork)
            : 0.0;

        std::cout << "{\n";
        std::cout << "  \"nodes\": " << numNodesInNetwork << ",\n";
        std::cout << "  \"edges\": " << edgeCount << ",\n";
        std::cout << "  \"isolated_nodes\": " << isolatedNodeCount << ",\n";
        std::cout << "  \"max_reachable_nodes\": " << maxReachableNodeCount << ",\n";
        std::cout << "  \"max_broadcast_coverage\": " << maxBroadcastCoverage << ",\n";
        std::cout << "  \"transactions_per_block\": " << numTransactionsPerBlock << ",\n";
        std::cout << "  \"total_transactions_declared\": " << totalNumTransactions << ",\n";
        std::cout << "  \"accepted_transactions\": " << acceptedTransactions << ",\n";
        std::cout << "  \"rejected_transactions\": " << rejectedTransactions << ",\n";
        std::cout << "  \"verified\": " << (verified ? "true" : "false") << ",\n";
        std::cout << "  \"propagation_enabled\": " << (n1.isPropagationEnabled() ? "true" : "false") << ",\n";
        std::cout << "  \"node_summaries\": [\n";
        for (int nodeIndex = 0; nodeIndex < numNodesInNetwork; ++nodeIndex) {
            std::cout << "    {\"node\": " << nodeIndex
                      << ", \"blocks\": " << n1.getNodeBlockCount(nodeIndex)
                      << ", \"transactions\": " << n1.getNodeTransactionCount(nodeIndex)
                      << ", \"rejected_transactions\": " << n1.getNodeRejectedTransactionCount(nodeIndex)
                      << "}";
            if (nodeIndex + 1 < numNodesInNetwork) {
                std::cout << ",";
            }
            std::cout << "\n";
        }
        std::cout << "  ]\n";
        std::cout << "}\n";
    }

    if (opts.verify && !verified) {
        std::cerr << "Chain verification failed" << std::endl;
        return 2;
    }

    if (opts.strict && rejectedTransactions > 0) {
        std::cerr << "Rejected transactions encountered in strict mode" << std::endl;
        return 3;
    }

    return 0;
}
