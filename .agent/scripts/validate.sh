#!/bin/bash
# Validation script for agent changes
# Ensures code compiles, tests pass, and no regressions occur

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
BUILD_DIR="$REPO_ROOT/build"
LOG_DIR="$REPO_ROOT/.agent/logs"
VALIDATION_LOG="$LOG_DIR/validation_$(date +%Y%m%d_%H%M%S).log"

mkdir -p "$LOG_DIR"

log() {
    echo "[$(date +'%Y-%m-%d %H:%M:%S')] $1" | tee -a "$VALIDATION_LOG"
}

log "Starting validation..."

# Change to repo root
cd "$REPO_ROOT"

# Check 1: Build Success
log "Checking build..."
if ! cmake -S . -B "$BUILD_DIR" >> "$VALIDATION_LOG" 2>&1; then
    log "ERROR: CMake configuration failed"
    exit 1
fi

if ! cmake --build "$BUILD_DIR" >> "$VALIDATION_LOG" 2>&1; then
    log "ERROR: Build failed"
    exit 1
fi
log "Build successful"

# Check 2: No Compiler Warnings
log "Checking for compiler warnings..."
if cmake --build "$BUILD_DIR" 2>&1 | grep -i "warning" >> "$VALIDATION_LOG" 2>&1; then
    log "ERROR: Compiler warnings detected"
    exit 1
fi
log "No compiler warnings"

# Check 3: Main Program Exists
log "Checking main program..."
if [ ! -f "$BUILD_DIR/p4" ]; then
    log "ERROR: Main program (p4) not found"
    exit 1
fi
log "Main program exists"

# Check 4: All Tests Pass
log "Running tests..."
if [ ! -f "$BUILD_DIR/tests" ]; then
    log "WARNING: Test executable not found, skipping tests"
else
    if ! "$BUILD_DIR/tests" >> "$VALIDATION_LOG" 2>&1; then
        log "ERROR: Tests failed"
        exit 1
    fi
    log "All tests passed"
fi

# Check 5: No Regressions (if input/output files exist)
if [ -f "$REPO_ROOT/input1.txt" ] && [ -f "$REPO_ROOT/output1.txt" ]; then
    log "Checking for regressions..."
    TMP_OUTPUT=$(mktemp)
    if ! "$BUILD_DIR/p4" < "$REPO_ROOT/input1.txt" > "$TMP_OUTPUT" 2>&1; then
        log "ERROR: Program execution failed"
        rm -f "$TMP_OUTPUT"
        exit 1
    fi
    
    if ! diff -q "$REPO_ROOT/output1.txt" "$TMP_OUTPUT" >> "$VALIDATION_LOG" 2>&1; then
        log "ERROR: Output regression detected"
        log "Expected output differs from actual output"
        rm -f "$TMP_OUTPUT"
        exit 1
    fi
    rm -f "$TMP_OUTPUT"
    log "No regressions detected"
fi

log "Validation complete - all checks passed"
exit 0

