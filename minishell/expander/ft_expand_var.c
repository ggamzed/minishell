#include "../minishell.h"

static char	*ft_handle_var_and_char(char *result, char *str, int *i,
										t_shell *shell)
{
	char	*temp;

	temp = ft_extract_and_expand_var(str, i, shell);
	if (temp)
		result = ft_strjoin(result, temp, shell);
	else
	{
		result = ft_append_char(result, str[*i], shell);
		(*i)++;
	}
	return (result);
}

char	*ft_expand_word_variables(char *str, t_shell *shell)
{
	char	*result;
	int		i;

	if (ft_strcmp(str, "$") == 0)
		return (ft_strdup("", shell));
	result = ft_strdup("", shell);
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] != '\0')
		{
			result = ft_append_char(result, str[i + 1], shell);
			i += 2;
		}
		else
			result = ft_handle_var_and_char(result, str, &i, shell);
	}
	return (result);
}

char	*ft_handle_word(char *value, t_shell *shell)
{
	char	*tilde;

	tilde = ft_expand_tilde(value, shell);
	if (tilde)
		return (tilde);
	return (ft_expand_word_variables(value, shell));
}
