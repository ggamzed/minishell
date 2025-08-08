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
# Source files
SRCS = main.c \
       utils.c \
	   free.c \
       $(lexer)ft_tokenize.c \
       $(lexer)ft_token.c \
       $(lexer)ft_validate_syntax.c \
       $(lexer)ft_validate_quotes.c \
       $(lexer)ft_validate_pipes.c \
       $(lexer)ft_validate_redirections.c \
       $(lexer)ft_count_args.c \
       $(parser)ft_parse_tokens.c \
	   $(parser)ft_handle_redirections.c \
	   $(parser)ft_parse_command.c \
	   $(env)ft_env_node.c \
	   $(env)ft_env_utils.c \
	   $(env)ft_init_env.c \
	   $(env)ft_parse_env.c \
	   $(expander)expander.c \
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
	   $(executor)setup_redirection.c #değişecek 

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