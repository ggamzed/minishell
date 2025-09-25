#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

void	ft_init_export_list(t_shell *shell)
{
	t_env	*curr;

	curr = shell->env_list;
	while (curr)
	{
		ft_set_env_value(curr->key, curr->value, &shell->export_list, shell);
		curr = curr->next;
	}
}

static t_shell	*ft_init_shell(char **envp, t_mem **mem_tracker)
{
	t_shell	*shell;
	//t_mem *tmp;

	shell = malloc(sizeof(t_shell)); // burada ft_malloc kullan kendi yazdığın ft_mallocun adını değiştir diğerlerinde onu kullan
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
	t_token	*tokens;

	shell->line = line;
	// Input validation
	if (ft_validate_syntax(line) == 1)
	{
		shell->exit_status = 2;
		//ft_putstr_fd("minishell: syntax error\n", 2);
		return (1);
	}
	
	// Tokenization
	tokens = ft_tokenize(line, shell);
	
	if (!tokens)
	{
		// if (ft_strchr(line, '"') || ft_strchr(line, '\''))
   		// {
        // 	ft_putstr_fd("minishell: : command not found\n", 2);
        // 	shell->exit_status = 127;
    	// }
		return (1);
	}
		
	
	// Parsing - token'ları command'lara çevir
	shell->cmd_list = ft_parse_tokens(tokens, shell);
	if (!shell->cmd_list)
		return (0);
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		return (130);
	}
	// Heredoc işleme burası da gereksiz gibi
	// if (ft_handle_heredoc(shell) == 0)
	// {
	// 	printf("TRAP1\n");
	// 	printf("g_signal: %d\n", g_signal);
	// 	ft_free_commands(shell->cmd_list);
	// 	shell->cmd_list = NULL;
	// 	return (1);
	// }
	// Expansion - tüm komutların argv'lerini hazırla (SADECE BURADA YAP)
	t_cmd *current = shell->cmd_list;
	while (current)
	{
		if (current->args) // Null check ekle
		{
			current->expanded_argv = ft_expand_tokens(current->args, shell);
			// ft_expand_tokens sonrası
			if (!current->expanded_argv || !current->expanded_argv[0] || 
		ft_strlen(current->expanded_argv[0]) == 0)
	{
		// Eğer orijinal token VARIABLE ise → undefined variable (exit 0)
		if (current->args && current->args->type == VARIABLE)
		{
			shell->exit_status = 0; // Undefined variable için 0
		}
		else
		{
			// Empty command için 127
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd("", 2);
			ft_putstr_fd(": command not found\n", 2);
			shell->exit_status = 127;
		}
		ft_free_commands(shell->cmd_list);
		shell->cmd_list = NULL;
		return (1);
	}
		}
		current = current->next;
	}
	// Execution - komutları çalıştır
	shell->exit_status = ft_execute_commands(shell);
	
	// Cleanup - commands'ı free et
	ft_free_commands(shell->cmd_list);
	shell->cmd_list = NULL;
	
	// tokens'ı da free et (memory leak'i önlemek için)
	// Ama dikkat: tokens zaten cmd'lerin içinde referans ediliyorsa
	// cmd_list free edildiğinde tokens da free edilmiş olur
	
	return (1);
}
int my_rl_hook(void)
{
    if (g_signal == SIGINT)
    {
        //printf("minishell> ");
        //g_signal = 0;
    }
    return 0;
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
		line = readline(PROMPT);
		if (!line)  // EOF (Ctrl+D) CTRL+D = NULL döner
		{
			printf("exit\n");
			shell->exit_flag = 1;
			break;
		}
		if (*line)  // Non-empty line
		{
			add_history(line);
			if (ft_process_line(shell, line) == 0)
			{
				free(line);
				break ;
			}
		}
		free(line);
	}
}

// static void	ft_shell_loop(t_shell *shell)
// {
// 	char	*line;
	
// 	while (!shell->exit_flag)
// 	{
// 		ft_setup_signals();
// 		rl_event_hook = my_rl_hook;
// 		if (g_signal == SIGINT)
// 		{
// 			shell->exit_status = 130;
// 			g_signal = 0;
// 		}
		
// 		// Terminal'den mi yoksa pipe/file'dan mı input alıyoruz kontrol et
// 		if (isatty(fileno(stdin)))
// 			line = readline(PROMPT);
// 		else
// 		{
// 			char *raw_line;
// 			raw_line = get_next_line(fileno(stdin));
// 			if (raw_line)
// 			{
// 				line = ft_strtrim(raw_line, "\n");
// 				free(raw_line);
// 			}
// 			else
// 				line = NULL;
// 		}
		
// 		if (!line)  // EOF (Ctrl+D) CTRL+D = NULL döner
// 		{
// 			//printf("exit\n");  // <-- BU PRINTF'İ COMMENT OUT ET
// 			shell->exit_flag = 1;
// 			break;
// 		}
// 		if (*line)  // Non-empty line
// 		{
// 			if (isatty(fileno(stdin)))  // Sadece interactive modda history'ye ekle
// 				add_history(line);
// 			if (ft_process_line(shell, line) == 0)
// 			{
// 				free(line);
// 				break ;
// 			}
// 		}
// 		free(line);
// 	}
// }

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	t_mem	*mem_tracker;
	int		exit_code;

	(void)argc;
	(void)argv;
	// Shell initialization
	mem_tracker = NULL;
	shell = ft_init_shell(envp, &mem_tracker);
	//printf("mem_tracker: %p\n", (void*)&mem_tracker);
	//printf("shell->mem_tracker: %p\n", (void*)shell->mem_tracker);
	if (!shell)
	{
		ft_putstr_fd("minishell: failed to initialize shell\n", 2);
		ft_free_mem_tracker(&mem_tracker);
		free(shell);
		return (1);
	}
	
	// Main shell loop
	ft_shell_loop(shell);
	
	// Cleanup and exit
	exit_code = shell->exit_status;
	ft_free_mem_tracker(&mem_tracker);
	free(shell);
	//ft_free_shell(shell);
	rl_clear_history();
	
	return (exit_code);
}
