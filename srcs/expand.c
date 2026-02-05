/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phonekha <phonekha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:07:36 by phonekha          #+#    #+#             */
/*   Updated: 2026/02/05 16:00:46 by phonekha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* Helper to find a value in your existing linked list */
char *env_get(t_env *env, char *key)
{
    t_env *node;

    node = find_env_node(env, key);
    if (node)
        return (node->value);
    return (NULL);
}

/* Helper to join strings and free the old buffer */
char *append_str(char *res, char *to_append)
{
    char *new;

    if (!to_append)
        return (res);
    new = ft_strjoin(res, to_append);
    free(res);
    return (new);
}

/* Helper to join a single character to the buffer */
char *append_char(char *res, char c)
{
    char tmp[2];

    tmp[0] = c;
    tmp[1] = '\0';
    return (append_str(res, tmp));
}

char *handle_dollar(char *res, char *s, int *i, t_shell *sh)
{
    char    *val;
    int     start;

    if (!s[*i + 1] || s[*i + 1] == ' ' || s[*i + 1] == '\t' || s[*i + 1] == '\0')
        return (res = append_char(res, s[(*i)++]));

    (*i)++; // skip the first '$'
    
    // 1. Handle Exit Status ($?)
    if (s[*i] == '?')
    {
        val = ft_itoa(sh->last_status);
        res = append_str(res, val);
        free(val);
        (*i)++;
    }
    // 2. NEW: Handle Process ID ($$)
    else if (s[*i] == '$')
    {
        // In 42 projects, you can use getpid() if permitted, 
        // or just a hardcoded dummy number if not.
        val = ft_itoa(getpid()); 
        res = append_str(res, val);
        free(val);
        (*i)++;
    }
    // 3. Handle Variable Name
    else if (ft_isalpha(s[*i]) || s[*i] == '_')
    {
        start = *i;
        while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
            (*i)++;
        char *key = ft_substr(s, start, *i - start);
        val = env_get(sh->env, key);
        free(key);
        if (val)
            res = append_str(res, val);
    }
    else
        res = append_char(res, '$');
    return (res);
}


char *handle_single_quote(char *res, char *s, int *i)
{
    int j = *i + 1;

    // Check if there is a closing quote anywhere later in the string
    while (s[j] && s[j] != '\'')
        j++;

    if (s[j] == '\0') // No closing quote found!
    {
        res = append_char(res, s[(*i)++]); // Treat current ' as literal
        return (res);
    }

    (*i)++; // skip opening '
    while (*i < j)
    {
        res = append_char(res, s[*i]);
        (*i)++;
    }
    (*i)++; // skip closing '
    return (res);
}

char *handle_double_quote(char *res, char *s, int *i, t_shell *sh)
{
    int j = *i + 1;

    while (s[j] && s[j] != '"')
        j++;

    if (s[j] == '\0') // No closing double quote
    {
        res = append_char(res, s[(*i)++]);
        return (res);
    }

    (*i)++; // skip "
    while (*i < j)
    {
        if (s[*i] == '$')
            res = handle_dollar(res, s, i, sh);
        else
            res = append_char(res, s[(*i)++]);
    }
    (*i)++; // skip "
    return (res);
}

char *expand_str(char *s, t_shell *sh)
{
    int     i;
    char    *res;

    if (!s)
        return (NULL);
    i = 0;
    res = ft_strdup("");
    while (s[i])
    {
        if (s[i] == '\'')
            res = handle_single_quote(res, s, &i);
        else if (s[i] == '"')
            res = handle_double_quote(res, s, &i, sh);
        else if (s[i] == '$')
            res = handle_dollar(res, s, &i, sh); // handle_dollar must increment i!
        else
        {
            res = append_char(res, s[i]);
            i++; // Only increment if no handler was called
        }
    }
    free(s);
    return (res);
}

void expand_cmds(t_cmd *cmds, t_shell *sh)
{
    int i;
    t_cmd *curr;

    curr = cmds;
    while (curr)
    {
        // 1. Expand Arguments
        i = 0;
        while (curr->args && curr->args[i])
        {
            curr->args[i] = expand_str(curr->args[i], sh);
            i++;
        }
        // 2. Expand Redirections with Ambiguous Check
        if (curr->infile)
        {
            char *original = curr->infile;
            curr->infile = expand_str(curr->infile, sh);
            if (!curr->infile || curr->infile[0] == '\0')
                fprintf(stderr, "minishell: %s: ambiguous redirect\n", original);
        }
        if (curr->outfile)
        {
            char *original = curr->outfile;
            curr->outfile = expand_str(curr->outfile, sh);
            if (!curr->outfile || curr->outfile[0] == '\0')
            {
                fprintf(stderr, "minishell: %s: ambiguous redirect\n", original);
                sh->last_status = 1; // Mark as error
            }
        }
        curr = curr->next;
    }
}
