#include "../minishell.h"

char	*ft_handle_exit_status(t_shell *shell)
{
	return (ft_itoa(shell->exit_status, shell));
}


static char	*ft_expand_token_value(char *value, t_token_type type, t_shell *shell, int is_heredoc_delimiter)
{
	int	i;

	i = 0;
	if (!value)
		return (NULL);
	
	// Heredoc delimiter ise hiçbir zaman genişletme
	if (is_heredoc_delimiter)
		return (ft_strdup(value, shell));
	if (type == VARIABLE)
		return (ft_extract_and_expand_var(value, &i, shell));
	else if (type == EXIT_STATUS)
		return (ft_handle_exit_status(shell));
	else if (type == DOUBLE_QUOTED_STRING)
		return (ft_expand_double_quoted(value, shell));
	else if (type == SINGLE_QUOTED_STRING)
		return (ft_strdup(value, shell));
	else if (type == WORD)
		return (ft_strdup(value, shell));
	else
		return (ft_strdup(value, shell));
}

static int	count_args(t_token *args)
{
	t_token *current;
	int	count;

	count = 0;
	current = args;
	while (current)
	{
		current = current->next;
		count++;
	}
	return (count);
}

char	**ft_expand_tokens(t_token *args, t_shell *shell)
{
	char		**argv;
	t_token		*current;
	int			i;
	
	// Allocate argv array
	argv = ft_malloc(sizeof(char *) * (count_args(args) + 1), shell);
	if (!argv)
		return (NULL);
	// Expand each argument
	current = args;
	i = 0;
	while (current)
	{
		argv[i++] = ft_expand_token_value(current->value, current->type, shell, 0);
		
		current = current->next;
	}
	argv[i] = NULL;
	return (argv);
}

// Backward compatibility için eski fonksiyon
char	*ft_expand_variables(char *str, t_shell *shell)
{
	return (ft_expand_double_quoted(str, shell));
}
