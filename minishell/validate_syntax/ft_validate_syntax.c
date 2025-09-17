#include "../minishell.h"

//kapanmamış tırnak var mı?
// static int	ft_validate_quotes(char *line)
// {
// 	int		i;
// 	char	quote;

// 	i = 0;
// 	while (line[i])
// 	{
// 		if (line[i] == '\'' || line[i] == '"')
// 		{
// 			quote = line[i];
// 			i++;
// 			while (line[i] && line[i] != quote)
// 				i++;
// 			if (!line[i])
// 			{
// 				printf("minishell: syntax error: unclosed quote\n");
// 				return (0);
// 			}
// 			i++; //buradaki i++ neden var sildiğimde neden çalışmıyor?
// 		}
// 		else
// 			i++;
// 	}
// 	return (1);
// }

// static int	ft_validate_pipes(char *line)
// {
// 	int	i = 0;

// 	while (line[i] && ft_is_space(line[i])) // Başlangıçta pipe kontrolü
// 		i++;
// 	if (line[i] == '|')
// 	{
// 		printf("minishell: syntax error near unexpected token `|'\n");
// 		return (0);
// 	}
// 	i = ft_strlen(line) - 1;
// 	while (i >= 0 && ft_is_space(line[i])) // Sonda pipe kontrolü
// 		i--;
// 	if (i >= 0 && line[i] == '|')
// 	{
// 		printf("minishell: syntax error near unexpected token `|'\n");
// 		return (0);
// 	}
// 	i = 0;
// 	while (line[i]) // Çift pipe kontrolü (|| değil, | | gibi)
// 	{
// 		if (line[i] == '|')
// 		{
// 			i++;
// 			while (line[i] && ft_is_space(line[i]))
// 				i++;
// 			if (line[i] == '|')
// 			{
// 				printf("minishell: syntax error near unexpected token `|'\n");
// 				return (1);
// 			}
// 		}
// 		else
// 			i++;
// 	}
// 	return (0);
// }

//bu fonksiyona bak. << kontrol edilmesine gerek yok sadece < yada > olması yeterli mi?
// static int	ft_validate_redirections(char *line)
// {
// 	int	i;

// 	i = 0;
// 	while (line[i])
// 	{
// 		if (line[i] == '<' || line[i] == '>')
// 		{
// 			if (line[i] == '<') // Operatörü atla (< veya << veya > veya >>)
// 			{
// 				i++;
// 				if (line[i] == '<')
// 					i++;
// 			}
// 			else if (line[i] == '>')
// 			{
// 				i++;
// 				if (line[i] == '>')
// 					i++;
// 			}
// 			while (line[i] && ft_is_space(line[i]))
// 				i++;
// 			if (!line[i] || line[i] == '|' || line[i] == '<' || line[i] == '>') // Filename kontrolü && karışık operatör && çift operatör (< <)
// 			{
// 				printf("minishell: syntax error near unexpected token `newline'\n");
// 				return (0);
// 			}
// 		}
// 		else
// 			i++;
// 	}
	
// 	return (1);
// }

int	ft_validate_syntax(char *line)
{
	if (!ft_validate_quotes(line))
		return (0);
	if (!ft_validate_pipes(line))
		return (0);
	if (!ft_validate_redirections(line))
		return (0);
	return (1);
}
