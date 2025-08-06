#include "../minishell.h"

static void	ft_skip_redirection_operator(char *line, int *i)
{
	if (line[*i] == '<')
	{
		(*i)++;
		if (line[*i] == '<')
			(*i)++;
	}
	else if (line[*i] == '>')
	{
		(*i)++;
		if (line[*i] == '>')
			(*i)++;
	}
}

static int	ft_check_filename_after_redirection(char *line, int i)
{
	while (line[i] && ft_is_whitespace(line[i]))
		i++;
	if (!line[i] || line[i] == '|' || line[i] == '<' || line[i] == '>')
	{
		printf("minishell: syntax error near unexpected token"); //daha düzgün bir hata mesajı yazdır
		//printf(" `newline'\n"); 
		return (0);
	}
	return (1);
}

int	ft_validate_redirections(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '<' || line[i] == '>')
		{
			ft_skip_redirection_operator(line, &i);
			if (!ft_check_filename_after_redirection(line, i))
				return (0);
		}
		else
			i++;
	}
	return (1);
}
