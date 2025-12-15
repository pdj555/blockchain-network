#!/bin/bash
# Main execution script for 24-hour agentic code improvement session
# Manages checkpoints, validation, and progress tracking

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
AGENT_DIR="$REPO_ROOT/.agent"
CONFIG_DIR="$AGENT_DIR/config"
PROGRESS_DIR="$AGENT_DIR/progress"
LOG_DIR="$AGENT_DIR/logs"
SCRIPTS_DIR="$AGENT_DIR/scripts"

SESSION_FILE="$PROGRESS_DIR/current_session.json"
HISTORY_FILE="$PROGRESS_DIR/history.jsonl"
VALIDATE_SCRIPT="$SCRIPTS_DIR/validate.sh"

DURATION_HOURS=24
CHECKPOINT_INTERVAL_MINUTES=60
MAX_ITERATIONS_PER_HOUR=10

mkdir -p "$LOG_DIR"

log() {
    echo "[$(date +'%Y-%m-%d %H:%M:%S')] $1" | tee -a "$LOG_DIR/agent_$(date +%Y%m%d).log"
}

error_exit() {
    log "ERROR: $1"
    exit 1
}

# Initialize session
init_session() {
    SESSION_ID=$(date +%Y%m%d_%H%M%S)
    START_TIME=$(date -u +"%Y-%m-%dT%H:%M:%SZ")
    
    cat > "$SESSION_FILE" <<EOF
{
  "session_id": "$SESSION_ID",
  "start_time": "$START_TIME",
  "end_time": "",
  "duration_hours": $DURATION_HOURS,
  "status": "running",
  "improvements_completed": 0,
  "commits_made": 0,
  "current_priority": "memory_safety",
  "current_task": "",
  "areas_covered": [],
  "last_checkpoint": "",
  "test_status": "unknown",
  "notes": []
}
EOF
    
    log "Session initialized: $SESSION_ID"
    log "Duration: $DURATION_HOURS hours"
    log "Checkpoint interval: $CHECKPOINT_INTERVAL_MINUTES minutes"
}

# Load session if exists
load_session() {
    if [ -f "$SESSION_FILE" ]; then
        log "Loading existing session..."
        # Session will be updated by agent
    else
        init_session
    fi
}

# Create checkpoint
create_checkpoint() {
    CHECKPOINT_TIME=$(date -u +"%Y-%m-%dT%H:%M:%SZ")
    log "Creating checkpoint at $CHECKPOINT_TIME"
    
    # Update session file with checkpoint time
    if command -v jq &> /dev/null; then
        jq ".last_checkpoint = \"$CHECKPOINT_TIME\"" "$SESSION_FILE" > "$SESSION_FILE.tmp" && mv "$SESSION_FILE.tmp" "$SESSION_FILE"
    fi
    
    # Save git state
    cd "$REPO_ROOT"
    if [ -d ".git" ]; then
        git add -A
        git commit -m "checkpoint: $(date +%Y%m%d_%H%M%S)" || true
    fi
}

# Validate before proceeding
validate_changes() {
    log "Running validation..."
    if "$VALIDATE_SCRIPT"; then
        log "Validation passed"
        return 0
    else
        log "Validation failed"
        return 1
    fi
}

# Calculate elapsed time
get_elapsed_hours() {
    if [ -f "$SESSION_FILE" ] && command -v jq &> /dev/null; then
        START=$(jq -r '.start_time' "$SESSION_FILE")
        if [ "$START" != "null" ] && [ -n "$START" ]; then
            START_EPOCH=$(date -j -f "%Y-%m-%dT%H:%M:%SZ" "$START" +%s 2>/dev/null || date -d "$START" +%s 2>/dev/null)
            NOW_EPOCH=$(date +%s)
            ELAPSED=$(( (NOW_EPOCH - START_EPOCH) / 3600 ))
            echo $ELAPSED
            return
        fi
    fi
    echo 0
}

# Main execution loop
main() {
    log "Starting agent execution..."
    
    load_session
    
    ELAPSED=0
    LAST_CHECKPOINT=$(date +%s)
    CHECKPOINT_INTERVAL_SEC=$((CHECKPOINT_INTERVAL_MINUTES * 60))
    ITERATION_COUNT=0
    HOUR_START=$(date +%s)
    
    while [ $ELAPSED -lt $DURATION_HOURS ]; do
        CURRENT_TIME=$(date +%s)
        
        # Check if checkpoint needed
        if [ $((CURRENT_TIME - LAST_CHECKPOINT)) -ge $CHECKPOINT_INTERVAL_SEC ]; then
            create_checkpoint
            LAST_CHECKPOINT=$CURRENT_TIME
        fi
        
        # Check iteration limit per hour
        HOUR_ELAPSED=$((CURRENT_TIME - HOUR_START))
        if [ $HOUR_ELAPSED -ge 3600 ]; then
            ITERATION_COUNT=0
            HOUR_START=$CURRENT_TIME
        fi
        
        if [ $ITERATION_COUNT -ge $MAX_ITERATIONS_PER_HOUR ]; then
            log "Reached max iterations for this hour, waiting..."
            sleep 300  # Wait 5 minutes
            continue
        fi
        
        # Agent should work here
        # This is a framework - actual agent logic would be integrated
        log "Agent iteration $ITERATION_COUNT"
        log "Elapsed time: $ELAPSED hours / $DURATION_HOURS hours"
        
        # Validate current state
        if ! validate_changes; then
            log "Validation failed, pausing..."
            sleep 60
            continue
        fi
        
        ITERATION_COUNT=$((ITERATION_COUNT + 1))
        ELAPSED=$(get_elapsed_hours)
        
        # Small delay between iterations
        sleep 10
    done
    
    # Final checkpoint
    create_checkpoint
    
    # Update session as completed
    END_TIME=$(date -u +"%Y-%m-%dT%H:%M:%SZ")
    if command -v jq &> /dev/null; then
        jq ".status = \"completed\" | .end_time = \"$END_TIME\"" "$SESSION_FILE" > "$SESSION_FILE.tmp" && mv "$SESSION_FILE.tmp" "$SESSION_FILE"
    fi
    
    log "Agent session completed"
}

# Handle signals
trap 'log "Received interrupt signal, creating checkpoint and exiting..."; create_checkpoint; exit 0' INT TERM

# Run main
main

