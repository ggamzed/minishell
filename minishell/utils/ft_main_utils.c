#include "../minishell.h"

void	ft_init_export_list(t_shell *shell)
{
	t_env	*curr;

	curr = shell->env_list;
	while (curr)
	{
		ft_set_env_value(curr->key, curr->value, &shell->export_list, shell);
		curr = curr->next;
	}
}

int	ft_validate_and_parse(t_shell *shell, char *line)
{
	t_token	*tokens;

	shell->line = line;
	if (ft_validate_syntax(line) == 1)
	{
		shell->exit_status = 2;
		return (1);
	}
	tokens = ft_tokenize(line, shell);
	if (!tokens)
		return (1);
	shell->cmd_list = ft_parse_tokens(tokens, shell);
	if (!shell->cmd_list)
		return (0);
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		return (130);
	}
	return (-1);
}

static int	ft_handle_expansion_error(t_shell *shell, t_cmd *current)
{
	if (current->args && current->args->type == VARIABLE)
		shell->exit_status = 0;
	else
	{
		ft_print_error_msg("", ": command not found\n");
		shell->exit_status = 127;
	}
	ft_free_fds(shell->cmd_list);
	shell->cmd_list = NULL;
	return (1);
}

int	ft_expand_argv(t_shell *shell)
{
	t_cmd	*current;

	current = shell->cmd_list;
	while (current)
	{
		if (current->args)
		{
			current->expanded_argv = ft_expand_tokens(current->args, shell);
			if (!current->expanded_argv || !current->expanded_argv[0]
				|| ft_strlen(current->expanded_argv[0]) == 0)
			{
				return (ft_handle_expansion_error(shell, current));
			}
		}
		current = current->next;
	}
	return (0);
}
