#!/bin/bash

echo "Testing minishell redirect functionality..."

# Test 1: Basic input redirection
echo "Test 1: Basic input redirection (<)"
echo "hello world" > test_input.txt
echo "< test_input.txt cat" | ./minishell
rm -f test_input.txt

# Test 2: Basic output redirection (>)
echo "Test 2: Basic output redirection (>)"
echo "echo hello world > test_output.txt" | ./minishell
cat test_output.txt
rm -f test_output.txt

# Test 3: Append redirection (>>)
echo "Test 3: Append redirection (>>)"
echo "echo first line > test_append.txt" | ./minishell
echo "echo second line >> test_append.txt" | ./minishell
cat test_append.txt
rm -f test_append.txt

# Test 4: Heredoc (<<)
echo "Test 4: Heredoc (<<)"
echo "cat << EOF
line 1
line 2
EOF" | ./minishell

# Test 5: Multiple redirects
echo "Test 5: Multiple redirects"
echo "hello world" > test_multiple.txt
echo "< test_multiple.txt cat > test_multiple_out.txt" | ./minishell
cat test_multiple_out.txt
rm -f test_multiple.txt test_multiple_out.txt

# Test 6: Command with only redirects
echo "Test 6: Command with only redirects"
echo "hello world" > test_only.txt
echo "< test_only.txt > test_only_out.txt" | ./minishell
cat test_only_out.txt
rm -f test_only.txt test_only_out_out.txt

# Test 7: Pipeline with redirects
echo "Test 7: Pipeline with redirects"
echo "hello world" > test_pipe.txt
echo "< test_pipe.txt cat | wc -l" | ./minishell
rm -f test_pipe.txt

echo "Redirect tests completed!" 