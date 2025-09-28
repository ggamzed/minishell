/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_heredoc.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:07 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:21:08 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>

static int	ft_check_heredoc_conditions(char *line, char *delimiter,
									int *pipefd)
{
	if (g_signal == SIGINT)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		free(line);
		return (-1);
	}
	if (!line)
	{
		ft_putstr_fd("minishell: warning: here-document at line 1 ", 2);
		ft_putstr_fd("delimited by end-of-file (wanted `", 2);
		ft_putstr_fd(delimiter, 2);
		ft_putstr_fd("')\n", 2);
		return (1);
	}
	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

static int	ft_process_and_write_line(char *line, int should_expand,
									t_shell *shell, int *pipefd)
{
	char	*expanded_line;

	if (should_expand)
		expanded_line = ft_expand_double_quoted(line, shell);
	else
		expanded_line = ft_strdup(line, shell);
	if (!expanded_line)
	{
		free(line);
		close(pipefd[1]);
		close(pipefd[0]);
		return (0);
	}
	write(pipefd[1], expanded_line, ft_strlen(expanded_line));
	write(pipefd[1], "\n", 1);
	return (1);
}

int	ft_process_heredoc(char *delimiter, t_shell *shell, int should_expand)
{
	char	*line;
	int		pipefd[2];
	int		check_result;

	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		check_result = ft_check_heredoc_conditions(line, delimiter, pipefd);
		if (check_result == -1)
			return (-1);
		if (check_result == 1)
			break ;
		if (!ft_process_and_write_line(line, should_expand, shell, pipefd))
			return (-1);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
