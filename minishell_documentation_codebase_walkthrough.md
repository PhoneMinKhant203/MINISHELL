# Minishell Documentation (Step-by-step code walkthrough)

This document explains how your minishell works end-to-end, and what each file/function is responsible for.

## 1) High-level architecture

Your shell processes user input in this pipeline:

1. **Read line** (interactive prompt)
2. **Handle multiline quotes** (keep reading until quotes are closed)
3. **Split by `;`** (quote-aware)
4. For each segment:
   - **Lex**: convert text into tokens
   - **Validate syntax**: reject invalid token sequences early
   - **Parse**: build an AST for `&&` / `||`, and command lists for pipelines
   - **Expand**: variables + quotes + word splitting + wildcards + redir targets
   - **Execute**: builtins, binaries, pipelines, subshells, redirections, heredocs

A compact call chain looks like this:

- `main()`
  - `shell_loop()`
    - `readline()`
    - `handle_input()`
      - `check_quotes()`
      - `process_input()`  (split by `;`)
        - `process_an_input()`
          - `tokenize()`
          - `validate_syntax()`
          - `parse_ast()`
          - `expand_ast()`
          - `execute_ast()`

## 2) Core data structures (minishell.h)

These are the “building blocks” that all modules share:

### Tokens
- `t_token { value, type, next }`
- `t_tktype` includes:
  - Words: `T_WORD`
  - Operators: `T_PIPE`, `T_AND`, `T_OR`
  - Redirs: `T_IN`, `T_OUT`, `T_APPEND`, `T_HEREDOC`
  - Parentheses: `T_LPAREN`, `T_RPAREN`
  - Invalid operators: `T_BAD`

### Commands + redirections
- `t_cmd` represents **one command in a pipeline**
  - `type`: `CMD_SIMPLE` or `CMD_SUBSHELL`
  - `args`: argv array (for simple commands)
  - `subshell`: AST for `( ... )`
  - `redirs`: linked list of `t_redir`
  - `next`: next command (pipeline)

- `t_redir { type, target, next }`
  - `target` is the filename/delimiter token string after expansion

### AST nodes for `&&` / `||`
- `t_node { type, left, right, pipeline }`
  - `type`:
    - `N_PIPELINE`: leaf node, runs a pipeline (`t_cmd*` list)
    - `N_AND`: `left && right`
    - `N_OR`: `left || right`

### Shell state
- `t_shell { env, last_status, should_exit, exit_code, line_no }`
  - `env`: linked list of `t_env`
  - `last_status`: last command exit code
  - `should_exit` / `exit_code`: used by `exit` builtin
  - `line_no`: used for heredoc warnings

## 3) Step-by-step: what happens when you type a command

Let’s follow a typical line:

Example:
- `echo "$USER" *.c > out.txt && (cat out.txt | wc -l)`

### Step A — Read input (main.c)
- `readline("minishell$ ")` reads a line.
- If `readline()` returns `NULL` (Ctrl-D):
  - prints `exit` in interactive mode
  - sets `should_exit`
  - returns last status.

### Step B — Handle Ctrl-C prompt behavior (signals.c + input_handler.c)
- The SIGINT handler sets `g_signal = SIGINT`, prints newline, resets readline buffer.
- At the start of `handle_input()`, if SIGINT happened, minishell sets:
  - `last_status = 130`, then clears `g_signal`.

### Step C — Multiline quotes (quote_checker.c)
- `check_quotes()` scans for unclosed `'` or `"`.
- If unclosed: prompts `> ` and appends the new line with `\n`.
- Ctrl-C during this state cancels the pending multiline input.
- Ctrl-D during this state prints the bash-like “unexpected EOF” errors.

### Step D — Split by `;` (input_handler.c)
- `process_input()` splits the line into segments at `;` **only when not inside quotes**.
- Each segment is trimmed.
- Empty segments trigger: `minishell: syntax error near ';'` and status `2`.

### Step E — Tokenize (lexer.c + lexer_utils.c)
- `tokenize()` walks the segment and builds a `t_token` linked list.
- `get_token()` recognizes:
  - `|` or `||`
  - `&&`
  - `<`, `<<`, `>`, `>>`
  - `(`, `)`
  - words (`T_WORD`) using `get_word()` which preserves quoted chunks.
- Invalid operators like `&` or `|&` become `T_BAD`.

### Step F — Validate syntax early (syntax.c)
- `validate_syntax()` rejects bad sequences such as:
  - starting with `|`, `&&`, `||`, `)`
  - `|` / `&&` / `||` at end of line
  - redirection with no filename or with non-word filename
  - wrong parenthesis placement / depth mismatch
  - word immediately after `)` (e.g. `)(` or `)word`)

### Step G — Parse into AST + pipelines (parser.c + parser_utils*.c)

**Parser output is an AST of logical operators**, where leaves are pipelines:

- `parse_ast()`
  - parses a first pipeline (`parse_pipeline()`)
  - then while next token is `&&` or `||`, links more pipelines into a tree

- `parse_pipeline()`
  - repeatedly calls `parse_one_cmd()`
  - connects them with `cmd->next` when `|` is present

- `parse_one_cmd()`
  - If token is `(`: uses `parse_subshell()`
    - consumes `(`
    - calls `parse_ast()` recursively until it reaches `)`
    - attaches redirections after `)` to the subshell command
  - Else: parses a simple command:
    - counts args (`count_args()`)
    - allocates `cmd->args`
    - copies words (`fill_args()`)
    - collects redirections (`handle_redir()`)

Key detail: redirections are stored on the command node, not executed yet.

### Step H — Expansion (expand.c + expand_utils.c + expand_redir.c)

Expansion is done **after parsing** so it can be applied to:
- argv (`cmd->args`)
- redirection targets (`cmd->redirs->target`)
- nested subshell ASTs

#### 1) `expand_ast()`
- Walks the AST. For each pipeline leaf:
  - calls `expand_cmds()`

#### 2) `expand_cmds()`
For each `t_cmd` in the pipeline:
- If it is a subshell: recursively expands inside `cmd->subshell`
- Expands argv via `process_cmd_argv()`
- Expands each redirection target via `expand_redir()`

#### 3) argv expansion rules (expand.c)
Each original argument is processed by `arg_expansion()`:
- `had_quotes = has_quotes(arg)`
- `expanded = expand_str(ft_strdup(arg), sh)`
- If it had quotes:
  - **no word splitting** (expanded result stays one argv element)
- If it had no quotes:
  - it is split by whitespace into multiple argv elements (`split_expand_wd()`)

#### 4) `expand_str()` (expand.c)
Builds a new string by scanning characters:
- `'` or `"` → `handle_quote()`
- `$` → `handle_dollar()`
- normal chars → append

#### 5) Quote handling (expand_utils.c)
- `handle_quote()` removes the quote delimiters.
- Inside double quotes:
  - `$VAR` expansions are allowed
- Inside single quotes:
  - `$` is not expanded
- Special behavior: wildcard characters inside quotes are **masked**
  - `*` becomes byte `1`, `?` becomes `2`, `[` becomes `3`
  - this prevents wildcard expansion later

#### 6) Dollar expansion (expand_utils.c)
- `$?` → last exit code
- `$$` → string `"4242"` (placeholder; not actual PID)
- `$0` → `"minishell"`
- `$<name>` where `<name>` is `[A-Za-z0-9_]` → env lookup
- `$` followed by space/quote/end → treated as literal `$`

#### 7) Redirection target expansion (expand_redir.c)
- Heredoc delimiter is **not expanded here** (`T_HEREDOC` returns early)
- For other redirs:
  - expands variables and removes quotes
  - runs wildcard expansion for redir targets (single match only)
  - detects “ambiguous redirect” when:
    - wildcard results in multiple matches
    - expansion produces spaces without quotes
    - expansion produces empty string
  - sets `r->target = NULL` to force redirection failure later

### Step I — Wildcards (wildcards*.c)

Wildcards are applied after argv expansion:
- `apply_wildcards(cmd)` calls `expand_wildcards_argv()`

Rules:
- Only patterns containing `*`, `?`, `[` are candidates.
- Matches are collected from current directory (`opendir(".")`).
- Hidden files (`.something`) only match if the pattern starts with `.`.
- Results are sorted lexicographically.
- If no matches: the original pattern is kept.
- After wildcard handling: masked wildcard bytes are restored back to `* ? [`.

Supported pattern syntax:
- `*` any string
- `?` any single char
- `[abc]` set
- `[a-z]` range
- `[!a-z]` / `[^a-z]` negated range

### Step J — Execution (mini_executor.c + cmd_execution*.c)

Execution chooses the best strategy depending on the pipeline structure.

#### 1) `execute_ast()` (mini_executor.c)
- `N_PIPELINE`: run the pipeline
- `N_AND`: run left; if status==0 run right
- `N_OR`: run left; if status!=0 run right

#### 2) Pipeline leaf execution (mini_executor.c)
`execute_cmds(cmds, sh)` decides:
- **Subshell only (no pipe)**: `handle_subshell()`
- **Null command** (no argv but redirs exist): `handle_null_cmd()`
  - still applies redirections (useful for `> file`)
- **Single builtin (no pipe)**: run builtin in parent via `handle_builtin()`
  - preserves shell state changes (cd/export/unset/exit)
- Otherwise: `start_executor()` runs pipeline with forks

#### 3) Pipeline forking (cmd_execution.c)
`start_executor(cmds, sh)`:
- ignores SIGINT/SIGQUIT in parent during fork/wait
- iterates commands:
  - creates a pipe if there is a next command
  - `fork()`
    - child: `child_process()`
    - parent: `update_parent_fds()`
- waits for all children; sets `last_status` to the last pipeline process exit

#### 4) Child execution (cmd_execution.c)
`child_process(cmd, sh, fdin, pipefd)`:
- set default SIGINT/SIGQUIT
- connect stdin/stdout to pipeline ends
- apply redirections via `setup_redirection()`
  - heredoc runs here by replacing stdin with a pipe
- if subshell command: `exit(execute_ast(cmd->subshell, sh))`
- if builtin in pipeline: `exit(exe_builtin(...))`
- else execute external program:
  - `child_exec_binary()`

#### 5) Resolving program path (cmd_execution_utils.c + path.c)
- If argv[0] contains `/`:
  - use it directly (`ft_strdup(argv[0])`)
- Else look for `PATH` in env:
  - if PATH missing/empty: print “No such file or directory” and exit 127
  - otherwise search `PATH` directories:
    - build `<dir>/<cmd>` and `stat()` + `access(X_OK)`

Extra safety:
- `is_directory(path)` checks for directories.
  - directories exit with 126 and message “Is a directory”

#### 6) Exit codes and error mapping
- If `execve()` fails:
  - prints `strerror(errno)`
  - exits `126` for `EACCES` or `EISDIR`
  - exits `127` for other errors
- If “command not found”: exits 127
- Signals:
  - last status becomes `128 + signal`
  - SIGINT commonly yields 130

### Step K — Redirections and heredocs (redirection.c + heredoc*.c)

#### 1) Redirection application (redirection.c)
`setup_redirection(cmd, sh)` iterates `cmd->redirs` in order:
- `< file` → `open(O_RDONLY)` then `dup2(fd, STDIN)`
- `> file` → `open(O_CREAT|O_TRUNC)` then `dup2(fd, STDOUT)`
- `>> file` → `open(O_CREAT|O_APPEND)` then `dup2(fd, STDOUT)`
- `<< delim` → `handle_heredoc()`

If any redir target is `NULL` (ambiguous) or open/dup2 fails → return `-1`.

#### 2) Heredoc behavior (heredoc.c + heredoc_utils.c)
`handle_heredoc(delimiter_raw, sh)`:
- creates a pipe
- `strip_delim_quotes()` removes quotes from delimiter and decides whether `$` expands
  - if delimiter had quotes → no expansion
- `heredoc_loop()` reads lines until delimiter or EOF
  - uses `readline("> ")`
  - expands `$VAR` when allowed (`expand_heredoc_line()`)
  - writes the content into pipe
- duplicates the pipe read end into `STDIN`

If EOF occurs before delimiter:
- `heredoc_warn()` prints a warning with the line number.

## 4) File-by-file responsibilities and function notes

Below is a concise map of each file.

### main.c
- `main()`
  - initializes `t_shell` state and environment
  - enters `shell_loop()`
- `shell_loop()`
  - reads input until EOF or `exit`
- `init_shell()`
  - `init_env()`, `update_shlvl()`, `setup_signals()`

### input_handler.c
- `handle_input()`
  - handles SIGINT bookkeeping, history, quote completion
  - calls `process_input()`
- `process_input()`
  - splits on `;` outside quotes and processes each segment

### quote_checker.c
- `check_quotes()`
  - if quotes aren’t closed, reads more lines with prompt `> `
  - Ctrl-C cancels pending multiline input
  - Ctrl-D prints EOF quote error messages

### lexer.c / lexer_utils.c
- `tokenize()`
  - produces linked-list tokens from a string
- `get_token()`
  - recognizes operators and words
- `get_word()`
  - reads until whitespace/operator, skipping quoted chunks

### syntax.c
- `validate_syntax()`
  - checks operator correctness, redirection correctness, parentheses depth
  - sets `sh->last_status = 2` on syntax errors

### parser.c / parser_utils.c / parser_utils_2.c
- `parse_ast()`
  - builds an AST for `&&` and `||`
- `parse_pipeline()`
  - builds `t_cmd` list for pipes
- `parse_one_cmd()`
  - parses a simple command or a subshell command
- `parse_subshell()`
  - consumes `( ... )`, parses inner AST recursively, attaches redirs

### mini_executor.c
- `expand_ast()`
  - expands pipelines in the AST
- `execute_ast()`
  - runs pipeline leaves and applies `&&` / `||` logic
- `execute_cmds()`
  - chooses builtin-in-parent vs forked pipeline vs subshell handling

### cmd_execution.c / cmd_execution_utils.c
- `start_executor()`
  - runs a pipeline with fork/pipe chaining
- `child_process()`
  - sets up pipeline fds, redirs, then runs builtin/subshell/binary
- `wait_all_children()`
  - waits for all children, sets shell last status based on last pipeline PID
- `resolve_path()`
  - command contains `/` → direct
  - otherwise use PATH search

### path.c
- `find_path()`
  - searches PATH dirs for an executable regular file

### redirection.c / redir_utils.c
- `setup_redirection()`
  - applies all redirections in order
- `new_redir()` and `redir_add_back()`
  - build redirection list on a command

### heredoc.c / heredoc_utils.c
- `handle_heredoc()`
  - reads heredoc content into a pipe, replaces stdin
- `strip_delim_quotes()`
  - determines whether heredoc `$` expansion is allowed
- `expand_heredoc_line()`
  - expands `$VAR` inside heredoc lines (when allowed)

### expand.c / expand_utils.c / expand_redir.c
- `expand_cmds()`
  - expands argv, redirections, and nested subshell ASTs
- `expand_str()`
  - removes quotes and expands `$`
- `handle_quote()` / `handle_dollar()`
  - implement quoting and variable rules
- `expand_redir()`
  - expands redirection targets and detects ambiguous redirect

### wildcards.c / wildcards_dir.c / wildcards_math.c / wildcards_utils.c
- `apply_wildcards()`
  - expands wildcard args and un-masks quoted wildcard bytes
- `list_matches()`
  - lists matches in current directory and sorts them
- `match_pattern()`
  - implements glob matching recursion

### subshell_utils.c
- `handle_subshell()`
  - executes a subshell command in a dedicated child when it’s alone
  - prints signal messages like bash

### env_init.c / env_utils.c / shell_lv.c
- `init_env()`
  - parses `envp` into a linked list
- `env_to_array()`
  - converts list back to `char**` for exec
- `update_shlvl()`
  - updates `SHLVL` with bash-like rules

### builtin.c + mini_*.c
- `is_builtin()` / `exe_builtin()`
  - builtin detection and dispatch
- `mini_cd()`
  - supports `cd`, `cd ~`, `cd -`, updates `PWD`/`OLDPWD`
- `mini_echo()`
  - supports repeated `-n` flags
- `mini_pwd()`
  - prints `getcwd()`
- `mini_env()`
  - prints environment variables with values
- `mini_export()`
  - validates identifiers, adds/updates variables, prints sorted list
- `mini_unset()`
  - removes keys from env list
- `mini_exit()`
  - sets `should_exit`, validates numeric args, handles “too many arguments”

### error.c
- `print_err()` standardized error messages
- `exe_error()` prints special errors for `.`, `..`, empty command

### free.c
- central free helpers for argv/tokens/cmds/env

## 5) A short “mental model” cheat sheet

When debugging, follow this order:

1. **Input**: `handle_input()` → was SIGINT/EOF handled correctly?
2. **Quotes**: `check_quotes()` → did it keep reading until closed?
3. **Segmentation**: `process_input()` → did `;` splitting create the right segments?
4. **Tokens**: `tokenize()` → do you get expected token types?
5. **Syntax**: `validate_syntax()` → if error, it should stop here (status 2)
6. **Parse**: `parse_ast()` → pipeline list + nested subshells look right?
7. **Expand**: `expand_cmds()` → variables, quotes, word splitting, wildcards
8. **Execute**:
   - builtin alone → parent executes (state changes persist)
   - pipeline / external → forked execution
   - `( ... )` → AST recursion inside child context
9. **Status**:
   - logical operators depend on exit status (0 success)
   - pipeline status is last process status

If you want, I can also generate a “call graph” diagram (mermaid) of the runtime flow and another diagram of the data flow (tokens → AST → pipeline → exec).

