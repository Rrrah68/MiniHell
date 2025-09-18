/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:27:41 by mobullad          #+#    #+#             */
/*   Updated: 2025/09/18 22:22:56 by mobullad         ###   ########.fr       */
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

typedef enum e_token_type
{
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
}								t_token_type;

typedef struct s_token
{
	char						c;
	char						*str;
	t_token_type				type;
	int							in_quotes;
	int							from_expansion;
	struct s_token				*next;
	struct s_token				*prev;
}								t_token;

typedef struct s_env
{
	char						*key;
	char						*value;
	struct s_env				*next;
}								t_env;

typedef struct s_cmd			t_cmd;

typedef struct s_data
{
	char						*input;
	char						*prompt;
	t_token						*lexer;
	t_env						*env;
	t_cmd						*cmds;
	int							exit_status;
	int							heredoc_interrupted;
}								t_data;

typedef struct s_cmd
{
	char						**argv;
	char						*infile;
	char						*outfile;
	char						*heredoc_limiter;
	char						*heredoc_content;
	int							heredoc_fd;
	int							heredoc_quoted;
	int							append;
	struct s_cmd				*next;
	int							in_precheck_failed;
	int							in_precheck_errno;
	char						*in_precheck_target;
	int							out_precheck_failed;
	int							out_precheck_errno;
	char						*out_precheck_target;
}								t_cmd;

typedef struct s_fork_params
{
	t_cmd						*cmd;
	int							in_fd;
	int							*p;
	int							*out_fd;
	pid_t						*pid;
}								t_fork_params;

typedef struct s_finalize_params
{
	t_cmd						*cmd;
	int							in_fd;
	int							*p;
	pid_t						pid;
}								t_finalize_params;

typedef enum e_builtin
{
	BI_ECHO,
	BI_CD,
	BI_PWD,
	BI_EXPORT,
	BI_UNSET,
	BI_ENV,
	BI_EXIT,
	BI_NONE
}								t_builtin;

typedef struct s_exec_error_params
{
	char						*cmd_name;
	char						*program_path;
	char						**env_array;
	t_data						*data;
	t_cmd						*cmd;
	int							error_type;
}								t_exec_error_params;

typedef struct s_cleanup_params
{
	char						*program_path;
	char						**env_array;
	t_data						*data;
	t_cmd						*cmd;
}								t_cleanup_params;

extern volatile sig_atomic_t	g_signal_status;

/************** INITIALIZATION & MAIN **************/
void							init_data(t_data *data, char **envp);
void							reset_heredoc_interrupt(t_data *data);
int								is_incomplete_input(const char *input);
void							get_prompt(t_data *data);

/************** LEXER **************/
void							lexer(t_data *data, char *input);
t_token							*create_token(char c, t_token_type type);
void							add_token(t_token **lexer, t_token *new_token);
void							free_tokens(t_token *head);
char							*get_type(t_token_type type);

/************** INPUT HANDLER **************/
int								handle_input(t_data *data);
int								handle_continuation_input(char **complete_input);

/************** MAIN EXECUTION **************/
int								process_and_execute(t_data *data);
int								main_loop(t_data *data);

/************** TOKEN MANAGEMENT **************/
void							remove_next_token(t_token *current);
t_token							*find_previous_token(t_token *head,
									t_token *target);
void							remove_quote_from_list(t_data *data,
									t_token *to_remove, t_token *prev);
void							remove_whitespace_tokens(t_data *data);

/************** QUOTES HANDLING **************/
int								process_tokens_quotes(t_data *data);
t_token							*handle_single_quote(t_token *current);
t_token							*handle_double_quote(t_token *current);
void							remove_quote_token(t_data *data);
t_token							*find_closing_quote(t_token *start);
t_token							*find_closing_single_quote(t_token *start);
char							*build_quoted_content(t_token *start,
									t_token *end);
void							handle_quote_removal(t_token *word);

/************** TOKEN MERGING **************/
void							merge_char_tokens(t_token *lexer);
void							merge_double_quoted_content(t_token *lexer);
void							merge_single_quoted_content(t_token *lexer);
void							merge_adjacent_words(t_data *data);
void							merge_adjacent_word_tokens(t_data *data);
char							*process_merge_char_tokens(t_token *start);
void							cleanup_intermediate_tokens(t_token *start,
									t_token *end);

/************** WORD TOKEN CREATION **************/
t_token							*create_word_token(char *word_str);
t_token							*create_word_token_with_content(char *content,
									t_token *end_quote);
void							setup_existing_word_token(t_token *word_token,
									char *content, t_token *end_quote);
t_token							*split_word_on_spaces(char *str);

/************** TOKEN PROCESSING **************/
void							handle_space_token(t_token **current,
									t_token **prev);
void							handle_char_token(t_token **current,
									t_token **prev);

/************** OPERATORS HANDLING **************/
void							handle_operators(t_data *data);
void							handle_single_operators(t_data *data);
void							handle_append_redirect(t_token *current);
void							handle_heredoc_redirect(t_token *current);
void							handle_single_operator_type(t_token *current);

/************** VARIABLE EXPANSION **************/
void							expand_var(t_data *data);
t_token							*handle_dollar_expansion(t_token *dollar,
									t_data *data);
t_token							*handle_exit_status(t_token *dollar,
									t_token *next, t_data *data);
t_token							*handle_double_dollar(t_token *dollar,
									t_token *next);
t_token							*handle_numeric_variable(t_token *dollar,
									t_token *next, t_data *data);
void							handle_partial_expansion(t_token *dollar,
									t_token *next, char *var_value,
									char *var_name);
int								process_expansion_loop(t_data *data);
t_token							*expand_single_variable(t_token **dollar,
									t_data *data);
t_token							*expand_word_variable(t_token *word,
									t_data *data);
int								check_special_cases(t_token *dollar,
									t_data *data);
void							replace_and_remove_next(t_token *current,
									t_token *to_remove, char *new_value);
void							replace_with_expansion(t_token *dollar,
									t_token *word, char *value, char *var_name);
void							perform_variable_expansion(t_token *dollar,
									t_token *word_token, char *var_name,
									char *var_value);
t_token							*process_variable_expansion(t_token **dollar,
									t_token *word_token, t_data *data,
									int var_len);
t_token							*handle_dollar_not_found(t_token **dollar);
t_token							*handle_invalid_word_token(t_token **dollar);
void							remove_double_dollars(t_token *word);

/************** VARIABLE NAME EXTRACTION **************/
char							*extract_var_name(char *str);
char							*extract_var_from_word(char *str);
char							*collect_variable_name(t_token *start);
char							*collect_var_name(t_token *start);
int								find_valid_var_length(char *str, t_data *data);
char							*get_variable_value(char *dollar_pos,
									int *var_len, t_data *data);
int								update_word_content(t_token *word,
									char *new_str);

/************** ENVIRONMENT MANAGEMENT **************/
t_env							*copy_environment(char **envp);
void							free_environment(t_env *env);
t_env							*create_env_node(const char *key,
									const char *value);
char							*get_env_value(t_env *env, char *key);
char							*get_env(char **envp, const char *name);
void							add_env_var(t_env **env, const char *key,
									const char *value);
void							remove_env_var(t_env **env, char *key);
char							**env_to_array(t_env *env);
int								count_env_vars(t_env *env);
char							*create_env_string(t_env *current);
void							print_exported_vars(t_env *env_list);

/************** SIGNALS **************/
void							signal_handler(int sig);
void							continuation_signal_handler(int sig);
void							heredoc_signal_handler(int sig);
int								setup_heredoc_signal_handler(struct sigaction
									*old_action);
int								restore_signal_handler(struct sigaction
									*old_action);
int								wait_child_with_signals(pid_t pid);
void							update_exit_status(t_data *data);

/************** UTILITIES **************/
int								ft_isspace(int c);
char							*ft_strjoin_three(char *s1, char *s2, char *s3);
char							*find_program_path(char *program, t_env *env);

/************** PARSING **************/

t_cmd							*create_cmd(void);
t_cmd							*parser_error(t_cmd *cmds, const char *msg);
t_cmd							*parse_token(t_token *lexer);

void							add_cmd(t_cmd **list, t_cmd *new_cmd);
void							free_cmd_list(t_cmd *cmd);
void							cleanup_data(t_data *data);

char							**append_to_argv(char **argv, char *word);

int								handle_redir(t_token **tok, t_cmd *cur);
int								handle_pipe(t_cmd **cmds, t_cmd **cur,
									t_token *tok);
int								process_token(t_token **tok, t_cmd **cmds,
									t_cmd **cur);
int								process_redirect(t_token **tok, t_cmd *cur,
									t_cmd *cmds);
int								process_pipe(t_token *tok, t_cmd **cmds,
									t_cmd **cur);

/************** EXECUTOR **************/

pid_t							fork_or_exit(void);

int								setup_pipe_fd(t_cmd *cmd, int p[2],
									int *out_fd);
int								handle_infile(char *infile);
int								handle_outfile(char *outfile, int append);
int								process_cmd(t_cmd *cmd, int in_fd,
									t_data *data);
int								setup_redirections(t_cmd *cmd, t_data *data);
int								setup_heredoc(t_cmd *cmd, t_data *data);
int								open_infile(char *infile);
int								get_outfile_flags(int append);
int								open_outfile(char *outfile, int append);
int								execute_simple_cmd(t_cmd *cmd, t_data *data);
int								is_variable_assignment(char *str);
int								handle_variable_assignment(char *assignment,
									t_data *data);
int								handle_builtin_parent(t_builtin bi, char **argv,
									t_data *data);
int								handle_builtin_child(t_builtin bi, char **argv,
									t_data *data);

void							execute_all(t_cmd *cmds, t_data *data);
void							redirect_and_close(int old_fd, int new_fd);
void							exec_child(t_data *data, t_cmd *cmd, int in_fd,
									int out_fd);
void							wait_children(t_data *data);
void							backup_fds(int *in_backup, int *out_backup);
void							restore_fds(int in_backup, int out_backup);
char							*check_path_directories(char **paths,
									char *program);
char							*prepare_program_path(t_cmd *cmd, t_env *env,
									t_data *data, int *has_slash);
void							print_exec_error_and_exit(
									t_exec_error_params *params);
int								handle_var_assignment_cmd(t_cmd *cmd,
									t_data *data, int in_backup,
									int out_backup);
int								execute_cmd_final(t_cmd *cmd, t_data *data,
									int in_backup, int out_backup);
void							child_run_exec(t_data *data, t_cmd *cmd);
int								handle_builtin_execution(t_cmd *cmd,
									t_data *data);

/************** BUILTINS **************/
t_builtin						get_builtin(const char *cmd);

int								exec_builtin(t_builtin bi, char **argv,
									t_data *data);
int								builtin_echo(char **args);
int								builtin_cd(char **args, t_data *data);
int								get_oldcwd(char *oldcwd);
char							*resolve_target(char **argv, t_data *data);
int								builtin_pwd(char **args, t_data *data);
int								builtin_export(char **args, t_data *data);
int								builtin_unset(char **args, t_data *data);
int								builtin_env(char **args, t_data *data);
int								builtin_exit(char **args);

/************** HEREDOC UTILS **************/
int								is_delimiter(const char *line,
									const char *delimiter);
void							write_heredoc_line(int fd, const char *line);
int								handle_heredoc_with_content(const char
									*content);
char							*read_heredoc_line_input(void);
void							handle_heredoc_eof_warning(
									const char *delimiter);
char							*append_to_result(char *result, char *temp);
char							*process_character(const char *line, int *i,
									t_data *data);
char							*expand_var_in_heredoc(const char *line, int i,
									int *new_i, t_data *data);

/************** PARSER REDIRECT UTILS **************/
void							ms_precheck_infile(t_cmd *cur,
									const char *path);
void							ms_precheck_outfile(t_cmd *cur,
									const char *path, int do_append);
int								ms_replace_str(char **dst, const char *src);
int								cmd_set_infile(t_cmd *cur, const char *str);
int								cmd_set_outfile(t_cmd *cur, const char *str,
									int append);

/************** PARSER CMD UTILS **************/
int								cmd_set_heredoc_limiter(t_cmd *cur,
									const char *str, int quoted);
int								cmd_add_redir(t_cmd *cur, t_token_type type,
									char *str, int quoted);

#endif