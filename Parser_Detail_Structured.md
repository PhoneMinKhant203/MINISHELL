# Minishell Parser & AST (Codebase-Aligned Guide)

This guide explains **exactly what your parser builds** and **why the AST exists**, based on your current code in:

- `parser.c`
- `parser_utils.c`
- `parser_utils_2.c`

It’s written to help you *track the token cursor (`t_token **`)* and stop getting lost while reading the parser.

---

## Table of Contents

1. Purpose: what the parser does (and doesn’t do)
2. Inputs & outputs (tokens → AST + pipelines)
3. The “grammar” and operator precedence your parser implements
4. File responsibilities (why 3 files)
5. Stop rules (the #1 reason the code feels confusing)
6. Step-by-step walkthrough of each parsing function
7. AST explained visually (how executor will walk it)
8. Exact structures for common examples
9. Memory ownership & error cleanup (who frees what)
10. Debugging tips (what to print to visualize your parse)

---

## 1) Purpose: what the parser does

After lexing, you have a **flat** linked list of tokens like:

```
WORD(echo) WORD(hi) PIPE(|) WORD(cat) AND(&&) LPAREN(() WORD(ls) RPAREN())
```

The parser converts that flat list into a **structured execution plan**:

### The parser builds
- **Pipelines (`t_cmd*`)**: a linked list of commands for `|`
  - `cmd1 -> cmd2 -> cmd3`
- **AST (`t_node*`)** for `&&` and `||`
  - leaves are pipelines (`N_PIPELINE`)
  - internal nodes are logical operators (`N_AND`, `N_OR`)
- **Subshell commands**: `( ... )` becomes a `t_cmd` where:
  - `cmd->type = CMD_SUBSHELL`
  - `cmd->subshell` points to a nested AST (parsed recursively)

### The parser does NOT do
- Syntax correctness rules (mostly done earlier in `syntax.c`)
- Expansion (`$VAR`, quotes removal, word splitting)
- Wildcards expansion
- Applying redirections / executing heredocs
- Fork/exec

Parser only:
- shapes the structure
- stores raw strings (argv, redir targets, heredoc delimiter)

---

## 2) Input → output (what you should “see”)

### Input
- `t_token*` linked list produced by lexer

### Output
- `t_node*` root of AST

Each `N_PIPELINE` node owns a pipeline list:

```
t_node (N_PIPELINE)
  ->pipeline (t_cmd list)
      cmd#1 -> cmd#2 -> ...
```

---

## 3) The grammar and precedence your parser implements

Your parser effectively implements:

- **AST** (logical operators):
  - `AST := PIPELINE ( (&& | ||) PIPELINE )*`
- **PIPELINE**:
  - `PIPELINE := ONE_CMD ( '|' ONE_CMD )*`
- **ONE_CMD**:
  - `ONE_CMD := SUBSHELL | SIMPLE_CMD`
- **SUBSHELL**:
  - `SUBSHELL := '(' AST ')' (REDIR)*`
- **SIMPLE_CMD**:
  - tokens until a stop token, split into:
    - argv words (`T_WORD`)
    - redirections (`<`, `>`, `>>`, `<<`) + their targets

### Precedence (highest → lowest)
1. Parentheses `( ... )` (subshell payload is parsed as its own AST)
2. Pipes `|`
3. `&&` and `||` (same precedence; handled **left-to-right**)

---

## 4) Why you have 3 parser files

This split is healthy and common in 42 minishells:

### `parser.c` — structure builder
- Builds the *shape*:
  - pipeline lists
  - AND/OR AST tree
- Functions:
  - `parse_ast()`
  - `parse_pipeline()` (static)
  - `link_node()` (static)
  - `new_node()` (static)

### `parser_utils.c` — extract argv + redirs from tokens
- Reads a token range and produces:
  - `argc`
  - argv strings in `cmd->args`
  - `cmd->redirs` list
- Functions:
  - `is_stop()`
  - `count_args()`
  - `fill_args()`
  - `handle_redir()`

### `parser_utils_2.c` — build one `t_cmd` (simple or subshell), and free AST
- Builds command nodes and advances the token cursor
- Functions:
  - `new_cmd()`
  - `parse_one_cmd()`
  - `parse_subshell()` (static)
  - `advance_to_stop()` (static)
  - `free_ast()`

---

## 5) Stop rules (the most important concept)

You have **two different “stop” concepts**.

### A) Command-level stop: `is_stop()` (parser_utils.c)

```c
int is_stop(t_tktype t)
{
    return (t == T_PIPE || t == T_AND || t == T_OR || t == T_RPAREN);
}
```

Meaning: **a single command ends** when we hit:
- `|`, `&&`, `||`, or `)`

Used by:
- `count_args()`
- `fill_args()`
- `handle_redir()`
- `advance_to_stop()`

### B) AST-term stop: `is_ast_stop()` (parser.c)

```c
static int is_ast_stop(t_token *tk)
{
    return (!tk || tk->type == T_AND
        || tk->type == T_OR || tk->type == T_RPAREN);
}
```

Meaning: **a pipeline ends** when we hit:
- `&&`, `||`, `)`, or end of tokens

Notice: `|` is *not* an AST stop, because pipelines contain pipes.

If you keep these two stop rules clear, the parser becomes much easier to follow.

---

## 6) Step-by-step walkthrough of parsing functions

### 6.1 `parse_ast(t_token **tok)` — build `&&/||` tree (left-associative)

What it does:
1. Parses the first pipeline: `p = parse_pipeline(tok)`
2. Wraps it into a leaf node: `left = new_node(N_PIPELINE, NULL, NULL, p)`
3. While next token is `&&` or `||`:
   - calls `link_node(left, tok)` which returns a new parent
   - assigns it back into `left`

That is exactly why it becomes left-associative:

- `a && b || c` becomes `((a && b) || c)`

### 6.2 `link_node(t_node *left, t_token **tok)` — attach the next pipeline

This is the “glue” between pipelines:

1. Reads `op = (*tok)->type` (`T_AND` or `T_OR`)
2. Consumes the operator: `*tok = (*tok)->next`
3. Parses the right pipeline: `p = parse_pipeline(tok)`
4. Wraps it as a leaf: `right = new_node(N_PIPELINE, NULL, NULL, p)`
5. Creates parent node:
   - `N_AND(left, right)` if op was `T_AND`
   - `N_OR(left, right)` if op was `T_OR`

### 6.3 `parse_pipeline(t_token **tk)` — build `cmd1 | cmd2 | ...`

Loop:
- while `*tk` exists and not `is_ast_stop(*tk)`:
  1. `new = parse_one_cmd(tk)`
  2. append to pipeline list (`head/tail`)
  3. if the next token is `|`, consume it and continue
  4. otherwise stop

So a pipeline is a linked list of `t_cmd` nodes.

### 6.4 `parse_one_cmd(t_token **tk)` — parse one pipeline element

There are 2 cases:

#### A) Subshell command
- if current token is `T_LPAREN`:
  - returns `parse_subshell(tk)`

#### B) Simple command
- builds `t_cmd` via `new_cmd()`
- counts argv with `count_args(start)`
- allocates `cmd->args`
- fills argv strings with `fill_args(cmd, start)`
- collects redirections with `handle_redir(cmd, start)`
- moves cursor forward with `advance_to_stop(tk)`

### 6.5 `count_args()` + `fill_args()` — why redirection targets don’t become argv

Both functions skip redirection targets:

- When they see `<`, `>`, `>>`, `<<`, they do `tk = tk->next` to skip the filename/delimiter.

That’s why:

```
cat < infile
```

becomes:
- argv: `["cat"]`
- redirs: `< infile`

### 6.6 `parse_subshell(t_token **tk)` — `( ... )` becomes a `t_cmd`

This is the nested-parse part:

1. Create command; set `cmd->type = CMD_SUBSHELL`
2. Consume `(`: `*tk = (*tk)->next`
3. Use a separate cursor: `cur = *tk`
4. Parse inner AST: `sub = parse_ast(&cur)`
5. Require a `)`:
   - if `!sub || !cur || cur->type != T_RPAREN` → error
6. Consume `)`: `cur = cur->next`
7. Attach nested AST:
   - `cmd->subshell = sub`
8. Parse redirections after subshell:
   - `handle_redir(cmd, cur)`
9. Update outer cursor:
   - `*tk = cur`
   - `advance_to_stop(tk)`

Key insight:
- `parse_ast(&cur)` consumes tokens only up to `)` because:
  - pipelines stop on `T_RPAREN` via `is_ast_stop()`

---

## 7) AST explained visually (what executor will walk)

Your AST represents only `&&` and `||`.

Node types:
- `N_AND`: run left; if status == 0 → run right
- `N_OR`: run left; if status != 0 → run right
- `N_PIPELINE`: execute the pipeline list (commands joined by `|`)

### Pipelines are stored in leaves

Example:
```
echo hi | cat && ls | wc -l
```

AST shape:
```
        N_AND
       /        PIPELINE  PIPELINE
   (echo|cat) (ls|wc)
```

Each `PIPELINE` leaf contains a `t_cmd` linked list.

### Subshell is “AST inside a command”
For:
```
(echo hi | cat) && ls
```

AST:
```
      N_AND
     /       PIPELINE PIPELINE
   (sub)     (ls)
```

Left leaf pipeline has:
- `cmd->type = CMD_SUBSHELL`
- `cmd->subshell = (inner AST for echo|cat)`

---

## 8) Exact structures for common examples

### 8.1 `(echo hello world) | cat`

```
ROOT: N_PIPELINE
  pipeline:
    cmd#1: CMD_SUBSHELL
      subshell:
        N_PIPELINE
          pipeline:
            cmd: CMD_SIMPLE args=["echo","hello","world"]
    cmd#2: CMD_SIMPLE args=["cat"]
```

### 8.2 `(ls | sleep 1) && (echo hi | cat)`

```
ROOT: N_AND
  left: N_PIPELINE
    pipeline:
      cmd: CMD_SUBSHELL
        subshell:
          N_PIPELINE
            pipeline:
              cmd1 args=["ls"] -> cmd2 args=["sleep","1"]
  right: N_PIPELINE
    pipeline:
      cmd: CMD_SUBSHELL
        subshell:
          N_PIPELINE
            pipeline:
              cmd1 args=["echo","hi"] -> cmd2 args=["cat"]
```

### 8.3 `ls | (cat)`  (same as `ls|(cat)`)

```
ROOT: N_PIPELINE
  pipeline:
    cmd#1: CMD_SIMPLE args=["ls"]
    cmd#2: CMD_SUBSHELL
      subshell:
        N_PIPELINE
          pipeline:
            cmd: CMD_SIMPLE args=["cat"]
```

---

## 9) Memory ownership & error cleanup (who frees what)

### AST freeing
- `free_ast(node)` frees:
  - `node->left`, `node->right`
  - if `node->type == N_PIPELINE`, it also frees `node->pipeline` via `free_cmds()`

### Command freeing
- `free_cmds(cmd)` frees:
  - argv (`cmd->args`)
  - subshell AST (`cmd->subshell` via `free_ast()`)
  - redirs list

### Error paths you should notice
- `parse_pipeline()` frees the partially built pipeline list on failure.
- `link_node()` frees `left` on failure, and also frees `right/p` allocations when needed.
- `parse_subshell()` frees the nested AST and the command on failure.

---

## 10) Debugging tips (quick visualization while studying)

When studying parser behavior, add temporary prints around these points:

1. Print tokens before calling `parse_ast(&tok)`
2. Print the token cursor after `parse_pipeline()` returns
3. Print the AST shape:
   - node type: AND/OR/PIPE
   - for PIPE leaves: print each cmd type + argv[0]

A minimal debug checklist for each input:
- How many `N_PIPELINE` leaves exist?
- Inside each leaf, how many `t_cmd` nodes exist?
- Which commands are `CMD_SUBSHELL` and what does their inner AST look like?

---

## Quick recap (memorize this)

- `parse_ast` builds a **left-associative** tree of `&&/||`.
- Leaves are `N_PIPELINE`, each holding a `t_cmd` linked list for pipes.
- `parse_one_cmd` creates either:
  - `CMD_SIMPLE` with argv+redirs, or
  - `CMD_SUBSHELL` with nested AST in `cmd->subshell`
- Two stop rules:
  - `is_stop`: ends a **command**
  - `is_ast_stop`: ends a **pipeline / AST term**
