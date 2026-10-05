*This project has been created as part of the 42 curriculum by egokce, gdemirci.*

# minishell

A small Unix shell written in C — a minimal re-implementation of the core of `bash`.

## Description

minishell shows an interactive prompt, reads a command line, and runs it the way bash would: it handles quotes, expands variables, connects commands with pipes, applies redirections and launches programs. It is built from scratch in C, using only the functions allowed by the 42 subject, with GNU `readline` for line editing and history.

Every command line goes through a fixed pipeline:

```
readline  →  syntax check  →  lexer  →  parser  →  expander  →  executor
```

1. **Syntax check** — rejects unclosed quotes and misplaced pipes or redirections with an error message instead of crashing.
2. **Lexer / parser** — splits the line into tokens and builds the list of commands with their arguments and redirections.
3. **Expander** — resolves `$VAR`, `$?`, `~` and quotes, then splits words.
4. **Executor** — runs a single command or a whole pipeline, finds programs through `PATH`, and sets the exit status.

**Features**

- Interactive prompt with command history
- Single and double quotes
- Environment variable expansion (`$VAR`), exit status (`$?`) and tilde (`~`)
- Pipes: any number of commands connected with `|`
- Redirections: `<`, `>`, `>>` and here-documents `<<`
- Builtins: `echo` (with `-n`), `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- External programs found through `PATH`, or by relative / absolute path
- Bash-like signal handling in interactive mode: `Ctrl-C`, `Ctrl-D`, `Ctrl-\`
- Memory tracker to keep allocations under control and free everything on exit

## Instructions

**Requirements:** a C compiler (`cc`), `make` and the GNU `readline` library.

```
sudo apt-get install libreadline-dev    # Debian / Ubuntu
```

**Build**

```
make          # build the minishell executable
make clean    # remove object files
make fclean   # remove object files and the executable
make re       # rebuild from scratch
```

**Run**

```
./minishell
```

Example session:

```
minishell$ echo "Hello $USER" | cat -e
Hello gamze$
minishell$ ls -l | grep minishell > out.txt
minishell$ cat << EOF
> heredoc line
> EOF
heredoc line
minishell$ export NAME=42 && echo $NAME
minishell$ exit
```

## Project Structure

```
minishell/
├── main.c             # prompt loop
├── signals.c          # signal handling
├── lexer/             # tokenizer
├── validate_syntax/   # quotes, pipes and redirection checks
├── parser/            # builds the command list
├── expander/          # variables, tilde, quotes, word splitting
├── executor/          # single commands, pipelines, PATH lookup
├── redirections/      # <, >, >> and heredoc
├── builtins/          # echo, cd, pwd, export, unset, env, exit
├── env/               # environment variable list
└── utils/             # helpers
```

## Team

Developed as a pair project by **egokce** and **gdemirci**.

- **gdemirci** — executor and the overall shell flow

## Resources

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [GNU Readline Library documentation](https://tiswww.case.edu/php/chet/readline/rltop.html)
- The 42 `minishell` subject
- Manual pages: `fork`, `execve`, `pipe`, `dup2`, `waitpid`, `sigaction`, `access`

**Use of AI**

AI tools were used as a support resource during this project for code review, debugging assistance and improving the clarity of documentation.