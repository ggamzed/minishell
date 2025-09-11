#include "minishell.h"

void	ft_free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		// value'yu ve current'ı free etme - memory tracker halleder
		current = next;
	}
}
// void	ft_free_tokens(t_token *tokens)
// {
// 	t_token	*current;
// 	t_token	*next;

// 	current = tokens;
// 	while (current)
// 	{
// 		next = current->next;
// 		free(current->value);
// 		free(current);
// 		current = next;
// 	}
// }

void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	ft_free_commands(t_cmd *commands)
{
	t_cmd	*current;
	t_cmd	*next;

	current = commands;
	while (current)
	{
		next = current->next;
		
		// args ve expanded_argv'yi burada free etme - memory tracker halleder
		if (current->args)
			ft_free_tokens(current->args);
		// expanded_argv'yi manuel free etme! Memory tracker'da kayıtlı
		
		// input_file, output_file, heredoc_delimiter'ları da free etme
		// Bunlar da memory tracker ile ayrılmış
		
		// Heredoc fd'yi kapat (bu önemli - FD leak'i önler)
		if (current->heredoc_fd != -1)
		{
			close(current->heredoc_fd);
			current->heredoc_fd = -1;
		}
			
		// current'ı da free etme - memory tracker halleder
		current = next;
	}
}
// void	ft_free_commands(t_cmd *commands)
// {
// 	t_cmd	*current;
// 	t_cmd	*next;

// 	current = commands;
// 	while (current)
// 	{
// 		next = current->next;
		
// 		// args ve expanded_argv'yi burada free et
// 		if (current->args)
// 			ft_free_tokens(current->args);
// 		if (current->expanded_argv)
// 			ft_free_split(current->expanded_argv);
			
// 		free(current->input_file);
// 		free(current->output_file);
// 		free(current->heredoc_delimiter);
		
// 		// Heredoc fd'yi kapat
// 		if (current->heredoc_fd != -1)
// 			close(current->heredoc_fd);
			
// 		free(current);
// 		current = next;
// 	}
// 	// Son kısımdaki kod kaldırıldı (zaten NULL'a erişiyordu)
// }

void	ft_free_shell(t_shell *shell)
{
	t_env	*env_current;
	t_env	*env_next;

	if (!shell)
		return;
	
	env_current = shell->env_list;
	while (env_current)
	{
		env_next = env_current->next;
		free(env_current->key);
		free(env_current->value);
		free(env_current);
		env_current = env_next;
	}
	
	ft_free_commands(shell->cmd_list);
	free(shell);
}
