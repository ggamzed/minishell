#include "../minishell.h"

int	ft_validate_syntax(char *line)
{
	if (ft_validate_quotes(line) == 1)
		return (1);
	if (ft_validate_pipes(line) == 1)
		return (1);
	if (ft_validate_redirections(line) == 1)
		return (1);
	return (0);
}
