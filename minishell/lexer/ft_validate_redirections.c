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
	while (line[i] && ft_is_space(line[i]))
		i++;
	if (!line[i] || line[i] == '|' || line[i] == '<' || line[i] == '>')
	{
		if (!line[i])
			printf("minishell: syntax error near unexpected token `newline'\n");
		else
			printf("minishell: syntax error near unexpected token `%c'\n", line[i]);
		return (0);
	}
	return (1);
}

static int	ft_check_consecutive_redirects(char *line, int i)
{
	// Consecutive redirects are not allowed
	if (i > 0 && (line[i - 1] == '<' || line[i - 1] == '>'))
	{
		if (line[i] == '<' || line[i] == '>')
		{
			printf("minishell: syntax error near unexpected token `%c'\n", line[i]);
			return (0);
		}
	}
	return (1);
}

static int	ft_check_redirect_at_end(char *line)
{
	int	i;
	
	i = ft_strlen(line) - 1;
	while (i >= 0 && ft_is_space(line[i]))
		i--;
	if (i >= 0 && (line[i] == '<' || line[i] == '>'))
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
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
			// Check for consecutive redirects
			if (!ft_check_consecutive_redirects(line, i))
				return (0);
				
			ft_skip_redirection_operator(line, &i);
			if (!ft_check_filename_after_redirection(line, i))
				return (0);
		}
		else
			i++;
	}
	
	// Check if line ends with a redirect
	if (!ft_check_redirect_at_end(line))
		return (0);
		
	return (1);
}
