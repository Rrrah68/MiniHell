#!/bin/bash

echo "=== TEST RAPIDE MINISHELL ==="

# Test echo
echo "Testing echo..."
echo "Hello World"

# Test echo -n
echo -n "No newline: "
echo "With newline"

# Test variables
export TEST_VAR="hello"
echo "Variable: $TEST_VAR"

# Test redirection
echo "Test file" > test.txt
cat test.txt
rm test.txt

# Test pipe
echo -e "line1\nline2\nline3" | grep "line2"

echo "=== TESTS TERMINÉS ==="