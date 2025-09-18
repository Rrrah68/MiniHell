#!/bin/bash

# Tests avancés pour minishell
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${YELLOW}=== TESTS AVANCÉS MINISHELL ===${NC}"

TOTAL_TESTS=0
PASSED_TESTS=0

run_test() {
    local test_name="$1"
    local command="$2"
    
    echo -e "\n${YELLOW}Test: $test_name${NC}"
    echo "Command: $command"
    
    echo "$command" | timeout 5s bash 2>/dev/null
    bash_exit=$?
    
    echo "$command" | timeout 5s ./minishell 2>/dev/null
    mini_exit=$?
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if [ $bash_exit -eq $mini_exit ]; then
        echo -e "${GREEN}✓ PASS${NC} (bash: $bash_exit, minishell: $mini_exit)"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAIL${NC} (bash: $bash_exit, minishell: $mini_exit)"
    fi
}

# Tests de pipes
echo -e "\n${YELLOW}=== TESTS PIPES ===${NC}"
run_test "Echo pipe grep" "echo hello world | grep hello"
run_test "Ls pipe wc" "ls | wc -l"

# Tests de redirections
echo -e "\n${YELLOW}=== TESTS REDIRECTIONS ===${NC}"
run_test "Echo redirect output" "echo test > /tmp/test_output && cat /tmp/test_output && rm /tmp/test_output"
run_test "Cat redirect input" "echo input_test > /tmp/input && cat < /tmp/input && rm /tmp/input"

# Tests d'expansion de variables
echo -e "\n${YELLOW}=== TESTS VARIABLES ===${NC}"
run_test "Echo variable HOME" "echo \$HOME | head -c 10"
run_test "Echo variable inexistante" "echo \$INEXISTANT_VAR"

# Tests de quotes
echo -e "\n${YELLOW}=== TESTS QUOTES ===${NC}"
run_test "Simple quotes" "echo 'hello world'"
run_test "Double quotes" "echo \"hello world\""
run_test "Mixed quotes" "echo 'hello \"world\"'"

echo -e "\n${YELLOW}=== RÉSUMÉ TESTS AVANCÉS ===${NC}"
echo -e "Tests passés: ${GREEN}$PASSED_TESTS${NC}/$TOTAL_TESTS"