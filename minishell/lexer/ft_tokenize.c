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
	// sanki buna hiç gerek yok çünkü | < veya > ise bu fonksiyona girecek NULL falan dönebilir belki
	return (ft_get_word_type(line, *i)); //bu return'e bak başka ne olabilir?
}

// char	*ft_get_word(char *line, int *i, t_shell *shell)
// {
// 	int		start;
// 	int		len;
// 	char	*word;
// 	char	quote;

// 	start = *i;
// 	if (line[*i] == '\'' || line[*i] == '"') // Eğer quote ile başlıyorsa, quote'ları çıkar
// 	{
// 		quote = line[*i];
// 		(*i)++;      // Açılış quote'unu atla
// 		start = *i;  // Gerçek içerik başlangıcı
// 		while (line[*i])
// 		{
// 			// "" içinde kacis karakterlerine literal gibi davranmasi icin (\\")
// 			if (quote == '"' && line[*i] == '\\' && line[*i + 1] != '\0')
// 			{
// 				*i += 2;
// 				continue;
// 			}
// 			if (line[*i] == quote)
// 				break;
// 			(*i)++;
// 		}
// 		//while (line[*i] && line[*i] != quote)
// 		//	(*i)++;
// 		len = *i - start;  // Quote içindeki uzunluk
// 		(*i)++;           // Kapanış quote'unu atla
// 	}
// 	else
// 	{
// 		while (line[*i] && !ft_is_space(line[*i]) // Normal kelime işleme
// 				&& line[*i] != '|' && line[*i] != '<' && line[*i] != '>'
// 				&& line[*i] != '\'' && line[*i] != '"' && line[*i] != '$')
// 			(*i)++;
// 		len = *i - start;
// 	}
// 	word = ft_substr(line, start, len, shell);
// 	if (!word)
// 		return (NULL);
// 	return (word);
// }

// t_token	*ft_tokenize(char *line, t_shell *shell)
// {
// 	t_token			*head_of_token;
// 	t_token			*token;
// 	t_token_type	type;
// 	char			*word;
// 	int				i;

// 	//if (!ft_validate_syntax(line)) //(main process_line'da yapılıyor burada gerek yok?) syntax kontrolü, kapanmamış tırnak var mı? başta yada sonda pipe/redirection yada ekstradan var mı?
// 	//	return (NULL);
// 	head_of_token = NULL;
// 	i = 0;
// 	while (line[i])
// 	{
// 		while (line[i] && ft_is_space(line[i]))
// 			i++;
// 		// if (!line[i])
// 		// 	break;
// 		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
// 		{
// 			type = ft_get_operator_type(line, &i);
// 			token = ft_create_token(type, NULL, shell); //null -> zaten type da ne olduğunu tutuyoruz
// 			if (!token)
// 				return (NULL);
// 			ft_add_token(&head_of_token, token);
// 		}
// 		else
// 		{
// 			word = ft_get_word(line, &i, shell);
// 			type = ft_get_word_type(line, i);
// 			if (!word)
// 				return (NULL);
// 			// if (!word)
// 			// {
// 			// 	ft_free_tokens(head_of_token);
// 			// 	return (NULL);
// 			// }
// 			token = ft_create_token(type, word, shell);
// 			if (!token)
// 				return (NULL);
// 			ft_add_token(&head_of_token, token);
// 			// free(word); // BU SATIRI KALDIR! Token artık word'ü sahiplenir
// 		}
// 	}
// 	return (head_of_token); //parser'a gönderilecek
// }

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
				&& line[*i] != '\'' && line[*i] != '"' && line[*i] != '$')
			(*i)++;
		len = *i - start;
	}
	
	word = ft_substr(line, start, len, shell);
	if (!word)
		return (NULL);
	return (word);
}

t_token	*ft_tokenize(char *line, t_shell *shell)
{
	t_token			*head_of_token;
	t_token			*token;
	t_token_type	type;
	char			*word;
	int				i;

	//if (!ft_validate_syntax(line)) //(main process_line'da yapılıyor burada gerek yok?) syntax kontrolü, kapanmamış tırnak var mı? başta yada sonda pipe/redirection yada ekstradan var mı?
	//	return (NULL);
	head_of_token = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && ft_is_space(line[i]))
			i++;
		// if (!line[i])
		// 	break;
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
		{
			type = ft_get_operator_type(line, &i);
			token = ft_create_token(type, NULL, shell);
			if (!token)
				return (NULL);
			ft_add_token(&head_of_token, token);
			
			// Operator'dan sonra boşluk kontrolü -> gerek var mı?
			if (line[i] && ft_is_space(line[i]))
				token->space_flag = 1;
			else
				token->space_flag = 0;
		}
		else
    {
        type = ft_get_word_type(line, i);
        word = ft_get_word(line, &i, shell);
        if (!word)
            return (NULL);
        
        // BOŞ STRING KONTROLÜ EKLEYIN
        if (ft_strlen(word) == 0)
        {
            // Boş string'i atla
            continue;
        }
        
        token = ft_create_token(type, word, shell);
        if (!token)
            return (NULL);
        ft_add_token(&head_of_token, token);
        
        if (line[i] && ft_is_space(line[i]))
            token->space_flag = 1;
        else
            token->space_flag = 0;
    }
	}
	return (head_of_token);
}
