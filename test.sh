#!/bin/bash

LOG_DIR="log"
mkdir -p $LOG_DIR

# 日付と時間をフォーマットしてログファイル名を作成
timestamp=$(date +"%Y%m%d_%H%M")
log_file="$LOG_DIR/test_$timestamp.log"

cleanup() {
	rm -f $LOG_DIR/cmp $LOG_DIR/out
}

assert() {
	printf '%-30s:' "\"$1\"" | tee -a $log_file
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

cleanup
echo 'all OK' | tee -a $log_file

