# Variables
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
get = get_next_line/
# Source files
SRCS = main.c \
		utils.c \
		free.c \
		signals.c \
		$(lexer)ft_tokenize.c \
		$(lexer)ft_token.c \
		$(lexer)ft_count_args.c \
		$(validate_syntax)ft_validate_syntax.c \
		$(validate_syntax)ft_validate_quotes.c \
		$(validate_syntax)ft_validate_pipes.c \
		$(validate_syntax)ft_validate_redirections.c \
		$(parser)ft_parse_tokens.c \
		$(parser)ft_in_parser_handle_redirections.c \
		$(parser)ft_parse_command.c \
		$(env)ft_env_node.c \
		$(env)ft_env_utils.c \
		$(env)ft_init_env.c \
		$(env)ft_parse_env.c \
		$(expander)expander.c \
		$(expander)expand_double_quoted.c \
		$(expander)extract_and_expand.c \
		$(builtins)ft_builtin_cd.c \
		$(builtins)ft_builtin_echo.c \
		$(builtins)ft_builtin_env.c \
		$(builtins)ft_builtin_exit.c \
		$(builtins)ft_builtin_export.c \
		$(builtins)ft_builtin_pwd.c \
		$(builtins)ft_builtin_unset.c \
		$(builtins)ft_execute_builtin.c \
		$(executor)ft_execute_child_process.c \
		$(executor)ft_execute_commands.c \
		$(executor)ft_execute_multiple_command.c \
		$(executor)ft_execute_single_command.c \
		$(executor)ft_find_executable.c \
		$(redirections)ft_handle_redirections.c \
		$(redirections)ft_handle_heredoc.c \
		$(get)get_next_line_bonus.c \
		$(get)get_next_line_utils_bonus.c


# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(NAME)

# Build executable
$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

# Compile source files
%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJS)

# Clean everything
fclean: clean
	rm -f $(NAME)

# Rebuild
re: fclean all

# Phony targets
.PHONY: all clean fclean re