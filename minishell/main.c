/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:52 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:21:52 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t	g_signal = 0;

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
	int	parse_result;

	parse_result = ft_validate_and_parse(shell, line);
	if (parse_result != 1)
		return (parse_result);
	if (ft_expand_argv(shell))
		return (1);
	shell->exit_status = ft_execute_commands(shell);
	shell->cmd_list = NULL;
	return (1);
}

static int	ft_handle_readline_input(t_shell *shell, char **line)
{
	*line = readline(PROMPT);
	if (!(*line))
	{
		printf("exit\n");
		shell->exit_flag = 1;
		return (1);
	}
	if (**line)
	{
		add_history(*line);
		if (ft_process_line(shell, *line) == -1)
		{
			free(*line);
			return (1);
		}
	}
	return (0);
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
		if (ft_handle_readline_input(shell, &line))
			break ;
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
		exit(1);
	}
	ft_shell_loop(shell);
	exit_code = shell->exit_status;
	ft_free_fds(shell->cmd_list);
	ft_free_mem_tracker(&mem_tracker);
	free(shell);
	rl_clear_history();
	return (exit_code);
}
