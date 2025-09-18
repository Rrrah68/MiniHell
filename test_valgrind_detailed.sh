#!/bin/bash

# Script de test Valgrind pour minishell avec fichier de suppression
# Résultat stocké dans out1

echo "=== Test Valgrind pour minishell ==="
echo "Date: $(date)"
echo "Utilisation du fichier de suppression: valgrind_minishell.supp"
echo "========================================="

# Vérifier que le binaire minishell existe
if [ ! -f "./minishell" ]; then
    echo "Erreur: Le binaire minishell n'existe pas. Compilation en cours..."
    make
    if [ $? -ne 0 ]; then
        echo "Erreur: Échec de la compilation"
        exit 1
    fi
fi

# Vérifier que le fichier de suppression existe
if [ ! -f "./valgrind_minishell.supp" ]; then
    echo "Erreur: Le fichier de suppression valgrind_minishell.supp n'existe pas"
    exit 1
fi

# Commandes de test pour minishell
TEST_COMMANDS="echo hello
ls -la
pwd
export TEST_VAR=42
echo \$TEST_VAR
cd /tmp
pwd
cd -
env | grep TEST
unset TEST_VAR
exit"

echo "=== Lancement de Valgrind avec suppression des fuites de readline ==="
echo "Commandes testées:"
echo "$TEST_COMMANDS"
echo ""

# Lancer Valgrind avec le fichier de suppression
echo "$TEST_COMMANDS" | valgrind \
    --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --verbose \
    --suppressions=./valgrind_minishell.supp \
    --log-file=out1 \
    ./minishell 2>&1

echo ""
echo "=== Test terminé ==="
echo "Les résultats de Valgrind ont été sauvegardés dans: out1"
echo "Pour consulter les résultats:"
echo "  cat out1"
echo ""
echo "Résumé rapide des erreurs:"
grep -E "(ERROR SUMMARY|definitely lost|indirectly lost|possibly lost)" out1 2>/dev/null || echo "Fichier out1 non trouvé ou pas encore généré"