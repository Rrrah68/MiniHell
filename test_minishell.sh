#!/bin/bash

# Script de test pour comparer minishell avec bash
# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}=== TESTS MINISHELL vs BASH ===${NC}"

# Compteurs
TOTAL_TESTS=0
PASSED_TESTS=0

# Function to run test
run_test() {
    local test_name="$1"
    local command="$2"
    
    echo -e "\n${YELLOW}Test: $test_name${NC}"
    echo "Command: $command"
    
    # Run with bash
    echo "$command" | timeout 5s bash 2>/dev/null
    bash_exit=$?
    
    # Run with minishell
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

# Tests basiques
echo -e "\n${YELLOW}=== TESTS BASIQUES ===${NC}"
run_test "Exit sans argument" "exit"
run_test "Exit avec 0" "exit 0"
run_test "Exit avec 42" "exit 42"
run_test "Exit avec 256" "exit 256"
run_test "Exit avec -1" "exit -1"

# Tests builtins
echo -e "\n${YELLOW}=== TESTS BUILTINS ===${NC}"
run_test "Echo simple" "echo hello"
run_test "Echo avec -n" "echo -n hello"
run_test "Pwd" "pwd"
run_test "Env" "env | head -3"

# Tests d'erreurs
echo -e "\n${YELLOW}=== TESTS D'ERREURS ===${NC}"
run_test "Exit argument non numérique" "exit abc"
run_test "Exit trop d'arguments" "exit 1 2"
run_test "Commande inexistante" "commandeinexistante"

# Résumé
echo -e "\n${YELLOW}=== RÉSUMÉ ===${NC}"
echo -e "Tests passés: ${GREEN}$PASSED_TESTS${NC}/$TOTAL_TESTS"

if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
    echo -e "${GREEN}Tous les tests sont passés ! 🎉${NC}"
else
    echo -e "${RED}Certains tests ont échoué${NC}"
fi