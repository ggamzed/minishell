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
		ft_set_env_value(key, value, &shell->export_list, shell);
	}
	else // Sadece key, boş değer ata
		ft_set_env_value(arg, "", &shell->export_list, shell);
}


int ft_is_valid_export_var(char *str)
{
    int i = 0;

    if (!(('A' <= str[i] && str[i] <= 'Z') || ('a' <= str[i] && str[i] <= 'z') || str[i] == '_'))
        return (0);
    i++;
    while (str[i] && str[i] != '=')
    {
        if (!(('A' <= str[i] && str[i] <= 'Z') || ('a' <= str[i] && str[i] <= 'z') || 
              ('0' <= str[i] && str[i] <= '9') || str[i] == '_'))
            return (0);
        i++;
    }
    return (1);
}


int	ft_builtin_export(char **argv, t_env **env_list, t_shell *shell)
{
	int	i;
	t_env	*curr;

	if (!argv[1]) // argüman yoksa tüm değişkenleri listele
	{
		curr = shell->export_list;
		while (curr)
		{
			if (curr->value)
				printf("declare -x %s=\"%s\"\n", curr->key, curr->value);
			else
				printf("declare -x %s\n", curr->key);
			curr = curr->next;
		}
		//ft_builtin_env(*env_list);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		if (!ft_is_valid_export_var(argv[i]))
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			shell->exit_status = 1; // kontrol et burayı dönüşünde 1 yapılıyorsa gerek yok
			//return (1); // echo $? 1 dönsün
		}
		else
			ft_set_export_variable(argv[i], env_list, shell);
		i++;
	}
	if (shell->exit_status == 1)
		return (1);
	// i = 1;
	// while (argv[i])
	// {
	// 	ft_set_export_variable(argv[i], env_list, shell);
	// 	i++;
	// }
	return (0);
}