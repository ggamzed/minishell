#include "../minishell.h"

int	ft_is_redirection(t_token_type type)
{
	if (type == REDIRECT_IN || type == REDIRECT_OUT
		|| type == REDIRECT_APPEND || type == HEREDOC)
		return (1);
	return (0);
}

int	ft_is_argument_token(t_token_type type)
{
	if (type == WORD || type == SINGLE_QUOTED_STRING
		|| type == DOUBLE_QUOTED_STRING || type == VARIABLE
		|| type == EXIT_STATUS)
		return (1);
	return (0);
}
