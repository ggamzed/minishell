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

int	ft_count_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != PIPE)
	{
		if (ft_is_argument_token(current->type))
		{
			count++;
			current = current->next;
		}
		else if (ft_is_redirection(current->type)) //redirectionları atla
		{
			current = current->next;
			// QUOTED FILENAME'LERİ DE ATLA!
			if (current && (current->type == WORD || 
			               current->type == SINGLE_QUOTED_STRING || 
			               current->type == DOUBLE_QUOTED_STRING))
				current = current->next;
		}
		else
		{
			current = current->next; // Bilinmeyen token'ları atla
		}
	}
	return (count);
}
