#!/bin/bash

echo "=== Test du heredoc avec Ctrl+C ==="
echo "Lancement du minishell..."
echo "Tapez: cat << EOF"
echo "Puis tapez quelques lignes et essayez Ctrl+C"
echo "Le shell devrait s'interrompre et revenir au prompt"
echo ""

./minishell