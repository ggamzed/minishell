NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g 
LDFLAGS = -lreadline

lexer = lexer/
parser = parser/
expander = expander/
env = env/
builtins = builtins/
executor = executor/
redirections = redirections/
validate_syntax = validate_syntax/
utils = utils/
SRCS = main.c \
		signals.c \
		$(lexer)ft_get_type.c \
		$(lexer)ft_get_word.c \
		$(lexer)ft_tokenize.c \
		$(lexer)ft_token.c \
		$(validate_syntax)ft_validate_syntax.c \
		$(validate_syntax)ft_validate_quotes.c \
		$(validate_syntax)ft_validate_pipes.c \
		$(validate_syntax)ft_validate_redirections.c \
		$(parser)ft_in_parser_handle_redirect_input.c \
		$(parser)ft_in_parser_handle_redirect_output.c \
		$(parser)ft_check.c \
		$(parser)ft_join_redirect_filename.c \
		$(parser)ft_parse_tokens.c \
		$(parser)ft_parse_command.c \
		$(env)ft_env_node.c \
		$(env)ft_env_utils.c \
		$(env)ft_init_env.c \
		$(env)ft_parse_env.c \
		$(expander)expander.c \
		$(expander)expand_double_quoted.c \
		$(expander)extract_and_expand.c \
		$(expander)ft_expand_tilde.c \
		$(expander)ft_expand_var.c \
		$(expander)ft_join_tokens.c \
		$(expander)ft_word_split_utils.c \
		$(expander)ft_word_split.c \
		$(builtins)ft_builtin_cd.c \
		$(builtins)ft_builtin_echo.c \
		$(builtins)ft_builtin_env.c \
		$(builtins)ft_builtin_exit.c \
		$(builtins)ft_builtin_export.c \
		$(builtins)ft_builtin_export_set_variable.c \
		$(builtins)ft_builtin_pwd.c \
		$(builtins)ft_builtin_unset.c \
		$(builtins)ft_execute_builtin.c \
		$(executor)ft_execute_child_process.c \
		$(executor)ft_execute_commands.c \
		$(executor)ft_execute_multiple_command.c \
		$(executor)ft_execute_single_command.c \
		$(executor)ft_find_executable.c \
		$(executor)ft_child_utils.c \
		$(executor)ft_multiple_command_utils.c \
		$(executor)ft_single_command_utils.c \
		$(redirections)ft_handle_heredoc_redirection.c \
		$(redirections)ft_handle_input_redirection.c \
		$(redirections)ft_handle_output_redirection.c \
		$(redirections)ft_process_heredoc.c \
		$(utils)ft_check_utils.c \
		$(utils)ft_convert_utils.c \
		$(utils)ft_exit_utils.c \
		$(utils)ft_main_utils.c \
		$(utils)ft_split.c \
		$(utils)ft_str_utils.c \
		$(utils)ft_str_utils2.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re