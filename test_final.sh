#!/bin/bash

echo "=== Test final du heredoc avec Ctrl+C ==="
echo "Instructions:"
echo "1. Tapez: cat << EOF"
echo "2. Tapez quelques lignes de texte"
echo "3. Appuyez sur Ctrl+C"
echo "4. Vérifiez que le prompt réapparaît immédiatement"
echo ""

exec timeout 30 ./minishell