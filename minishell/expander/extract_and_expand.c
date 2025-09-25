#include "../minishell.h"

static char	*ft_extract_variable_name(char *str, int *i, t_shell *shell)
{
	int		start;

	if (str[*i] == '$')
		(*i)++;
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_substr(str, start, *i - start, shell));
}

char	*ft_extract_and_expand_var(char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*env_value;

	var_name = ft_extract_variable_name(str, i, shell);
	if (!var_name || !*var_name)
	{
		return (ft_strdup("", shell)); 
	}
	env_value = ft_get_env_value(var_name, shell->env_list);
	if (env_value)
		return (ft_strdup(env_value, shell));
	return (ft_strdup("", shell));
}