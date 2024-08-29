#!/bin/bash

LOG_DIR="log"
mkdir -p $LOG_DIR

# 日付と時間をフォーマットしてログファイル名を作成
timestamp=$(date +"%Y%m%d_%H%M")
log_file="$LOG_DIR/test_$timestamp.log"

cat <<EOF | gcc -xc -o a.out -
#include <stdio.h>
int main() { printf("hello from a.out\n"); }
EOF

cat <<EOF | gcc -xc -o print_args -
#include <stdio.h>
int main(int argc, char *argv[]) {
	for (int i = 0; argv[i]; i++)
		printf("argv[%d] = %s\n", i, argv[i]);
}
EOF

cleanup() {
	rm -f $LOG_DIR/cmp $LOG_DIR/out $LOG_DIR/a.out $LOG_DIR/print_args
}

assert() {
	printf '%-50s:' "[$1]" | tee -a $log_file
	# exit status
	echo -n -e "$1" | bash >$LOG_DIR/cmp 2>&- 
	expected=$?
	echo -n -e "$1" | ./minishell >$LOG_DIR/out 2>&-
	actual=$?

	diff $LOG_DIR/cmp $LOG_DIR/out >/dev/null && echo -n '  diff OK' | tee -a $log_file || echo -n '  diff NG' | tee -a $log_file

	if [ "$actual" = "$expected" ]; then
		echo -n '  status OK' | tee -a $log_file
	else
		echo -n "  status NG, expected $expected but got $actual" | tee -a $log_file
	fi
	echo | tee -a $log_file
}

# Empty line (EOF)
assert ''

# Absolute path commands without args 
assert '/bin/pwd'
assert '/bin/echo'
assert '/bin/ls'

# Search command path without args
assert 'pwd'
assert 'echo'
assert 'ls'
assert './a.out'

## no such command
assert 'a.out'
assert 'nosuchfile'

# Tokenize
## unquoted word
assert 'ls /'
assert 'echo hello    world     '
assert 'nosuchfile\n\n'

cleanup
echo 'all OK' | tee -a $log_file

## single quote
assert "./print_args 'hello   world' '42Tokyo'"
assert "echo 'hello   world' '42Tokyo'"
assert "echo '\"hello   world\"' '42Tokyo'"

## double quote
assert './print_args "hello   world" "42Tokyo"'
assert 'echo "hello   world" "42Tokyo"'
assert "echo \"'hello   world'\" \"42Tokyo\""

## combination
assert "echo hello'      world'"
assert "echo hello'  world  '\"  42Tokyo  \""