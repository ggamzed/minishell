#include "../minishell.h"

static int	ft_check_pipe_boundaries(char *line)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(line) - 1;
	while (line[i] && ft_is_space(line[i]))
		i++;
	while (j >= 0 && ft_is_space(line[j]))
		j--;
	if (line[i] == '|' || (j >= 0 && line[j] == '|'))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}

static int	ft_check_double_pipes(char *line)
{
	int	i;
	int	is_quote;

	i = 0;
	is_quote = 0;
	while (line[i])
	{
		if ((line[i] == '"') || (line[i] == '\''))
			is_quote++;
		if (line[i] == '|')
		{
			i++;
			while (line[i] && ft_is_space(line[i]))
				i++;
			if (line[i] == '|')
			{
				if (is_quote)
					return (0);
				ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
				return (1);
			}
		}
		else
			i++;
	}
	return (0);
}

int	ft_validate_pipes(char *line)
{
	if (ft_check_pipe_boundaries(line) == 1)
		return (1);
	if (ft_check_double_pipes(line) == 1)
		return (1);
	return (0);
}
