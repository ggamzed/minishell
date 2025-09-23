#include "../minishell.h"

t_token_type	ft_get_word_type(char *line, int i)
{
	if (line[i] == '$' && line[i + 1] == '?')
		return (EXIT_STATUS);
	if (line[i] == '$' && (ft_isalpha(line[i + 1]) || line[i + 1] == '_'))
		return (VARIABLE);
	if (line[i] == '\'')
		return (SINGLE_QUOTED_STRING);
	if (line[i] == '"')
		return (DOUBLE_QUOTED_STRING);
	return (WORD);
}

t_token_type	ft_get_operator_type(char *line, int *i)
{
	if (line[*i] == '|' && ++(*i))
		return (PIPE);
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<' && (*i += 2))
			return (HEREDOC);
		return (++(*i), REDIRECT_IN);
	}
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>' && (*i += 2))
			return (REDIRECT_APPEND);
		return (++(*i), REDIRECT_OUT);
	}
	return (ft_get_word_type(line, *i));
}

char	*ft_get_word(char *line, int *i, t_shell *shell)
{
	int		start;
	int		len;
	char	*word;
	char	quote;

	start = *i;
	
	// Quote ile başlıyorsa
	if (line[*i] == '\'' || line[*i] == '"')
	{
		quote = line[*i];
		(*i)++;      // Açılış quote'unu atla
		start = *i;  // Gerçek içerik başlangıcı
		
		while (line[*i])
		{
			// Çift tırnak içinde escape karakterleri
			if (quote == '"' && line[*i] == '\\' && line[*i + 1] != '\0')
			{
				*i += 2;
				continue;
			}
			if (line[*i] == quote)
				break;
			(*i)++;
		}
		len = *i - start;  // Quote içindeki uzunluk
		(*i)++;           // Kapanış quote'unu atla
	}
	// $ variable ile başlıyorsa
	else if (line[*i] == '$')
	{
		(*i)++; // $ karakterini atla
		
		// $? durumu
		if (line[*i] == '?')
		{
			(*i)++;
			len = *i - start;
		}
		// Normal variable $VAR durumu
		else if (ft_isalpha(line[*i]) || line[*i] == '_')
		{
			while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
				(*i)++;
			len = *i - start;
		}
		// Sadece $ karakteri
		else
		{
			len = 1; // Sadece $ karakteri
		}
	}
	// Normal kelime
	else
	{
		while (line[*i] && !ft_is_space(line[*i])
				&& line[*i] != '|' && line[*i] != '<' && line[*i] != '>'
				&& line[*i] != '\'' && line[*i] != '"')
		{
			// Escape karakteri varsa bir sonrakini de dahil et
			if (line[*i] == '\\' && line[*i + 1] != '\0')
				*i += 2;  // Hem \ hem de sonraki karakteri dahil et
			else if (line[*i] == '$')
				break;    // $ gördüğünde dur (ayrı token olsun)
			else
				(*i)++;
		}
		len = *i - start;
	}
	
	word = ft_substr(line, start, len, shell);
	if (!word)
		return (NULL);
	return (word);
}