
#!/usr/bin/env bash
set -e

echo "Running cppcheck..."
cppcheck \
    --enable=all \
    --inconclusive \
    --std=c++20 \
    --force \
    --suppress=missingIncludeSystem \
    -I include \
    -i test \
    src/

echo "Static analysis complete."
