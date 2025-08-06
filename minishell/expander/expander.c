#include "../minishell.h"

static char	*ft_handle_exit_status(t_shell *shell)
{
	return (ft_itoa(shell->exit_status));
}

static char	*ft_extract_variable_name(char *str)
{
	int		i;
	int		start;

	i = 0;
	if (str[i] == '$')
		i++;
	start = i;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (ft_substr(str, start, i - start));
}

static char	*ft_expand_variable_token(char *value, t_shell *shell)
{
	char	*var_name;
	char	*env_value;

	var_name = ft_extract_variable_name(value);
	if (!var_name)
		return (ft_strdup(""));
	env_value = ft_get_env_value(var_name, shell->env_list);
	free(var_name);
	if (env_value)
		return (ft_strdup(env_value));
	return (ft_strdup(""));
}

static char	*ft_append_char(char *str, char c)
{
	char	temp[2];
	char	*temp_str;

	temp[0] = c;
	temp[1] = '\0';
	temp_str = ft_strdup(temp);
	return (ft_strjoin_free(str, temp_str));
}

static char	*ft_extract_and_expand_var(char *str, int *i, t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*env_value;

	(*i)++; // Skip '$'
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	env_value = ft_get_env_value(var_name, shell->env_list);
	free(var_name);
	if (env_value)
		return (ft_strdup(env_value));
	return (ft_strdup(""));
}

static char	*ft_expand_double_quoted(char *str, t_shell *shell)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] == '?')
		{
			temp = ft_handle_exit_status(shell);
			result = ft_strjoin_free(result, temp);
			i += 2;
		}
		else if (str[i] == '$' && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
		{
			temp = ft_extract_and_expand_var(str, &i, shell);
			result = ft_strjoin_free(result, temp);
		}
		else
		{
			result = ft_append_char(result, str[i]);
			i++;
		}
	}
	return (result);
}

char	*ft_expand_token_value(char *value, t_token_type type, t_shell *shell, int is_heredoc_delimiter)
{
	if (!value)
		return (NULL);
	
	// Heredoc delimiter ise hiçbir zaman genişletme
	if (is_heredoc_delimiter)
		return (ft_strdup(value));
		
	if (type == VARIABLE)
		return (ft_expand_variable_token(value, shell));
	else if (type == EXIT_STATUS)
		return (ft_handle_exit_status(shell));
	else if (type == DOUBLE_QUOTED_STRING)
		return (ft_expand_double_quoted(value, shell));
	else if (type == SINGLE_QUOTED_STRING)
		return (ft_strdup(value));
	else if (type == WORD)
		return (ft_strdup(value));
	else
		return (ft_strdup(value));
}

char	**ft_expand_cmd_arguments(t_cmd_arg *args, t_shell *shell)
{
	char		**argv;
	t_cmd_arg	*current;
	char		*expanded;
	int			count;
	int			i;

	// Count arguments
	count = 0;
	current = args;
	while (current)
	{
		count++;
		current = current->next;
	}
	
	// Allocate argv array
	argv = ft_malloc(sizeof(char *) * (count + 1));
	
	// Expand each argument
	current = args;
	i = 0;
	while (current)
	{
		expanded = ft_expand_token_value(current->value, current->type, shell, 0);
		argv[i++] = expanded;
		current = current->next;
	}
	argv[i] = NULL;
	return (argv);
}

// Backward compatibility için eski fonksiyon
char	*ft_expand_variables(char *str, t_shell *shell)
{
	return (ft_expand_double_quoted(str, shell));
}