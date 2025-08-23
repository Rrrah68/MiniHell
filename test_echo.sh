#!/usr/bin/env bash

MINISHELL=./minishell  # chemin vers votre binaire
pass=0
fail=0

run_test() {
    local cmd="$1"
    local expected="$2"
    # 1) On force l'envoi d'un \n avec printf
    # 2) On supprime la première ligne (le prompt) avec sed '1d'
    local output
    output=$(printf '%s\n' "$cmd" \
             | $MINISHELL \
             | sed '1d')

    if [ "$output" = "$expected" ]; then
        echo "OK   │ $cmd → «$output»"
        ((pass++))
    else
        echo "FAIL │ $cmd → «$output» (expected «$expected»)"
        ((fail++))
    fi
}

echo "=== Tests echo builtin ==="

# Cas de base
run_test "echo" ""
run_test "echo hello" "hello"
run_test "echo hello world" "hello world"

# Option -n
run_test "echo -n hello" "hello"
run_test "echo -n hello world" "hello world"
run_test "echo -n -n hello" "-n hello"
run_test "echo -nnn foo" "-nnn foo"

# Chaînes vides et quotes
run_test 'echo ""' ""
run_test 'echo "" test' " test"
run_test 'echo "  spaced   out  "' "  spaced   out  "

# Pas d’expansion de variables ici
run_test 'export FOO=bar && echo $FOO' ""
run_test 'echo "$HOME"' "\$HOME"

echo
echo "Résultat : $pass passed, $fail failed."
[ $fail -eq 0 ] || exit 1
