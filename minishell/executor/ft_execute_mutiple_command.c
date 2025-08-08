#include "../minishell.h"

static int	ft_create_pipe_if_needed(t_cmd *cmd, int pipefd[2])
{
	if (cmd->next && pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	return (0);
}

static pid_t	ft_create_child_and_exec(t_shell *shell, t_cmd *cmd,
									int pipefd[2], int prev_fd)
{
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (-1);
	}
	if (pid == 0)
		ft_execute_child_process(shell, cmd, pipefd, prev_fd);
	return (pid);
}

static int	ft_wait_all_children(void)
{
	int status;
	int last_status = 0;

	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
	}
	return (last_status);
}

int	ft_execute_multiple_command(t_shell *shell)
{
	t_cmd	*current;
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;

	current = shell->cmd_list;
	prev_fd = -1;
	pipefd[2] = -1;
	while (current)
	{
		if (ft_create_pipe_if_needed(current, pipefd))
			return (1);
		pid = ft_create_child_and_exec(shell, current, pipefd, prev_fd);
		if (pid == -1)
			return (1);
		if (prev_fd != -1)
			close(prev_fd);
		if (current->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		current = current->next;
	}
	return (ft_wait_all_children());
}

