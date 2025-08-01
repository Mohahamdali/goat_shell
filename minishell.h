#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <errno.h> 
# include <sys/stat.h>
# include <signal.h>

typedef enum s_type
{
	STRING,
	RED_IN,
	RED_OUT,
	RED_APPEND,
	RED_HEREDOC,
	PIPE
}	t_type;

typedef struct s_env {
	char				*key;
	char				*content;
	int					append;
	int					exit_status;
	struct s_env		*next;
}   t_env;

typedef struct s_token
{
	char				*content;
	bool				empty_expanded;
	t_type				type;
	bool				quoted;
	t_env				*env;
	struct s_token		*next;
}	t_token;

typedef struct s_redirect 
{
	t_type				type; // Between 1 and 4
	char				*filename;
	bool				filename_fo_exp; // 0 if '\0' come from expander , 1 if file name is '' or ""
	char				*delimiter;
	bool				heredoc_exp; // 0 IF MUST EXPAND HEREDOC , 1 IF HEREDOC MUST STAY AS HE IS
	int					heredoc_fd;
	int					opened_fd;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command
{
	char				**args;
	int					*args_fo_exp;
	char				**ori_env;
	t_env				*env;
	t_redirect			*redirect;
	int					save_out;
	int					save_in;
	struct s_command	*next;
}	t_command;

typedef struct s_heredoc
{
	char	*content;
	t_env	*env;
}	t_heredoc;

typedef struct s_flag
{
	int		i;
	bool	at_redirect;
	bool	found_command;
}	t_flag;

typedef struct s_exp
{
	char	*content;
	char	**expanded;
	t_token	**token;
	char	quote_type;
	int		empty_exp;
	int		m;

}	t_exp;

typedef struct s_blk
{
	void			*ptr;
	struct s_blk	*next;
}	t_blk;

typedef struct	s_garbage
{
	t_blk	*blk;
	int		count;
	int		append;
}	t_garbage;

typedef struct s_last_redirects 
{
    t_redirect *input;
    t_redirect *output;
    t_redirect *append;
    t_redirect *heredoc;
} t_last_redirects;

typedef struct gc_manager
{
	t_garbage	*cmd_gc;
	t_garbage	*env_gc;
}	t_gc_manager;


//toknizer
t_token		*tokenizer(char *str,t_env *env, t_garbage *cmd_gc);

//envirenement
t_env		*set_env(char **env, t_garbage *g_c);
void		clear_env(t_env *env_head);
void		free_default_env(char **env);
char		**create_default_env(t_garbage *g_c);

//syntax error
bool		syntax_error(t_token *tokens);

//expander
void		handle_quotes(char *content, int *i, char *quote_type);
int			expanded_size(char *content, t_env *env, int i);
int			expand_tokens(t_token **token, t_env *env, t_garbage *cmd_gc);
char		*expander(t_token **token, char *content, t_env *env, t_garbage *g_c);
int			expand_alphanumeric(t_exp *exp, int *i, int *x, t_env *env, t_garbage *g_c);
int			count_strings(char *content);
char		*replace_env_key(char *key, t_env *env, t_garbage *g_c);
bool		is_only_vars(char *content);
void		init_vars(t_exp *exp, int *i, int *x, char *quote_type);
void		dollar_underscore(char *content, char **expanded, int *i, int *x);
void		expand_exit_status(char **expanded, int *i, int *x, t_env *env, t_garbage *g_c);


// expander heredoc
int	must_expand(t_command *cmd, t_garbage *g_c);

//parser
t_command	*parser(t_env *env, char *line, char **envv, t_garbage *cmd_gc);
int			handle_multispace_tokens(t_token *token, t_command **list, t_flag *fg, int args_nb, t_garbage *g_c);
int			fill_list_args(t_command **list, t_token *token, t_flag *fg, bool found_cmd, t_garbage *g_c);
int			handle_redirect_filename(t_command **list, t_token *token, bool *at_redirect, t_garbage *g_c);
int			init_redirect(t_command **list, t_token *token, bool *at_redirect, t_garbage *g_c);
t_redirect	*redirect_alloc(t_garbage *g_c);
int			init_args_nb(t_command **list, t_token *token, int *args_nb, t_redirect **head, t_garbage *g_c);
void		init_fg(t_flag *fg);
int			handle_string_token(t_command **list, t_token **tmp, int args_nb, t_flag *fg, t_garbage *g_c);
int			handle_pipe(t_command **list, t_token **tmp, t_redirect **redirect_head, t_garbage *g_c);
int			init_head_list(t_command **list, t_command **list_head, t_garbage *g_c);
t_command	*create_command(t_garbage *g_c);


//garbage collector
t_garbage	*gc_init(void);
void		*g_malloc(t_garbage *g_c, size_t size);
char		*g_strdup(t_garbage *g_c, const char *s);
void		g_free_one_ptr(t_garbage *g_c, void *ptr);
void		cleanup_grb_cltr(t_garbage *g_c);
char		*g_substr(t_garbage *g_c, char const *s, unsigned int start, size_t len);

//signals
void		init_signals(void);
void		init_heredoc_signals(void);

//libft
char    *ft_strcpy(char *dst, const char *src);
char	*trimmed(char *str, t_garbage *g_c);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
int		ft_isalnumm(int c);
char	*ft_itoa(int n, t_garbage *g_c);
int		ft_strcmp(const char *s1, const char *s2);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	**my_split(char *s, t_garbage *g_c);
int		count_word(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2, t_garbage *gc);
char	**ft_split(char const *s, char c, t_garbage *gc);
size_t	ft_strlcpy(char *dest, const char *src, size_t dest_size);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
int     ft_strncmp(const char *str1, const char *str2, size_t n);
int		ft_atoi(const char *str, t_command *cmd, t_gc_manager *g_c);
int     ft_isalpha(int c);
int     ft_isdigit(int c);
char	*ft_strchr(const char *str, int c);
int     ft_isalnum(int c);

// builtin
void	execution_builtins (t_command *cmd, t_env **env, int flag, t_gc_manager *g_c);
int     is_builtins (t_command *cmd);
int		buittin_echo (t_command *cmd);
void    built_pwd (t_command *cmd);
int		env_built (t_command *cmd);
int		cd_builtin (t_command *cmd, t_garbage *gc);
int		export(t_command *cmd, t_env **env, t_garbage *gc);
void	unset(t_env **env, char *key);
void    builtin_exit (t_command *cmd, int flag, t_gc_manager *g_c);

// execustion
int		execution(t_command *cmd, t_env **env, int flag, t_gc_manager *g_c);
void	external_command(char *path, t_command *command, t_gc_manager *g_c);
void    free_env_list(t_env *env);
char	*get_env_var(char **env, const char *var_name);
int		apply_redirections (t_command *cmd);
void	external_command_pipe (char *path, t_command *command, t_gc_manager *gc);
void	execution_pipes (t_command *cmd, t_gc_manager *gc);
void	execute_child_process(char *path, t_command *cmd, t_gc_manager *gc);

// utils external command
int     is_dire(char *path);
int     path_exists(const char *path);
int     is_executable(const char *path);
char	**convert_linked_to_char(t_env *env, t_garbage *gc);
void	handle_local_path(t_command *command, t_gc_manager *gc);
void	search_in_path(char *path, t_command *command, t_gc_manager *gc);
//export
int		parse_export (char *arg);
void	sort_export (t_env *env);
int		is_append_operation(char *arg);
char	*determine_content(char *args, t_garbage *gc);
t_env	*create_env_node(char *key, char *value, t_garbage *gc);
void    append_env_node(t_env **head, t_env *new_node);
char    *determine_key(char *argv, t_garbage *gc);
int     change_env_already(t_env *env, char *key, char *value, t_garbage *gc);
void	print_env(t_env *env);

void    execute_path_command(char *path, t_command *cmd, t_gc_manager *gc);
void    free_env(char **env);
void	unset_all_env(t_env **env, char **keys);


//herdoc utils
int	 handle_all_heredocs(t_command *cmd);
int		handle_heredoc (t_redirect *redirect, t_env *env);

//error_function
void	cd_error_write(char *target);
void	print_exit_error(char *arg);
void	print_numeric_error(char *arg);

//exit status
int last_exit_status(int status, int set);

void	close_all_pipes(int **pipe_fd, int n_cmd);
void	handle_exit_status(int status);
int		count_cmd (t_command *cmd);
int		create_and_append_node(t_env **env, char *key, char *content, t_garbage *gc);
int		update_env_content(t_env *env, char *content, int append, t_garbage *gc);
void	free_pipes(int **pipe_fd, int count);
void	cleanup_resources(int **pipe_fd, pid_t *pids, int n_cmd);
void	close_pipes(int **pipe_fd, int count);
void	close_parent_pipes(int **pipe_fd, int n_cmd);
int		**create_pipes(int n_cmd);
char	*get_cd_target(t_command *cmd, char *oldpwd, char *pwd_env);
void	update_pwd_env_cd_dotdot(char **pwd_env);
void	kill_all_children(pid_t *pids, int count);
void	wait_for_children(pid_t *pids, int n_cmd);
t_env	*find_env_var(t_env *env, const char *key);
void	clean_save_in (t_command *cmd);
int		cd_error_handler(char *oldpwd, char *pwd_env);
int		save_output_redi(t_redirect *redirect);
int		handle_input_redi(t_redirect *redirect);
int		handle_output_redi(t_redirect *redirect);
char	*get_tmp_filename(void);
int		open_heredoc_file(char *tmpfile);
#endif