// #include "../minishell.h"

// t_token_type	ft_get_word_type(char *line, int i)
// {
// 	if (line[i] == '$' && line[i + 1] == '?')
// 		return (EXIT_STATUS);
// 	if (line[i] == '$' && (ft_isalpha(line[i + 1]) || line[i + 1] == '_'))
// 		return (VARIABLE);
// 	if (line[i] == '\'')
// 		return (SINGLE_QUOTED_STRING);
// 	if (line[i] == '"')
// 		return (DOUBLE_QUOTED_STRING);
// 	return (WORD);
// }

// t_token_type	ft_get_operator_type(char *line, int *i)
// {
// 	if (line[*i] == '|' && ++(*i))
// 		return (PIPE);
// 	if (line[*i] == '<')
// 	{
// 		if (line[*i + 1] == '<' && (*i += 2))
// 			return (HEREDOC);
// 		return (++(*i), REDIRECT_IN);
// 	}
// 	if (line[*i] == '>')
// 	{
// 		if (line[*i + 1] == '>' && (*i += 2))
// 			return (REDIRECT_APPEND);
// 		return (++(*i), REDIRECT_OUT);
// 	}
// 	// sanki buna hiç gerek yok çünkü | < veya > ise bu fonksiyona girecek NULL falan dönebilir belki
// 	return (ft_get_word_type(line, *i)); //bu return'e bak başka ne olabilir?
// }

// char	*ft_get_word(char *line, int *i, t_shell *shell)
// {
// 	int		start;
// 	int		len;
// 	char	*word;
// 	char	quote;

// 	start = *i;
	
// 	// Quote ile başlıyorsa
// 	if (line[*i] == '\'' || line[*i] == '"')
// 	{
// 		quote = line[*i];
// 		(*i)++;      // Açılış quote'unu atla
// 		start = *i;  // Gerçek içerik başlangıcı
		
// 		while (line[*i])
// 		{
// 			// Çift tırnak içinde escape karakterleri
// 			if (quote == '"' && line[*i] == '\\' && line[*i + 1] != '\0')
// 			{
// 				*i += 2;
// 				continue;
// 			}
// 			if (line[*i] == quote)
// 				break;
// 			(*i)++;
// 		}
// 		len = *i - start;  // Quote içindeki uzunluk
// 		(*i)++;           // Kapanış quote'unu atla
// 	}
// 	// $ variable ile başlıyorsa
// 	else if (line[*i] == '$')
// 	{
// 		(*i)++; // $ karakterini atla
		
// 		// $? durumu
// 		if (line[*i] == '?')
// 		{
// 			(*i)++;
// 			len = *i - start;
// 		}
// 		// Normal variable $VAR durumu
// 		else if (ft_isalpha(line[*i]) || line[*i] == '_')
// 		{
// 			while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
// 				(*i)++;
// 			len = *i - start;
// 		}
// 		// Sadece $ karakteri
// 		else
// 		{
// 			len = 1; // Sadece $ karakteri
// 		}
// 	}
// 	// Normal kelime
// 	// Normal kelime
// else
// {
//     while (line[*i] && !ft_is_space(line[*i])
//             && line[*i] != '|' && line[*i] != '<' && line[*i] != '>'
//             && line[*i] != '\'' && line[*i] != '"')
//     {
//         // Escape karakteri varsa bir sonrakini de dahil et
//         if (line[*i] == '\\' && line[*i + 1] != '\0')
//             *i += 2;  // Hem \ hem de sonraki karakteri dahil et
//         else if (line[*i] == '$')
//             break;    // $ gördüğünde dur (ayrı token olsun)
//         else
//             (*i)++;
//     }
//     len = *i - start;
// }
	
// 	word = ft_substr(line, start, len, shell);
// 	if (!word)
// 		return (NULL);
// 	return (word);
// }

// t_token *ft_tokenize(char *line, t_shell *shell)
// {
	
//     t_token *head_of_token;
//     t_token *token;
//     t_token_type type;
//     char *word;
//     int i;
//     int had_space;
    
//     head_of_token = NULL;
//     i = 0;
//     had_space = 1; // İlk token'ın önünde her zaman boşluk var sayılır
    
//     while (line[i])
//     {
//         // Boşlukları atla
//         while (line[i] && ft_is_space(line[i]))
//         {
//             had_space = 1;
//             i++;
//         }
        
//         if (!line[i])
//             break;
            
//         if (line[i] == '|' || line[i] == '<' || line[i] == '>')
//         {
//             type = ft_get_operator_type(line, &i);
//             token = ft_create_token(type, NULL, shell);
//             if (!token)
//                 return (NULL);
            
//             token->space_flag = had_space;
//             ft_add_token(&head_of_token, token);
//             had_space = 0; // Operator sonrası boşluk yok varsayımı
//         }
//         else
//         {
//             type = ft_get_word_type(line, i);
//             word = ft_get_word(line, &i, shell);
//             if (!word)
//                 return (NULL);
            
//             // Boş string kontrolü
//             // if (ft_strlen(word) == 0)
//             // {
// 			// 	had_space = 0;
//             //     continue;
//             // }
            
//             token = ft_create_token(type, word, shell);
//             if (!token)
//                 return (NULL);
            
//             token->space_flag = had_space;
//             ft_add_token(&head_of_token, token);
//             had_space = 0; // Word sonrası boşluk yok varsayımı
//         }
//     }
	
//     return (head_of_token);
// }







#include "../minishell.h"

// Debug fonksiyonu - geçici olarak ekle
void debug_print_tokens(t_token *tokens)
{
    t_token *current = tokens;
    int i = 0;
    
    //printf("=== DEBUG: TOKENS ===\n");
    while (current)
    {
      //  printf("Token[%d]: type=%d, value='%s', space_flag=%d\n", 
    //        i, current->type, current->value ? current->value : "NULL", current->space_flag);
        current = current->next;
        i++;
    }
    printf("=== END DEBUG ===\n");
}

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

t_token *ft_tokenize(char *line, t_shell *shell)
{
	t_token *head_of_token;
	t_token *token;
	t_token_type type;
	char *word;
	int i;
	int had_space;
	
	head_of_token = NULL;
	i = 0;
	had_space = 1; // İlk token'ın önünde her zaman boşluk var sayılır
	
	while (line[i])
	{
		// Boşlukları atla
		while (line[i] && ft_is_space(line[i]))
		{
			had_space = 1;
			i++;
		}
		
		if (!line[i])
			break;
			
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
		{
			type = ft_get_operator_type(line, &i);
			token = ft_create_token(type, NULL, shell);
			if (!token)
				return (NULL);
			
			token->space_flag = had_space;
			ft_add_token(&head_of_token, token);
			
			// DÜZELTME: Operator'dan sonra space kontrolü yap
			// Eğer operator'dan sonra boşluk varsa had_space=1, yoksa had_space=0
			had_space = 0; // Önce sıfırla
			if (line[i] && ft_is_space(line[i])) // Operator'dan sonra boşluk var mı?
				had_space = 1;
		}
		else
		{
			type = ft_get_word_type(line, i);
			word = ft_get_word(line, &i, shell);
			if (!word)
				return (NULL);
			
			token = ft_create_token(type, word, shell);
			if (!token)
				return (NULL);
			
			token->space_flag = had_space;
			ft_add_token(&head_of_token, token);
			
			// DÜZELTME: Word'den sonra space kontrolü yap
			had_space = 0; // Önce sıfırla
			if (line[i] && ft_is_space(line[i])) // Word'den sonra boşluk var mı?
				had_space = 1;
		}
	}
	
	// DEBUG: Token'ları yazdır
	//debug_print_tokens(head_of_token);
	
	return (head_of_token);
}
