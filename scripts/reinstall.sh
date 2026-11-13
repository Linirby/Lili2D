#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

cd "$REPO_ROOT"

bash "$SCRIPT_DIR/uninstall.sh"
rm -rf "$REPO_ROOT/build/"
bash "$SCRIPT_DIR/install.sh"
