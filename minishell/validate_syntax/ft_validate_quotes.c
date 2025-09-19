#include "../minishell.h"

// int	ft_validate_quotes(char *line)
// {
// 	int		i;
// 	char	quote;

// 	i = 0;
// 	while (line[i])
// 	{
// 		if ((line[i] == '\'' || line[i] == '"') && ((i != 0) && (line[i - 1] != '\\')))
// 		{
// 			quote = line[i];
// 			i++;
// 			//while (line[i] && line[i] != quote)
// 			//	i++;
// 			while (line[i])
// 			{
// 				// "" içinde kacis karakterlerine literal gibi davranmasi icin (\\")
// 				if (quote == '"' && line[i] == '\\' && line[i + 1] != '\0')
// 				{
// 					i += 2;
// 					continue;
// 				}
// 				if (line[i] == quote)
// 					break;
// 				i++;
// 			}
// 			if (!line[i])
// 			{
// 				printf("minishell: syntax error: unclosed quote\n");
// 				return (1);
// 			}
// 			i++;
// 		}
// 		else
// 			i++;
// 	}
// 	return (0);
// }

int	ft_validate_quotes(char *line)
{
	int		i;
	char	quote;
	int		in_quote;

	i = 0;
	in_quote = 0;
	while (line[i])
	{
		if ((line[i] == '\'' || line[i] == '"') && !in_quote && 
		    (i == 0 || line[i - 1] != '\\'))
		{
			quote = line[i];
			in_quote = 1;
			i++;
			while (line[i] && in_quote)
			{
				if (quote == '"' && line[i] == '\\' && line[i + 1] != '\0')
				{
					i += 2;
					continue;
				}
				if (line[i] == quote)
				{
					in_quote = 0;
					break;
				}
				i++;
			}
			if (in_quote)
			{
				printf("minishell: syntax error: unclosed quote\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}
