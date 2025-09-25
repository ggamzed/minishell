#include "../minishell.h"

static int	ft_create_pipe(t_cmd *cmd, int pipefd[2])
{
	if (cmd->next)
	{
		if (pipe(pipefd) == -1)
		{
			perror("minishell: pipe");
			return (1);
		}
	}
	return (0);
}

static pid_t	ft_create_child_and_execute(t_shell *shell, t_cmd *cmd,
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

static int	ft_wait_all_children(pid_t last_pid)
{
	int status;
	int last_status = 0;
	int sig;
	int quit_printed = 0;
	pid_t waited_pid;
	
	while ((waited_pid = wait(&status)) > 0)
	{
		if (WIFEXITED(status))
		{
			if (last_pid == -1 || waited_pid == last_pid)
				last_status = WEXITSTATUS(status);
		}
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			
			if (sig == SIGPIPE)
			{
				if (last_pid == -1 || waited_pid == last_pid)
				{
					continue;
				}
				continue;
			}
			if (sig == SIGQUIT && !quit_printed)
			{
				ft_putstr_fd("Quit (core dumped)\n", 2);
				quit_printed = 1;
			}
			if (last_pid == -1 || waited_pid == last_pid)
				last_status = 128 + sig;
		}
	}
	return (last_status);
}

int	ft_execute_multiple_command(t_shell *shell)
{
	t_cmd	*current;
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	pid_t	last_pid = -1;

	current = shell->cmd_list;
	prev_fd = -1;
	pipefd[0] = -1;
	pipefd[1] = -1;
	
	while (current)
	{
		if (ft_create_pipe(current, pipefd))
			return (1);
		pid = ft_create_child_and_execute(shell, current, pipefd, prev_fd);
		if (pid == -1)
			return (1);
		if (!current->next)
			last_pid = pid;
		if (prev_fd != -1)
			close(prev_fd);
		if (current->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		current = current->next;
	}
	
	return (ft_wait_all_children(last_pid));
}
