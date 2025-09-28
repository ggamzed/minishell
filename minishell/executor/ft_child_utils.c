/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_child_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:19:29 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 12:42:44 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <unistd.h>
#include <sys/stat.h>

void	ft_setup_pipe_connections(int *pipefd, int prev_fd, t_cmd *cmd)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);
	}
	else if (pipefd)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

void	ft_update_shlvl(t_shell *shell)
{
	char	*current_shlvl;
	int		shlvl_value;
	char	*new_shlvl;

	current_shlvl = ft_get_env_value("SHLVL", shell->env_list);
	if (current_shlvl)
	{
		shlvl_value = ft_atoi(current_shlvl);
		shlvl_value++;
		new_shlvl = ft_itoa(shlvl_value, shell);
		ft_set_env_value("SHLVL", new_shlvl, &shell->env_list, shell);
	}
	else
		ft_set_env_value("SHLVL", "1", &shell->env_list, shell);
}

void	ft_exec_error(t_shell *shell, char *cmd)
{
	struct stat	st;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
		{
			if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
			{
				ft_print_error_msg(cmd, ": is a directory\n");
				ft_cleanup_and_exit(shell, 126);
			}
			ft_print_error_msg(cmd, ": Permission denied\n");
			ft_cleanup_and_exit(shell, 126);
		}
	}
	ft_print_error_msg(cmd, ": command not found\n");
	ft_cleanup_and_exit(shell, 127);
}

int	ft_handle_redirections(t_cmd *cmd)
{
	if (ft_handle_input_redirection(cmd) != 0)
		return (1);
	if (ft_handle_heredoc_redirection(cmd) != 0)
		return (1);
	if (ft_handle_output_redirection(cmd) != 0)
		return (1);
	if (cmd->heredoc_fd != -1)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	return (0);
}
