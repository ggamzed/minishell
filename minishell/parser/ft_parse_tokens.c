#include "../minishell.h"

// komut listesinin sonuna yeni komut ekler
void	ft_add_command(t_cmd **commands, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!*commands)
	{
		*commands = new_cmd;
		return;
	}
	current = *commands;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

// pipe'larla ayrılmış komutları ayrı t_cmd'lere dönüştürür
t_cmd	*ft_parse_tokens(t_token *tokens)
{
	t_cmd	*commands;  //komut listesi
	t_cmd	*cmd; //tek bir komut
	t_token	*current;

	commands = NULL;
	current = tokens;
	while (current)
	{
		cmd = ft_parse_command(&current);
		ft_add_command(&commands, cmd);
		if (current && current->type == PIPE)
			current = current->next;
	}
	return (commands);
}
