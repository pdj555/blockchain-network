# Agent Configuration for Autonomous Code Improvement

This directory contains the configuration and infrastructure for running autonomous code improvement sessions using Codex or similar agentic coding tools.

## Overview

The agent system is designed to run for extended periods (default: 24 hours) and systematically improve the codebase following the principles outlined in the configuration files.

## Directory Structure

```
.agent/
├── config/              # Configuration files
│   ├── agent.yaml       # Main agent configuration
│   └── validation_rules.yaml  # Validation requirements
├── instructions/        # Agent instructions
│   ├── system_prompt.md # Primary agent instructions
│   └── improvement_roadmap.md  # Prioritized improvement tasks
├── progress/            # Progress tracking
│   ├── current_session.json  # Current session state
│   └── history.jsonl    # Historical log of improvements
├── scripts/             # Utility scripts
│   ├── validate.sh      # Pre-commit validation script
│   └── run_agent.sh     # Main execution script
├── logs/                # Execution logs (gitignored)
└── README.md            # This file
```

## Starting a 24-Hour Run

To start an autonomous improvement session:

```bash
cd /path/to/blockchain-network
./.agent/run_agent.sh
```

The script will:

1. Initialize or load an existing session
2. Run for 24 hours (configurable in `config/agent.yaml`)
3. Create checkpoints every hour
4. Validate all changes before committing
5. Track progress in `progress/current_session.json`

## Stopping a Run

To stop a running session:

- Press `Ctrl+C` - the script will create a checkpoint before exiting
- The session can be resumed later (it will load from `current_session.json`)

## Reviewing Progress

### Current Session

View the current session status:

```bash
cat .agent/progress/current_session.json
```

Or with pretty printing (if `jq` is installed):

```bash
jq . .agent/progress/current_session.json
```

### History

View improvement history:

```bash
cat .agent/progress/history.jsonl
```

### Logs

View execution logs:

```bash
tail -f .agent/logs/agent_$(date +%Y%m%d).log
```

## Configuration

### Main Configuration (`config/agent.yaml`)

Key settings:

- `runtime.duration_hours`: How long to run (default: 24)
- `runtime.checkpoint_interval_minutes`: How often to checkpoint (default: 60)
- `improvement_focus.priority_order`: Order of improvement areas
- `validation.required_before_commit`: What must pass before committing

### Validation Rules (`config/validation_rules.yaml`)

Defines what checks must pass:

- Build success
- All tests pass
- No compiler warnings
- No regressions

## Manual Validation

To manually validate the current codebase state:

```bash
./.agent/scripts/validate.sh
```

This runs all validation checks without making any changes.

## Improvement Areas

The agent focuses on these areas (in priority order):

1. **Memory Safety**: Replace manual memory management with smart pointers
2. **Code Quality**: Fix typos, improve naming, remove dead code
3. **Error Handling**: Add validation, improve error messages
4. **Documentation**: Add essential comments, improve README
5. **Performance**: Optimize data structures, reduce copies
6. **Test Coverage**: Add edge case tests, improve existing tests

See `instructions/improvement_roadmap.md` for detailed tasks.

## Principles

The agent follows these core principles:

- **Zero Fluff**: Every line must have high value
- **Complete Fixes**: Never temporary band-aids
- **Simplicity**: Self-explaining code
- **Validation**: All tests must pass after changes
- **Atomic Commits**: One focused improvement per commit

## Integration with Codex/Agent Tools

To integrate with an actual agentic coding tool:

1. Load `instructions/system_prompt.md` as the system prompt
2. Use `instructions/improvement_roadmap.md` for task selection
3. Run `scripts/validate.sh` after each change
4. Update `progress/current_session.json` after successful improvements
5. Append to `progress/history.jsonl` for each completed improvement

## Troubleshooting

### Validation Fails

- Check logs in `.agent/logs/`
- Review recent changes with `git log`
- Run validation manually: `./.agent/scripts/validate.sh`

### Session Not Resuming

- Check `progress/current_session.json` exists
- Verify session status is not "completed"
- Check logs for errors

### Build Issues

- Ensure CMake is installed
- Clean build directory: `rm -rf build && cmake -S . -B build`
- Check compiler version (requires C++11 support)

## Notes

- Logs are automatically rotated every 6 hours
- Checkpoints create git commits (if in a git repository)
- The agent respects the iteration limit per hour to avoid overwhelming the system
- All changes are validated before being committed
