#include "../minishell.h"

// environment değişkenlerini dışa aktarır, argüman yoksa tüm export edilmiş değişkenleri listeler, key=value formatında değişken oluşturur/günceller
static void	ft_set_export_variable(char *arg, t_env **env_list, t_shell *shell)
{
	char	*equals_sign;
	char	*key;
	char	*value;

	equals_sign = ft_strchr(arg, '='); // '=' karakterini ara
	if (equals_sign) // key=value formatı
	{
		key = ft_substr(arg, 0, equals_sign - arg, shell);
		value = ft_strdup(equals_sign + 1, shell);
		ft_set_env_value(key, value, env_list, shell);
		free(key);
		free(value);
	}
	else // Sadece key, boş değer ata
		ft_set_env_value(arg, "", env_list, shell);
}

int	ft_builtin_export(char **argv, t_env **env_list, t_shell *shell)
{
	int	i;

	if (!argv[1]) // argüman yoksa tüm değişkenleri listele
	{
		ft_builtin_env(*env_list);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		ft_set_export_variable(argv[i], env_list, shell);
		i++;
	}
	return (0);
}