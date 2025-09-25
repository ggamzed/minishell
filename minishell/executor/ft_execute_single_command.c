#include "../minishell.h"

static int	ft_handle_builtin_redirections(t_cmd *cmd, int *stdin_backup, int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	if (ft_handle_redirections(cmd) != 0)
	{
		close(*stdin_backup);
		close(*stdout_backup);
		return (1);
	}
	return (0);
}

static void	ft_restore_redirections(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

static int	ft_execute_builtin_in_parent(t_shell *shell, t_cmd *cmd)
{
	int	original_stdin;
	int	original_stdout;
	int	result;

	if (ft_handle_builtin_redirections(cmd, &original_stdin, &original_stdout) != 0)
		return (1);
	result = ft_execute_builtin(shell, cmd, 0);
	ft_restore_redirections(original_stdin, original_stdout);
	return (result);
}

static int	ft_execute_external_command(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;
	int		status;
	int		sig;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		ft_execute_child_process(shell, cmd, NULL, -1);
		ft_free_mem_tracker(shell->mem_tracker);
		free(shell);
		exit(1);
	}
	ft_ignore_signals();
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
    	return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", 2);
		return (128 + sig);
	}
	return (0);
}

int	ft_execute_single_command(t_shell *shell, t_cmd *cmd)
{
	if (ft_is_builtin(cmd->expanded_argv[0]))
		return (ft_execute_builtin_in_parent(shell, cmd));
	else
		return (ft_execute_external_command(shell, cmd));
}
