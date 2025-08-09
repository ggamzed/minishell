#include "../minishell.h"

int	ft_validate_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			quote = line[i];
			i++;
			//while (line[i] && line[i] != quote)
			//	i++;
			while (line[i])
			{
				// "" içinde kacis karakterlerine literal gibi davranmasi icin (\\")
				if (quote == '"' && line[i] == '\\' && line[i + 1] != '\0')
				{
					i += 2;
					continue;
				}
				if (line[i] == quote)
					break;
				i++;
			}
			if (!line[i])
			{
				printf("minishell: syntax error: unclosed quote\n");
				return (0);
			}
			i++;
		}
		else
			i++;
	}
	return (1);
}
