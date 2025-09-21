#include "../minishell.h"

char	*ft_handle_exit_status(t_shell *shell)
{
	return (ft_itoa(shell->exit_status, shell));
}

static char *ft_expand_tilde(const char *value, t_shell *shell)
{
    char    *base;
    char    *suffix;
    char    *expanded;

    if (!value || value[0] != '~')
        return (NULL);

    // tildeden sonra gelen karakterler bunlar olmali '/' , '+' , or '-'
    if (value[1] != '\0' && value[1] != '/' && value[1] != '+' && value[1] != '-')
        return (NULL);

    if (value[1] == '+' || value[1] == '-')
    {
        if (value[1] == '+')
            base = ft_get_env_value("PWD", shell->env_list);
        else
            base = ft_get_env_value("OLDPWD", shell->env_list);
        suffix = (char *)(value + 2);
    } 
    else // Diğer durumlar (sadece ~ veya ~/...) → kullanıcının HOME dizini.
    {
        base = ft_get_env_value("HOME", shell->env_list);
        suffix = (char *)(value + 1);
    }

    // Eğer ortam değişkeni yoksa (örneğin $HOME unset ise)
    if (!base)
        return (ft_strdup(value, shell));

    // suffix; ~, ~+, ~-’den sonraki kalan kısım (örn: "/Desktop" gibi).
    // Eğer suffix boşsa (örn: ~ ya da ~+ tek başına) 
    if (suffix[0] == '\0')
        return (ft_strdup(base, shell));

    // Eğer suffix / ile başlıyorsa (örn: ~/Desktop) → base + suffix birleştir.
    if (suffix[0] == '/')
        return (ft_strjoin(base, suffix, shell));

    // Kalan durum (Normalde buraya gelinmemeli). sadece /, +, -, veya \0 izin var
    expanded = ft_strjoin(base, "/", shell);
    expanded = ft_strjoin_free(expanded, ft_strdup(suffix, shell), shell);
    return (expanded);
}

char *ft_expand_word_variables(char *str, t_shell *shell)
{
    // Eğer sadece "$" ise boş string döndür
    if (ft_strcmp(str, "$") == 0)
        return (ft_strdup("", shell));
    
    char *result;
    char *temp;
    int i;

    result = ft_strdup("", shell);
    if (!result)
        return (NULL);
    
    i = 0;
    while (str[i])
    {
        // $ ile başlayan variable'ları expand et
        if (str[i] == '$' && str[i + 1] == '?')
        {
            temp = ft_handle_exit_status(shell);
            result = ft_strjoin_free(result, temp, shell);
            i += 2;
        }
        else if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
        {
            temp = ft_extract_and_expand_var(str, &i, shell);
            result = ft_strjoin_free(result, temp, shell);
        }
       else
{
    // Escape karakteri kontrolü
    if (str[i] == '\\' && str[i + 1] != '\0')
    {
        // Bir sonraki karakteri literal olarak ekle (escape'i atla)
        temp = ft_malloc(2, shell);
        temp[0] = str[i + 1];
        temp[1] = '\0';
        result = ft_strjoin_free(result, temp, shell);
        i += 2; // Hem \ hem de sonraki karakteri atla
    }
    else
    {
        // Normal karakterleri olduğu gibi ekle
        temp = ft_malloc(2, shell);
        temp[0] = str[i];
        temp[1] = '\0';
        result = ft_strjoin_free(result, temp, shell);
        i++;
    }
}
    }
    return (result);
}

static char	*ft_expand_token_value(char *value, t_token_type type, t_shell *shell, int is_heredoc_delimiter)
{
	int	i;
	char *tilde;

	i = 0;
	if (!value)
		return (NULL);
	
	// Heredoc delimiter ise hiçbir zaman genişletme
	if (is_heredoc_delimiter)
		return (ft_strdup(value, shell));
	if (type == VARIABLE)
		return (ft_extract_and_expand_var(value, &i, shell));
	else if (type == EXIT_STATUS)
		return (ft_handle_exit_status(shell));
	else if (type == DOUBLE_QUOTED_STRING)
		return (ft_expand_double_quoted(value, shell));
	else if (type == SINGLE_QUOTED_STRING)
		return (ft_strdup(value, shell));
	else if (type == WORD)
{
    tilde = ft_expand_tilde(value, shell);
    if (tilde)
        return (tilde);
    
    // Variable expansion yap
    return (ft_expand_word_variables(value, shell)); // ← BURADA ÇAĞIRILIYOR
}
	else
		return (ft_strdup(value, shell));
}

static int	count_args(t_token *args)
{
	t_token *current;
	int	count;

	count = 0;
	current = args;
	while (current)
	{
		current = current->next;
		count++;
	}
	return (count);
}


// Word splitting fonksiyonu
char **ft_split_expanded_word(char *word, t_shell *shell)
{
    char **words;
    int count = 0;
    int i = 0, j = 0, start;
    
    // Kelime sayısını say
    while (word[i])
    {
        while (word[i] && ft_is_space(word[i]))
            i++;
        if (word[i])
        {
            count++;
            while (word[i] && !ft_is_space(word[i]))
                i++;
        }
    }
    
    if (count == 0)
        return (NULL);
        
    words = ft_malloc(sizeof(char *) * (count + 1), shell);
    if (!words)
        return (NULL);
    
    i = 0;
    while (word[i] && j < count)
    {
        while (word[i] && ft_is_space(word[i]))
            i++;
        start = i;
        while (word[i] && !ft_is_space(word[i]))
            i++;
        if (i > start)
        {
            words[j] = ft_substr(word, start, i - start, shell);
            j++;
        }
    }
    words[j] = NULL;
    return (words);
}

// Word splitting gerekip gerekmediğini kontrol et
int should_word_split(t_token *token_group)
{
    t_token *current = token_group;
    
    // Token grubunda unquoted variable var mı kontrol et
    while (current)
    {
        if (current->type == VARIABLE)
            return (1); // Unquoted variable varsa split yap
        if (current->next && current->next->space_flag == 0)
            current = current->next;
        else
            break;
    }
    return (0); // Split yapma
}

// Mevcut ft_join_expand_tokens fonksiyonunu bu kodla DEĞİŞTİRİN:

void ft_join_expand_tokens(char ***joined_argv, char **expanded_argv, 
                            t_token *original_tokens, t_shell *shell)
{
    char **final_argv;
    t_token *current;
    char **split_words;
    int final_count = 0;
    int i, j, k;
    
    // İlk geçiş: toplam kelime sayısını hesapla
    current = original_tokens;
    i = 0;
    while (current)
    {
        char *merged_word = ft_strdup(expanded_argv[i], shell);
        t_token *token_start = current;
        
        // Bitişik token'ları birleştir
        while (current->next && current->next->space_flag == 0)
        {
            current = current->next;
            i++;
            merged_word = ft_strjoin_free(merged_word, expanded_argv[i], shell);
        }
        
        // Word splitting gerekli mi kontrol et
        if (should_word_split(token_start))
        {
            split_words = ft_split_expanded_word(merged_word, shell);
            if (split_words)
            {
                j = 0;
                while (split_words[j])
                {
                    final_count++;
                    j++;
                }
            }
        }
        else
        {
            final_count++; // Tek kelime olarak say
        }
        
        i++;
        current = current->next;
    }
    
    // Final argv dizisini oluştur
    final_argv = ft_malloc(sizeof(char *) * (final_count + 1), shell);
    if (!final_argv)
    {
        *joined_argv = NULL;
        return;
    }
    
    // İkinci geçiş: kelimeleri kopyala
    current = original_tokens;
    i = 0;
    k = 0;
    while (current)
    {
        char *merged_word = ft_strdup(expanded_argv[i], shell);
        t_token *token_start = current;
        
        // Bitişik token'ları birleştir
        while (current->next && current->next->space_flag == 0)
        {
            current = current->next;
            i++;
            merged_word = ft_strjoin_free(merged_word, expanded_argv[i], shell);
        }
        
        // Word splitting gerekli mi kontrol et
        if (should_word_split(token_start))
        {
            split_words = ft_split_expanded_word(merged_word, shell);
            if (split_words)
            {
                j = 0;
                while (split_words[j])
                {
                    final_argv[k] = ft_strdup(split_words[j], shell);
                    k++;
                    j++;
                }
            }
        }
        else
        {
            final_argv[k] = ft_strdup(merged_word, shell);
            k++;
        }
        
        i++;
        current = current->next;
    }
    
    final_argv[k] = NULL;
    *joined_argv = final_argv;
}


char	**ft_expand_tokens(t_token *args, t_shell *shell)
{
	char		**argv;
	char		**join_argv;
	t_token		*current;
	int			i;
	
	// Allocate argv array
	argv = ft_malloc(sizeof(char *) * (count_args(args) + 1), shell);
	if (!argv)
		return (NULL);
	// Expand each argument
	current = args;
	i = 0;
	while (current)
	{
		argv[i++] = ft_expand_token_value(current->value, current->type, shell, 0);
		
		current = current->next;
	}
	argv[i] = NULL;
	ft_join_expand_tokens(&join_argv, argv, args, shell);
	i = 0;
	// while (argv[i])
	// {
	// 	free(argv[i]);
	// 	i++;
	// }
	// free(argv);
	return (join_argv);
}

