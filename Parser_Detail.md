# Below is a detailed walkthrough about Parser:

- parser.c
- parser_utils.c
- parser_utils_2.c

## 1) What is the parser’s purpose in your minishell?

After the lexer, you have a linked list of tokens like:

WORD(echo) WORD(hi) PIPE(|) WORD(cat) AND(&&) LPAREN(() WORD(ls) RPAREN())

That list is still “flat”. The parser’s job is to:

Group tokens into commands (t_cmd)

argv (cmd->args)

redirections (cmd->redirs)

subshell payload (cmd->subshell if ( ... ))

Chain commands into pipelines (cmd1 -> cmd2 -> cmd3 for |)

Build an AST for && and || (t_node)

leaves are pipelines

internal nodes are AND/OR logic

So execution becomes simple:

evaluate left branch → depending on status run right branch

when you reach a leaf → run pipeline list

2) What “structure” does your parser output?
A) Pipeline list (t_cmd*)

For this input:

echo hi | cat

Parser produces:

a linked list of t_cmd nodes:

cmd1(args=["echo","hi"], redirs=...)  ->  cmd2(args=["cat"], redirs=...)
B) AST for && / || (t_node*)

For:

echo hi | cat && ls

Parser produces:

        N_AND
       /     \
  PIPELINE   PIPELINE
   (echo|cat)   (ls)

That’s the whole point: executor doesn’t need to “parse tokens” again, it just walks this structure.

3) Why you have 3 parser files

This is very normal in 42 projects:

parser.c — “big picture” composition

Builds:

pipeline lists

AND/OR AST tree

No argv copying logic here, no redir list building here

parser_utils.c — “extract data from tokens”

Count argv size

Copy argv strings

Collect redirections

parser_utils_2.c — “construct command nodes + subshell”

Allocate t_cmd

Parse ( ... ) recursively using parse_ast

Free AST

So it’s split by responsibility + Norm/file-size sanity.

4) The parser logic step-by-step (your exact functions)
4.1 parse_ast(t_token **tok) in parser.c

This parses only && and || at the “AST level”.

What it does:

Parse the first pipeline:

p = parse_pipeline(tok);

Wrap it into a leaf node:

left = new_node(N_PIPELINE, NULL, NULL, p);

While next operator is && or ||:

repeatedly “link” another pipeline onto the tree via link_node

Important behavior:

It stops naturally when:

token stream ends, or

we hit ) (because your pipeline parsing uses is_ast_stop())

So parse_ast can be used inside subshell parsing safely.

4.2 parse_pipeline(t_token **tk) in parser.c

This parses sequences joined by |.

It loops:

new = parse_one_cmd(tk);

Append to linked list (head/tail)

If current token is |, consume it and continue

Stop if:

end

&& / ||

) (this is critical for subshell)

So pipeline precedence is naturally higher than &&/||.

4.3 parse_one_cmd(t_token **tk) in parser_utils_2.c

This parses one command element inside a pipeline:

either a simple command (argv + redirs)

or a subshell command ( ... ) + possible redirs after it

Flow:

Guard:

if *tk is NULL or already a stop token → return NULL

If token is ( → parse subshell:

return parse_subshell(tk);

Else parse simple command:

argc = count_args(start);

allocate cmd->args

fill_args(cmd, start)

handle_redir(cmd, start)

advance *tk to the stop token using advance_to_stop(tk)

Key idea:

This function consumes tokens by moving *tk forward.

That’s why it takes t_token ** instead of t_token *.

5) The “confusing” helper functions (what they actually do)
is_stop(t_tktype t) in parser_utils.c

Stop tokens are:

|, &&, ||, )

This defines “where a command ends”.

Redirections are not stops, because they belong to the command.

count_args(t_token *tk) in parser_utils.c

Counts how many argv words belong to this command.

Crucial detail:

It does not count redirection targets as argv.

How?

When it sees a redirection token (<, >, >>, <<), it does:

tk = tk->next; (move to filename/delimiter)

then it advances again at loop bottom
So it skips the target word.

fill_args(t_cmd *cmd, t_token *tk) in parser_utils.c

Copies only “real argv words” into cmd->args[].

Same trick:

it skips redirection operator + its target

so filenames don’t become argv

handle_redir(t_cmd *cmd, t_token *tk) in parser_utils.c

Collects redirections into cmd->redirs list.

If it sees a redirection token and tk->next exists:

new_redir(tk->type, tk->next->value)

add to cmd

For heredoc, tk->next->value is stored as delimiter as typed (quotes included). Later heredoc logic decides whether expansion happens.

advance_to_stop(t_token **tk) in parser_utils_2.c

This is a “cursor mover”:

move *tk forward until |, &&, ||, ) or NULL

It makes sure after parsing one command, your pipeline loop can see whether it should continue.

6) Subshell parsing (this is where people get lost)
parse_subshell(t_token **tk) in parser_utils_2.c

This is the core idea:

( begins a subshell command node (CMD_SUBSHELL)

inside parentheses, you parse a full AST again

you stop when you reach the matching )

Your function does:

Create cmd and mark type CMD_SUBSHELL

Consume (:

*tk = (*tk)->next;

Use a separate cursor cur = *tk;

Parse inner AST:

sub = parse_ast(&cur);

Validate it ended on ):

cur must exist and cur->type == T_RPAREN

Move cur past ):

cur = cur->next;

Attach the inner AST to the command:

cmd->subshell = sub;

Parse redirections that come after the subshell:

handle_redir(cmd, cur)

Update main token cursor and advance to stop:

*tk = cur;

advance_to_stop(tk);

Why this “two-pointer” thing matters:

parse_ast(&cur) consumes tokens until it hits ) (because your pipeline parser stops on T_RPAREN)

the outer parser doesn’t lose its place, because it only updates *tk after the subshell is done.

7) A concrete trace you can follow in your head

Input:

(echo hi | cat) > out && ls

Token stream (simplified):

( echo hi | cat ) > out && ls
Parsing:

parse_ast → calls parse_pipeline

parse_pipeline → calls parse_one_cmd

parse_one_cmd sees ( → parse_subshell

Inside parse_subshell:

calls parse_ast(&cur) to parse echo hi | cat

stops when cur points to )

then it parses redirections after ):

sees > + out → adds to cmd->redirs

returns one t_cmd node (type subshell)

Back to outer:

after pipeline leaf is done, parse_ast sees && → links right pipeline (ls)

Execution later:

left leaf is “a command that runs AST in child”

redirection > out applies to the subshell command’s stdout

8) What parser is NOT doing (important for clarity)

Your parser is not responsible for:

syntax correctness (mostly handled earlier in syntax.c)

expansion ($VAR, quotes, splitting) → handled in expand files

wildcard expansion → handled later

redirection application / heredoc reading → handled in executor/redirection

Parser only:

shapes the structure

stores raw strings (argv, redir targets, heredoc delimiter)

9) How to stop getting lost while reading parser code

When you read parser code, keep only two questions in mind:

What structure is being built right now?

AST node? pipeline list? command node?

Who is moving the token cursor (t_token **)?

parse_pipeline / parse_one_cmd / parse_subshell all “consume tokens”

advance_to_stop is the “skip to next operator” step


AST (Abstract Syntax Tree) in your minishell is basically a tree-shaped “execution plan” for && and ||. It answers: “What should I run first, and what depends on the previous exit status?”

The key is: your AST only represents && / ||.
Pipes | are not AST nodes in your project — they live inside the AST leaf as a linked list of t_cmd.

1) What your AST nodes mean (visual)

In your codebase, think of 3 node types:

N_AND: run left; if exit status == 0, then run right

N_OR: run left; if exit status != 0, then run right

N_PIPELINE (leaf): contains a pipeline (a t_cmd* list)

So an AST node looks like:

           [N_AND]
           /     \
      left expr  right expr

And a leaf looks like:

[N_PIPELINE]
    |
  cmds:  cmd1 -> cmd2 -> cmd3   (this is the '|' pipeline list)
2) Why AST exists (the purpose)

Without AST, you would have to re-scan tokens during execution to know what to do next.

With AST:

Parsing decides the structure once.

Execution becomes: walk the tree and apply short-circuit rules.

That’s why executor code is clean:

execute_ast(node)

if AND: run left → maybe run right

if OR: run left → maybe run right

if PIPELINE: run the pipeline

3) Visual examples (most important)
Example A: a && b
      [N_AND]
      /     \
 [PIPE]   [PIPE]
  a        b

Execution:

Run a

If a returns 0 → run b

Otherwise skip b

Example B: a || b
      [N_OR]
      /    \
 [PIPE]  [PIPE]
  a       b

Execution:

Run a

If a returns non-zero → run b

Otherwise skip b

Example C: a && b || c (left-associative)

In shell, && and || have the same precedence and are evaluated left-to-right.

So it becomes:

          [N_OR]
          /    \
      [N_AND]  [PIPE]
      /    \     c
 [PIPE]  [PIPE]
   a       b

Meaning:

Evaluate (a && b) first

Then ... || c

Example D: pipelines live inside leaves

echo hi | cat && ls | wc -l

AST:

        [N_AND]
        /     \
   [PIPE]     [PIPE]
    |          |
  echo->cat   ls->wc

Leaf detail (what’s inside a [PIPE]):

[PIPELINE leaf]
   cmds = cmd1 -> cmd2 -> ...
   cmd1.args = ["echo","hi"]
   cmd2.args = ["cat"]
4) How parentheses (subshell) relates to AST

When you type:
(echo hi | cat) && ls

Your AST is still N_AND, but the left leaf pipeline contains a subshell command:

        [N_AND]
        /     \
   [PIPE]     [PIPE]
    |           |
 (subshell)     ls

And that subshell command has its own AST inside cmd->subshell:

cmd.type = CMD_SUBSHELL
cmd.subshell =   [PIPE]
                   |
                echo->cat

So you effectively get a “tree inside a command” (AST nested inside the pipeline leaf).

5) A very accurate mental model
Your parser builds:

AST for && / ||

leaves hold pipeline lists (t_cmd* chain for |)

a subshell is a t_cmd whose payload is another AST (cmd->subshell)

Your executor uses:

short-circuit evaluation for && / ||

pipeline exit status = status of last command in the pipeline

subshell execution = run execute_ast(cmd->subshell) in a child (so it doesn’t mutate parent shell state)

6) Visualizing “how parse_ast builds the tree” (why it becomes left-associative)

Your parse_ast works like:

parse first pipeline → make it the current left

while next token is && or ||:

parse next pipeline as right

make new node: left = new_node(op, left, right)

That pattern always produces a left-growing tree:

a && b || c becomes ((a && b) || c).

7) If you want an even clearer visualization

Tell me 3 real commands you test often (including parentheses + pipes), for example:

(echo hello world) | cat

(ls | sleep 1) && (echo hi | cat)

ls | (cat)

