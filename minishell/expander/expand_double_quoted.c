#include "../minishell.h"

char	*ft_append_char(char *str, char c, t_shell *shell)
{
	char	temp[2];
	char	*temp_str;

	temp[0] = c;
	temp[1] = '\0';
	temp_str = ft_strdup(temp, shell);
	return (ft_strjoin(str, temp_str, shell));
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
		temp = ft_extract_and_expand_var(str, &i, shell);
		if (temp)
			result = ft_strjoin(result, temp, shell);
		else
		{
			result = ft_append_char(result, str[i], shell);
			i++;
		}
	}
	return (result);
}
