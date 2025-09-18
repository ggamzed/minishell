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

static int	ft_count_merged_args(t_token *tokens)
{
	t_token	*current;
	int		count;

	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		while (current->next && current->space_flag == 0)
			current = current->next;
		current = current->next;
	}
	return (count);
}

void	ft_join_expand_tokens(char ***joined_argv, char **expanded_argv, 
							t_token *original_tokens, t_shell *shell)
{
	char	**merged_argv;
	t_token	*current;
	int		i;
	int		j;

	merged_argv = ft_malloc(sizeof(char *) * 
		(ft_count_merged_args(original_tokens) + 1), shell);
	if (!merged_argv)
	{
		*joined_argv = NULL;
		return;
	}
	current = original_tokens;
	i = 0;
	j = 0;
	while (current)
	{
		merged_argv[j] = ft_strdup(expanded_argv[i], shell);
		while (current->next && current->space_flag == 0)
		{
			current = current->next;
			i++;
			merged_argv[j] = ft_strjoin_free(merged_argv[j], expanded_argv[i], shell);
		}
		j++;
		i++;
		current = current->next;
	}
	merged_argv[j] = NULL;
	*joined_argv = merged_argv;
}

char	**ft_expand_tokens(t_token *args, t_shell *shell)
{
	char		**argv;
	char		**join_argv;
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
	ft_join_expand_tokens(&join_argv, argv, args, shell);
	i = 0;
	// while (argv[i])
	// {
	// 	free(argv[i]);
	// 	i++;
	// }
	// free(argv);
	return (join_argv);
}

