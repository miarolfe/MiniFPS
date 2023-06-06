#!/bin/bash

# Get the absolute path of the directory containing the script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Change the current directory to the script directory
cd "$SCRIPT_DIR"
./MiniFPS