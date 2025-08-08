#include "../minishell.h"

int	ft_execute_single_command(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if (cmd->heredoc_delimiter)
	{
		if (handle_heredoc(cmd) != 0)
			return (1);
	}
	if (ft_is_builtin(cmd->args->value))
		return (ft_execute_builtin(shell, cmd, 0));
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	
	if (pid == 0)
		execute_child_process(shell, cmd, NULL, -1);
	
	ignore_signals();
	waitpid(pid, &status, 0);
	
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	
	return (0);
}

int	ft_execute_multiple_commands(t_shell *shell)
{
	t_cmd	*current;
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	int		status;
	int		last_status;

	current = shell->cmd_list;
	prev_fd = -1;
	last_status = 0;
	
	while (current)
	{
		if (current->next && pipe(pipefd) == -1)
		{
			perror("pipe");
			return (1);
		}
		
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}
		
		if (pid == 0)
			execute_child_process(shell, current, pipefd, prev_fd);
		
		if (prev_fd != -1)
			close(prev_fd);
		
		if (current->next)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		
		current = current->next;
	}
	
	ignore_signals();
	
	while (wait(&status) > 0)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
	}
	
	return (last_status);
}

int	execute_commands(t_shell *shell)
{
	int	cmd_count;

	if (!shell->cmd_list || !shell->cmd_list->args) //|| !shell->cmd_list->args[0])
		return (0);
	cmd_count = ft_count_commands(shell->cmd_list);
	if (cmd_count == 1)
		return (ft_execute_single_command(shell, shell->cmd_list));
	else
		return (ft_execute_multiple_commands(shell));
}

