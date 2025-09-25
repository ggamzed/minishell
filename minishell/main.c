#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

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

static t_shell	*ft_init_shell(char **envp, t_mem **mem_tracker)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->mem_tracker = mem_tracker;
	shell->env_list = ft_init_env(envp, shell);
	if (!shell->env_list)
	{
		free(shell);
		return (NULL);
	}
	shell->cmd_list = NULL;
	shell->line = NULL;
	shell->export_list = NULL;
	ft_init_export_list(shell);
	shell->exit_status = 0;
	shell->exit_flag = 0;
	return (shell);
}

static int	ft_process_line(t_shell *shell, char *line)
{
	t_token	*tokens;
	t_cmd *current;

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
	current = shell->cmd_list;
	while (current)
	{
		if (current->args)
		{
			current->expanded_argv = ft_expand_tokens(current->args, shell);
			if (!current->expanded_argv || !current->expanded_argv[0] || 
							ft_strlen(current->expanded_argv[0]) == 0)
			{
				if (current->args && current->args->type == VARIABLE)
				{
					shell->exit_status = 0;
				}
				else
				{
					ft_print_error_msg("", ": command not found\n");
					shell->exit_status = 127;
				}
				ft_free_fds(shell->cmd_list);
				shell->cmd_list = NULL;
				return (1);
			}
		}
		current = current->next;
	}
	shell->exit_status = ft_execute_commands(shell);
	ft_free_fds(shell->cmd_list);
	shell->cmd_list = NULL;
	return (1);
}
int my_rl_hook(void)
{
    if (g_signal == SIGINT)
    {
        //printf("minishell> ");
        //g_signal = 0;
    }
    return 0;
}

static void	ft_shell_loop(t_shell *shell)
{
	char	*line;
	
	while (!shell->exit_flag)
	{
		ft_setup_signals();
		rl_event_hook = my_rl_hook;
		if (g_signal == SIGINT)
		{

			shell->exit_status = 130;
			g_signal = 0;
		}
		line = readline(PROMPT);
		if (!line)
		{
			printf("exit\n");
			shell->exit_flag = 1;
			break;
		}
		if (*line)
		{
			add_history(line);
			if (ft_process_line(shell, line) == 0)
			{
				free(line);
				break ;
			}
		}
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	t_mem	*mem_tracker;
	int		exit_code;

	(void)argc;
	(void)argv;
	mem_tracker = NULL;
	shell = ft_init_shell(envp, &mem_tracker);
	if (!shell)
	{
		ft_putstr_fd("minishell: failed to initialize shell\n", 2);
		ft_cleanup_and_exit(shell, 1);
	}
	ft_shell_loop(shell);
	exit_code = shell->exit_status;
	ft_free_mem_tracker(&mem_tracker);
	free(shell);
	rl_clear_history();
	return (exit_code);
}
