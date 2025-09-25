#include "../minishell.h"

static int	ft_handle_builtin_redirections(t_cmd *cmd, int *stdin_backup,
					int *stdout_backup)
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

static int	ft_check_only_child_status(int status)
{
	int	sig;

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
