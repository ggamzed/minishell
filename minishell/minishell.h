#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <limits.h>
//# include <sys/wait.h>

// PATH_MAX güvenlik kontrolü -> cd fonksiyonunda kullanmak için
#ifndef PATH_MAX
# ifdef _POSIX_PATH_MAX
#  define PATH_MAX _POSIX_PATH_MAX
# else
#  define PATH_MAX 4096  // eski sistemlerde tanımlı olmayabiliyormuş, onun için
# endif
#endif
# define PROMPT "minishell$ "

//token tipleri
typedef enum e_token_type
{
	WORD,					// echo, hello, /bin/ls
	SINGLE_QUOTED_STRING,	// 'hello world'
	DOUBLE_QUOTED_STRING,	// "hello $USER"
	VARIABLE,				// $USER, $HOME
	EXIT_STATUS,			// $?
	PIPE,					// |
	REDIRECT_IN,			// < (stdin redirection)
	REDIRECT_OUT,			// > (stdout redirection)
	REDIRECT_APPEND,		// >> (output append)
	HEREDOC,				// << (here document)
}	t_token_type;

//token yapısı
typedef struct s_token
{
	t_token_type	type;		//token hangi tipte?
	char			*value;		//token içeriği
	struct s_token	*next;		//sonraki node'un adresi
}	t_token;

// // command argument yapısı
// typedef struct s_cmd_arg
// {
// 	char			*value;          // Raw token value
// 	t_token_type	type;           // Token type (WORD, VARIABLE, DOUBLE_QUOTED, etc.)
// 	struct s_cmd_arg *next;
// }	t_cmd_arg;

// command yapısı -> tek bir komutu temsil eder 
typedef struct s_cmd
{
	t_token			*args;              // Linked list of arguments with types
	char 			**expanded_argv;	// argümanların expand edilmiş bir şekilde tutulduğu hali
	char			*input_file;        // < input.txt (raw value)
	t_token_type	input_type;         // Input file token type
	char			*output_file;       // > output.txt (raw value)  
	t_token_type	output_type;        // Output file token type
	int				append_mode;        // >> modu (1) veya > modu (0)
	char			*heredoc_delimiter; // << eof (raw value)
	t_token_type	heredoc_type;       // Delimiter token type
	int				heredoc_fd;         // heredoc için file descriptor
	struct s_cmd	*next;              // pipe'daki sonraki komut
} t_cmd;

// environment (çevre değişkenleri) yapısı
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

// main shell yapısı
typedef struct s_shell
{
	t_env	*env_list;		// environment değişkenleri ($HOME, $USER...)
	t_cmd	*cmd_list;		// parse edilmiş komut listesi
	char	*line;			// kullanıcının girdiği raw input
	int		exit_status;	// son komutun exit code'u ($?)
	int		exit_flag;		// shell kapansın mı? (exit komutu)
}	t_shell;




//--------------------------------UTILS--------------------------------
int		ft_is_space(char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_strlen(const char *s);
char	*ft_strdup(const char *s);
void	*ft_malloc(size_t size);
int		ft_isalpha(int c);
char	*ft_itoa(int n);
int		ft_isalnum(int c);
char	*ft_strjoin(char const *s1, char const *s2);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin_free(char *s1, char *s2);
char	*ft_strchr(const char *s, int c);
int		ft_is_digit(char c);
int		ft_atoi(const char *str);
char	**ft_split(char const *s, char c);

//--------------------------------FREE--------------------------------
void	ft_free_tokens(t_token *tokens);
void	ft_free_commands(t_cmd *commands);
void	ft_free_shell(t_shell *shell);
void	ft_free_split(char **split);

//--------------------------------LEXER--------------------------------
t_token_type	ft_get_operator_type(char *line, int *i);
char			*ft_get_word(char *line, int *i);
t_token			*ft_tokenize(char *line);
t_token			*ft_create_token(t_token_type type, char *value);
void			ft_add_token(t_token **token_list, t_token *new_token);

//--------------------------------VALIDATOR--------------------------------
int	ft_validate_syntax(char *line);
int	ft_validate_quotes(char *line);
int	ft_validate_pipes(char *line);
int	ft_validate_redirections(char *line);

//--------------------------------PARSER--------------------------------
t_cmd		*ft_create_command(void);
//t_token	*ft_create_cmd_arg(char *value, t_token_type type); //ft_create_token_arg
//void		ft_add_cmd_arg(t_token **args, t_token *new_arg);	//ft_add_token_to_args
void		ft_get_cmd_arguments(t_token **current, t_cmd *cmd);
int			ft_parse_redirections(t_cmd *cmd, t_token **current);
t_cmd		*ft_parse_command(t_token **current);
void		ft_add_command(t_cmd **commands, t_cmd *new_cmd);
t_cmd		*ft_parse_tokens(t_token *tokens);
int			ft_is_redirection(t_token_type type);
int			ft_is_argument_token(t_token_type type);
int			ft_count_args(t_token *tokens);
int			ft_in_parser_handle_redirect_in(t_cmd *cmd, t_token **current);
int			ft_in_parser_handle_redirect_out(t_cmd *cmd, t_token **current);
int			ft_in_parser_handle_redirect_append(t_cmd *cmd, t_token **current);
int			ft_in_parser_handle_heredoc(t_cmd *cmd, t_token **current);

//--------------------------------EXPANDER--------------------------------
char	**ft_expand_tokens(t_token *args, t_shell *shell);

//--------------------------------ENVIRONMENT--------------------------------
t_env	*ft_create_env_node(char *key, char *value);
void	ft_add_env_node(t_env **env_list, t_env *new_node);
char 	*ft_parse_env_value(char *env_str);
char 	*ft_parse_env_key(char *env_str);
int		ft_parsing_env_entry(char *env_str, t_env **env_list);
t_env	*ft_init_env(char **envp);
char	*ft_get_env_value(char *key, t_env *env_list);
int		ft_set_env_value(char *key, char *value, t_env **env_list);
int		ft_unset_env_value(char *key, t_env **env_list);
char	**ft_env_to_array(t_env *env_list);

//--------------------------------BUILTIN--------------------------------
int	ft_execute_builtin(t_shell *shell, t_cmd *cmd, int in_pipe);
int	ft_is_builtin(char *cmd);
int	ft_builtin_cd(char **argv, t_env *env_list);
int	ft_builtin_echo(char **argv);
int	ft_builtin_env(t_env *env_list);
int	ft_builtin_exit(char **argv, t_shell *shell);
int	ft_builtin_export(char **argv, t_env **env_list);
int	ft_builtin_pwd(void);
int	ft_builtin_unset(char **argv, t_env **env_list);

//--------------------------------EXECUTOR--------------------------------
int		ft_execute_child_process(t_shell *shell, t_cmd *cmd, int *pipefd, int prev_fd);
int		ft_execute_commands(t_shell *shell);
int		ft_execute_multiple_command(t_shell *shell);
int		ft_execute_single_command(t_shell *shell, t_cmd *cmd);
char	*ft_find_executable(char *cmd, t_env *env_list);
int		ft_handle_redirections(t_cmd *cmd);

//--------------------------------REDİRECTIONS--------------------------------
void	ft_handle_heredoc(t_shell *shell);
int		ft_handle_redirections(t_cmd *cmd);

#endif