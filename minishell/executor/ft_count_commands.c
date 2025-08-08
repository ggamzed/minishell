#include "../minishell.h"

int	ft_count_commands(t_cmd *commands)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = commands;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}
