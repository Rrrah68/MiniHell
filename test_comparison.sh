#!/bin/bash

# Script de comparaison BASH vs MINISHELL

echo "========================================"
echo "🔍 COMPARAISON BASH vs MINISHELL"
echo "========================================"

echo -e "\n🔹 Test 1: Commandes simples"
echo "BASH:"
echo "Hello World" | cat
echo "MINISHELL: (à tester manuellement)"

echo -e "\n🔹 Test 2: Echo avec options"
echo "BASH:"
echo -n "Sans newline"
echo " - Avec newline"
echo "MINISHELL: (à tester manuellement)"

echo -e "\n🔹 Test 3: Variables d'environnement"
echo "BASH:"
echo "HOME = $HOME"
echo "PATH = $PATH"
echo "MINISHELL: (à tester manuellement)"

echo -e "\n🔹 Test 4: Redirections"
echo "BASH:"
echo "Test redirection" > test_bash.txt
cat test_bash.txt
rm test_bash.txt
echo "MINISHELL: (à tester manuellement)"

echo -e "\n🔹 Test 5: Pipes"
echo "BASH:"
echo -e "ligne1\nligne2\nligne3" | grep "ligne2"
echo "MINISHELL: (à tester manuellement)"

echo -e "\n🔹 Test 6: Gestion d'erreurs"
echo "BASH:"
ls fichier_inexistant 2>/dev/null || echo "Erreur gérée"
echo "MINISHELL: (à tester manuellement)"

echo -e "\n🔹 Test 7: Heredoc"
echo "BASH:"
cat << EOF
Ceci est un test
de heredoc
EOF
echo "MINISHELL: (à tester manuellement)"

echo -e "\n🔹 Test 8: Builtins"
echo "BASH:"
pwd
cd /tmp && pwd && cd - >/dev/null
echo "MINISHELL: (à tester manuellement)"

echo -e "\n🔹 Test 9: Signaux (Ctrl+C)"
echo "BASH: Tapez 'sleep 5' puis Ctrl+C"
echo "MINISHELL: (à tester manuellement)"

echo -e "\n🔹 Test 10: Syntaxe invalide"
echo "BASH:"
bash -c "echo |" 2>&1 || echo "Erreur syntaxe détectée"
echo "MINISHELL: (à tester manuellement)"

echo -e "\n========================================"
echo "✅ Tests BASH terminés"
echo "🔄 Maintenant testez dans MINISHELL :"
echo "./minishell"
echo "========================================"