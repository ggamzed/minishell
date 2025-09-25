#include "../minishell.h"
#include <sys/stat.h>

static void	ft_setup_pipe_connections(int *pipefd, int prev_fd, t_cmd *cmd)
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

static void	ft_execute_builtin_in_child(t_shell *shell, t_cmd *cmd)
{
   int	exit_code;

   exit_code = ft_execute_builtin(shell, cmd, 1);
   ft_free_mem_tracker(shell->mem_tracker);
   free(shell);
   exit(exit_code);
}

static void ft_update_shlvl(t_shell *shell)
{
    char *current_shlvl;
    int shlvl_value;
    char *new_shlvl;

    current_shlvl = ft_get_env_value("SHLVL", shell->env_list);
    if (current_shlvl)
    {
        shlvl_value = ft_atoi(current_shlvl);
        shlvl_value++;
        new_shlvl = ft_itoa(shlvl_value, shell);
        ft_set_env_value("SHLVL", new_shlvl, &shell->env_list, shell);
        free(new_shlvl);
    }
    else
        ft_set_env_value("SHLVL", "1", &shell->env_list, shell);
}

static void	ft_exec_error_msg(t_shell *shell, char *cmd)
{
	struct stat st;

    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, F_OK) == 0)
        {
            if (stat(cmd, &st) == 0 && (st.st_mode & S_IFMT) == S_IFDIR)
            {
                ft_putstr_fd("minishell: ", 2);
                ft_putstr_fd(cmd, 2);
                ft_putstr_fd(": is a directory\n", 2);
                ft_free_mem_tracker(shell->mem_tracker);
                free(shell);
                exit(126);
            }
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd, 2);
            ft_putstr_fd(": Permission denied\n", 2);
            ft_free_mem_tracker(shell->mem_tracker);
            free(shell);
            exit(126);
        }
    }
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(cmd, 2);
    ft_putstr_fd(": command not found\n", 2);
    ft_free_mem_tracker(shell->mem_tracker);
    free(shell);
    exit(127);
}

static void	ft_execute_external_in_child(t_shell *shell, t_cmd *cmd)
{
	char	*executable;
	char	**envp;

	executable = ft_find_executable(cmd->expanded_argv[0], shell->env_list, shell);
	if (!executable)
        ft_exec_error_msg(shell, cmd->expanded_argv[0]);
	ft_update_shlvl(shell);
	envp = ft_env_to_array(shell->env_list, shell);
	execve(executable, cmd->expanded_argv, envp);
	perror("execve");
	ft_free_mem_tracker(shell->mem_tracker);
	free(shell);
	exit(126);
}

int	ft_execute_child_process(t_shell *shell, t_cmd *cmd, int *pipefd, int prev_fd)
{
	ft_default_signals();
	ft_setup_pipe_connections(pipefd, prev_fd, cmd);
	if (ft_handle_redirections(cmd) != 0)
	{
		ft_free_mem_tracker(shell->mem_tracker);
		free(shell);
		exit(1);
	}
	if (!cmd->expanded_argv || !cmd->expanded_argv[0])
	{
		ft_free_mem_tracker(shell->mem_tracker);
		free(shell);
		exit(1);
	}
	if (ft_is_builtin(cmd->expanded_argv[0]))
		ft_execute_builtin_in_child(shell, cmd);
	else
		ft_execute_external_in_child(shell, cmd);
	return (0);
}

int ft_handle_redirections(t_cmd *cmd)
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
