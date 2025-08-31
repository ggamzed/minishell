#include "../minishell.h"

static char	*ft_append_char(char *str, char c, t_shell *shell)
{
	char	temp[2];
	char	*temp_str;

	temp[0] = c;
	temp[1] = '\0';
	temp_str = ft_strdup(temp, shell);
	return (ft_strjoin_free(str, temp_str, shell));
}

static char	*ft_expand_dollar_quoted(char *str, int *i, t_shell *shell)
{
	char	*temp;

	if (str[*i] == '$' && str[*i + 1] == '?')
	{
		temp = ft_handle_exit_status(shell);
		*i += 2;
		return (temp);
	}
	else if (str[*i] == '$' && (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_'))
	{
		temp = ft_extract_and_expand_var(str, i, shell);
		return (temp);
	}
	return (NULL);
}

char	*ft_expand_double_quoted(char *str, t_shell *shell)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("", shell);
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		temp = ft_expand_dollar_quoted(str, &i, shell);
		if (temp)
			result = ft_strjoin_free(result, temp, shell);
		else
		{
			result = ft_append_char(result, str[i], shell);
			i++;
		}
	}
	return (result);
}