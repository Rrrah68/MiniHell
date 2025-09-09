/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rayandahmani <rayandahmani@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 16:52:11 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/09 14:56:34 by rayandahman      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/include/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

/* ---- GLOBAL VARIABLES ---- */
extern volatile sig_atomic_t g_signal_status;
extern int                  g_exit_status;

/* ---- ENUMS ---- */
typedef enum e_token_type {
    WORD,
    WHITESPACE,
    CHAR,
    SINGLE_QUOTE,
    DOUBLE_QUOTE,
    SYMBOL,
    PIPE,
    REDIRECT_IN,
    REDIRECT_OUT,
    REDIRECT_APPEND,
    REDIRECT_HEREDOC,
    HEREDOC,
    OSEF,
}   t_token_type;

typedef enum e_builtin {
    BI_ECHO,
    BI_CD,
    BI_PWD,
    BI_EXPORT,
    BI_UNSET,
    BI_ENV,
    BI_EXIT,
    BI_NONE
}   t_builtin;

/* ---- STRUCTURES ---- */
typedef struct s_token {
    char            c;
    char            *str;
    t_token_type    type;
    int             in_quotes;
    int             from_expansion;
    struct s_token  *next;
    struct s_token  *prev;
}   t_token;

typedef struct s_env {
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef struct s_cmd {
    char            **argv;
    char            *infile;
    char            *outfile;
    char            *heredoc_limiter;
    int             heredoc_fd;
    int             append;
    struct s_cmd    *next;
    int             in_precheck_failed;
    int             in_precheck_errno;
    char            *in_precheck_target;    
    int             out_precheck_failed;
    int             out_precheck_errno;
    char            *out_precheck_target;
}   t_cmd;

typedef struct s_data {
    char            *input;
    char            *prompt;
    t_token         *lexer;
    t_env           *env;
    t_cmd           *cmds;
    int             exit_status;
    int             heredoc_interrupted;
}   t_data;

/* ---- LEXER FUNCTIONS ---- */
void        lexer(t_data *data, char *input);
t_token     *create_token(char c, t_token_type type);
void        add_token(t_token **lexer, t_token *new_token);
void        free_tokens(t_token *head);
int         ft_isspace(int c);
char        *get_type(t_token_type type);

/* ---- QUOTE HANDLING ---- */
int         process_tokens_quotes(t_data *data);
t_token     *handle_single_quote(t_token *current);
t_token     *handle_double_quote(t_token *current);
t_token     *find_closing_quote(t_token *start);
t_token     *find_closing_single_quote(t_token *start);
void        remove_quote_token(t_data *data);
void        handle_quote_removal(t_token *word);

/* ---- TOKEN MANIPULATION ---- */
void        merge_char_tokens(t_token *lexer);
void        merge_double_quoted_content(t_token *lexer);
void        merge_single_quoted_content(t_token *lexer);
void        remove_whitespace_tokens(t_data *data);
void        merge_adjacent_words(t_data *data);
void        merge_adjacent_word_tokens(t_data *data);

/* ---- EXPANSION FUNCTIONS ---- */
void        expand_var(t_data *data);
t_token     *handle_dollar_expansion(t_token *dollar, t_data *data);
t_token     *handle_numeric_variable(t_token *dollar, t_token *next, t_data *data);
t_token     *handle_exit_status(t_token *dollar, t_token *next, t_data *data);
t_token     *handle_double_dollar(t_token *dollar, t_token *next);
char        *extract_var_name(char *str);
char        *extract_var_from_word(char *str);

/* ---- PARSER FUNCTIONS ---- */
t_cmd       *parse_token(t_token *lexer);
t_cmd       *create_cmd(void);
t_cmd       *parser_error(t_cmd *cmds, const char *msg);
void        add_cmd(t_cmd **list, t_cmd *new_cmd);
void        *free_cmd_list(t_cmd *cmd);
int         handle_redir(t_token **tok, t_cmd *cur);
int         handle_pipe(t_cmd **cmds, t_cmd **cur, t_token *tok);

/* ---- EXECUTOR FUNCTIONS ---- */
void        execute_all(t_cmd *cmds, t_data *data);
int         execute_simple_cmd(t_cmd *cmd, t_data *data);
pid_t       fork_or_exit(void);
int         process_cmd(t_cmd *cmd, int in_fd, t_data *data);
void        exec_child(t_data *data, t_cmd *cmd, int in_fd, int out_fd);
void        wait_children(void);

/* ---- REDIRECTION HANDLING ---- */
int         setup_redirections(t_cmd *cmd);
int         setup_pipe_fd(t_cmd *cmd, int p[2], int *out_fd);
int         handle_infile(char *infile);
int         handle_outfile(char *outfile, int append);
int         setup_heredoc(t_cmd *cmd);
void        redirect_and_close(int old_fd, int new_fd);

/* ---- BUILTIN FUNCTIONS ---- */
t_builtin   get_builtin(const char *cmd);
int         exec_builtin(t_builtin bi, char **argv, t_data *data);
int         builtin_echo(char **args);
int         builtin_cd(char **args, t_data *data);
int         builtin_pwd(char **args, t_data *data);
int         builtin_export(char **args, t_data *data);
int         builtin_unset(char **args, t_data *data);
int         builtin_env(char **args, t_data *data);
int         builtin_exit(char **args);

/* ---- ENVIRONMENT FUNCTIONS ---- */
t_env       *copy_environment(char **envp);
void        free_environment(t_env *env);
char        *get_env_value(t_env *env, char *key);
void        add_env_var(t_env **env, const char *key, const char *value);
void        remove_env_var(t_env **env, char *key);
char        **env_to_array(t_env *env);

/* ---- UTILS FUNCTIONS ---- */
void        get_prompt(t_data *data);
void        init_data(t_data *data, char **envp);
void        signal_handler(int sig);
void        update_exit_status(t_data *data);
char        *ft_strjoin_three(char *s1, char *s2, char *s3);
char        *find_program_path(char *program, t_env *env);

#endif