#!/bin/bash

# Script de test Valgrind pour MiniShell
# Usage: ./test_valgrind.sh [test_name]

# Couleurs pour la sortie
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
SUPPRESSION_FILE="valgrind_minishell.supp"
VALGRIND_OPTS="--leak-check=full --show-leak-kinds=all --track-origins=yes"

echo -e "${BLUE}=== Test Valgrind pour MiniShell ===${NC}"
echo ""

# Test 1: Simple commande
echo -e "${YELLOW}Test 1: Commande simple${NC}"
echo "echo hello" | valgrind $VALGRIND_OPTS --log-file=out_test1 ./minishell
echo -e "${GREEN}✓ Test 1 terminé - Voir out_test1${NC}"
echo ""

# Test 2: Commande simple avec suppressions
echo -e "${YELLOW}Test 2: Commande simple (avec suppressions)${NC}"
echo "echo hello" | valgrind --suppressions=$SUPPRESSION_FILE $VALGRIND_OPTS --log-file=out_test2_clean ./minishell
echo -e "${GREEN}✓ Test 2 terminé - Voir out_test2_clean${NC}"
echo ""

# Test 3: Commandes avec erreurs
echo -e "${YELLOW}Test 3: Commandes avec erreurs${NC}"
echo -e "cat | inexistant\nls | erreur\nexit" | valgrind $VALGRIND_OPTS --log-file=out_test3 ./minishell
echo -e "${GREEN}✓ Test 3 terminé - Voir out_test3${NC}"
echo ""

# Test 4: Commandes avec erreurs (avec suppressions)
echo -e "${YELLOW}Test 4: Commandes avec erreurs (avec suppressions)${NC}"
echo -e "cat | inexistant\nls | erreur\nexit" | valgrind --suppressions=$SUPPRESSION_FILE $VALGRIND_OPTS --log-file=out_test4_clean ./minishell
echo -e "${GREEN}✓ Test 4 terminé - Voir out_test4_clean${NC}"
echo ""

# Résumé des résultats
echo -e "${BLUE}=== Résumé des tests ===${NC}"
echo ""

for file in out_test1 out_test2_clean out_test3 out_test4_clean; do
    if [ -f "$file" ]; then
        echo -e "${YELLOW}Fichier: $file${NC}"
        
        # Chercher ERROR SUMMARY
        error_summary=$(grep "ERROR SUMMARY" "$file" | tail -1)
        if [[ $error_summary == *"0 errors"* ]]; then
            echo -e "  ${GREEN}$error_summary${NC}"
        else
            echo -e "  ${RED}$error_summary${NC}"
        fi
        
        # Chercher LEAK SUMMARY
        echo "  $(grep -A4 "LEAK SUMMARY" "$file" | tail -4)"
        echo ""
    fi
done

echo -e "${GREEN}Tous les tests terminés !${NC}"
echo ""
echo -e "${BLUE}Guide d'interprétation :${NC}"
echo "- ERROR SUMMARY: 0 errors = ${GREEN}Aucune erreur mémoire${NC}"
echo "- definitely lost: 0 bytes = ${GREEN}Aucune fuite définitive${NC}" 
echo "- still reachable: 0 bytes = ${GREEN}Fuites bibliothèques masquées${NC}"
echo "- suppressed: ~232,000 bytes = ${YELLOW}Normal (bibliothèques externes)${NC}"