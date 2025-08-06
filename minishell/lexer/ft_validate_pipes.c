#include "../minishell.h"

static int	ft_check_pipe_boundaries(char *line)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(line) - 1;
	while (line[i] && ft_is_whitespace(line[i]))
		i++;
	while (j >= 0 && ft_is_whitespace(line[j]))
		j--;
	if (line[i] == '|' || (j >= 0 && line[j] == '|'))
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (0);
	}
	return (1);
}

static int	ft_check_double_pipes(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '|')
		{
			i++;
			while (line[i] && ft_is_whitespace(line[i]))
				i++;
			if (line[i] == '|')
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				return (0);
			}
		}
		else
			i++;
	}
	return (1);
}

int	ft_validate_pipes(char *line)
{
	if (!ft_check_pipe_boundaries(line))
		return (0);
	if (!ft_check_double_pipes(line))
		return (0);
	return (1);
}
