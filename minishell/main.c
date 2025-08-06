#include "minishell.h"

static t_shell	*ft_init_shell(char **envp)
{
	t_shell	*shell;

	shell = ft_malloc(sizeof(t_shell));
	shell->env_list = ft_init_env(envp);
	shell->cmd_list = NULL;
	shell->line = NULL;
	shell->exit_status = 0;
	shell->exit_flag = 0;
	return (shell);
}

static void	ft_process_line(t_shell *shell, char *line)
{
	t_token	*tokens;
	t_cmd	*current_cmd;

	shell->line = line;
	
	// Input validation
	if (!ft_validate_syntax(line))
	{
		shell->exit_status = 2; // Syntax error
		return;
	}
	
	// Tokenization
	tokens = ft_tokenize(line);
	if (!tokens)
		return;
	
	// Parsing
	shell->cmd_list = ft_parse_tokens(tokens);
	//free_tokens(tokens); // TODO: Implement when ready
	
	if (!shell->cmd_list)
		return;
	
	// Execute each command in the command list
	current_cmd = shell->cmd_list;
	while (current_cmd && !shell->exit_flag)
	{
		// Check if it's a builtin command
		if (current_cmd->args && ft_is_builtin(current_cmd->args->value))
		{
			shell->exit_status = ft_execute_builtin(shell, current_cmd, 0);
		}
		else
		{
			// TODO: Execute external commands when ready
			printf("External command execution not implemented yet: %s\n", 
				current_cmd->args ? current_cmd->args->value : "NULL");
			shell->exit_status = 0;
		}
		current_cmd = current_cmd->next;
	}
	
	//free_commands(shell->cmd_list); // TODO: Implement when ready
	shell->cmd_list = NULL;
}

void	ft_shell_loop(t_shell *shell)
{
	char	*line;
	
	while (!shell->exit_flag)
	{
		//setup_signals(); // TODO: Implement signals when ready
		
		line = readline(PROMPT);
		if (!line) // EOF (Ctrl+D)
		{
			printf("exit\n");
			break;
		}
		
		if (*line) // Non-empty line
		{
			add_history(line);
			ft_process_line(shell, line);
		}
		
		free(line);
		
		// TODO: Signal handling
		// if (g_signal == SIGINT)
		// {
		// 	shell->exit_status = 130;
		// 	g_signal = 0;
		// }
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;

	(void)argc;
	(void)argv;
	
	shell = ft_init_shell(envp);
	if (!shell)
	{
		fprintf(stderr, "Error: Failed to initialize shell\n");
		return (1);
	}
	
	ft_shell_loop(shell);
	
	//free_shell(shell); // TODO: Implement when ready
	rl_clear_history();
	
	return (shell->exit_status);
}
