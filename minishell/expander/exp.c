#include "../minishell.h"

// Executor'dan çağrılacak ana fonksiyonlar için utility fonksiyonlar

static char	*ft_safe_strdup(char *str)
{
	// NULL pointer'ı güvenli şekilde kopyalar
	// Executor'da NULL value ile karşılaştığımızda crash'i önler
	if (!str)
		return (ft_strdup(""));
	return (ft_strdup(str));
}

static char	*ft_extract_var_name(char *str)
{
	// $HOME veya $USER gibi variable'lardan sadece ismi çıkarır
	// $ işaretini atlar ve sadece variable adını döndürür
	int	i;
	int	start;

	i = 0;
	if (str[i] == '$')
		i++;
	start = i;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, start, i - start));
}

static char	*ft_get_env_value(char *key, t_shell *shell)
{
	// Environment list'den key'e göre value bulur
	// Executor'da $HOME, $USER gibi variable'ları çözmek için kullanılır
	t_env	*current;

	if (!key || !shell || !shell->env_list)
		return (NULL);
	current = shell->env_list;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static char	*ft_handle_exit_status(t_shell *shell)
{
	// $? variable'ını handle eder
	// Son komutun exit code'unu string'e çevirir
	if (!shell)
		return (ft_strdup("0"));
	return (ft_itoa(shell->exit_status));
}

static char	*ft_expand_variable(char *value, t_shell *shell)
{
	// VARIABLE token type'ı için expansion yapar
	// $HOME → /home/user şeklinde genişletir
	char	*var_name;
	char	*env_value;

	var_name = ft_extract_var_name(value);
	if (!var_name)
		return (ft_strdup(""));
	env_value = ft_get_env_value(var_name, shell);
	free(var_name);
	return (ft_safe_strdup(env_value));
}

static char	*ft_append_char_to_string(char *str, char c)
{
	// String'e tek karakter ekler
	// DOUBLE_QUOTED_STRING processing'de karakter karakter ekleme için
	char	temp[2];
	char	*temp_str;

	temp[0] = c;
	temp[1] = '\0';
	temp_str = ft_strdup(temp);
	return (ft_strjoin_free(str, temp_str));
}

static char	*ft_process_dollar_in_double_quotes(char *str, int *i, t_shell *shell)
{
	// Double quote içindeki $ işlemlerini handle eder
	// $HOME veya $? gibi variable'ları genişletir
	int		start;
	char	*var_name;
	char	*env_value;

	(*i)++; // '$' karakterini atla
	if (str[*i] == '?') // $? durumu
	{
		(*i)++;
		return (ft_handle_exit_status(shell));
	}
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	env_value = ft_get_env_value(var_name, shell);
	free(var_name);
	return (ft_safe_strdup(env_value));
}

static char	*ft_expand_double_quoted_string(char *str, t_shell *shell)
{
	// DOUBLE_QUOTED_STRING token'ları için expansion
	// "Hello $USER" → "Hello egokce" şeklinde genişletir
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (str[i + 1] == '?' || 
			ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
		{
			temp = ft_process_dollar_in_double_quotes(str, &i, shell);
			result = ft_strjoin_free(result, temp);
		}
		else
		{
			result = ft_append_char_to_string(result, str[i]);
			i++;
		}
	}
	return (result);
}

char	*ft_expand_single_token(char *value, t_token_type type, t_shell *shell)
{
	// Tek bir token'ı expand eder
	// Executor'dan çağrılacak ana fonksiyon - her token type için farklı işlem
	if (!value)
		return (ft_strdup(""));
	
	if (type == VARIABLE)
		return (ft_expand_variable(value, shell));
	else if (type == EXIT_STATUS)
		return (ft_handle_exit_status(shell));
	else if (type == DOUBLE_QUOTED_STRING)
		return (ft_expand_double_quoted_string(value, shell));
	else if (type == SINGLE_QUOTED_STRING)
		return (ft_strdup(value)); // Tek tırnak hiç genişletilmez
	else if (type == WORD)
		return (ft_strdup(value)); // Normal kelimeler olduğu gibi kalır
	else
		return (ft_strdup(value)); // Bilinmeyen tipler güvenli şekilde kopyalanır
}

char	*ft_expand_filename(char *filename, t_token_type type, t_shell *shell)
{
	// Redirection dosya adlarını expand eder
	// Executor'da input/output file adlarını çözmek için kullanılır
	// Heredoc delimiter'lar için özel kontrol YOKTUR - onlar executor'da kontrol edilir
	return (ft_expand_single_token(filename, type, shell));
}

char	**ft_expand_arguments_to_argv(t_token *args, t_shell *shell)
{
	// Argument linked list'ini argv array'ine çevirir
	// Executor'da execve için hazır hale getirir
	char		**argv;
	t_token	*current;
	char		*expanded;
	int			count;
	int			i;

	// Argument sayısını say
	count = 0;
	current = args;
	while (current)
	{
		count++;
		current = current->next;
	}
	
	// argv array'i oluştur
	argv = ft_malloc(sizeof(char *) * (count + 1));
	
	// Her argümanı expand et ve array'e ekle
	current = args;
	i = 0;
	while (current)
	{
		expanded = ft_expand_single_token(current->value, current->type, shell);
		argv[i++] = expanded;
		current = current->next;
	}
	argv[i] = NULL; // Array'i NULL ile sonlandır
	return (argv);
}