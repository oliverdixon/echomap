#!/usr/bin/env bash

command -v miniserve >/dev/null 2>&1 || {
  echo "error: miniserve is required." >&2
  exit 1
}

[[ $# -ge 1 ]] && directory="$1" || directory="$PWD"

miniserve "$directory" \
  --interfaces 127.0.0.1 \
  --port 8080 \
  --header "Cross-Origin-Opener-Policy:same-origin" \
  --header "Cross-Origin-Embedder-Policy:require-corp" \
  --header "Cross-Origin-Resource-Policy:same-origin" \
  --header "Cache-Control:no-cache, no-store, must-revalidate"

exit $?
