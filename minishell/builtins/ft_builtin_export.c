#include "../minishell.h"

// environment değişkenlerini dışa aktarır, argüman yoksa tüm export edilmiş değişkenleri listeler, key=value formatında değişken oluşturur/günceller
static void ft_set_export_variable(char *arg, t_env **env_list, t_shell *shell)
{
    char *equals_sign;
    char *key;
    char *value;
    int i = 0;
    int is_append = 0;

    // += kontrolü
    while (arg[i])
    {
        if (arg[i] == '+' && arg[i + 1] == '=')
        {
            is_append = 1;
            break;
        }
        i++;
    }

    if (is_append) // key+=value formatı (append)
    {
        key = ft_substr(arg, 0, i, shell);
        value = ft_strdup(arg + i + 2, shell); // += den sonraki kısım
        
        // Mevcut değere append et
        char *existing = ft_get_env_value(key, *env_list);
        if (existing)
        {
            char *new_value = ft_strjoin(existing, value, shell);
            ft_set_env_value(key, new_value, env_list, shell);
            ft_set_env_value(key, new_value, &shell->export_list, shell);
        }
        else
        {
            ft_set_env_value(key, value, env_list, shell);
            ft_set_env_value(key, value, &shell->export_list, shell);
        }
    }
    else
    {
        // Normal key=value handling
        equals_sign = ft_strchr(arg, '=');
        if (equals_sign)
        {
            key = ft_substr(arg, 0, equals_sign - arg, shell);
            value = ft_strdup(equals_sign + 1, shell);
            ft_set_env_value(key, value, env_list, shell);
            ft_set_env_value(key, value, &shell->export_list, shell);
        }
        else // Sadece key, değer ataması yok
            ft_set_env_value(arg, NULL, &shell->export_list, shell);
    }
}


int ft_is_valid_export_var(char *str)
{
    int i = 0;

    if (!(('A' <= str[i] && str[i] <= 'Z') || ('a' <= str[i] && str[i] <= 'z') || str[i] == '_'))
        return (0);
    i++;
    while (str[i] && str[i] != '=' && !(str[i] == '+' && str[i + 1] == '='))
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

	if (!argv[1])
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
		return (0);
	}
	
	i = 1;
	while (argv[i])
	{
		// ÖNCE OPTION KONTROLÜ
		if (argv[i][0] == '-')
		{
			if (argv[i][1] == '-') // --option
			{
				ft_putstr_fd("bash: export: ", 2);
				ft_putstr_fd(argv[i], 2);
				ft_putstr_fd(": invalid option\n", 2);
				ft_putstr_fd("export: usage: export [-fn] [name[=value] ...] or export -p\n", 2);
				return (2); // Option error için 2
			}
			else if (ft_strcmp(argv[i], "-p") == 0)
			{
				// -p option handling (şimdilik skip)
				i++;
				continue;
			}
			else
			{
				ft_putstr_fd("bash: export: ", 2);
				ft_putstr_fd(argv[i], 2);
				ft_putstr_fd(": invalid option\n", 2);
				return (2);
			}
		}
		
		// SONRA IDENTIFIER KONTROLÜ
		if (!ft_is_valid_export_var(argv[i]))
		{
			ft_putstr_fd("export: not a valid identifier\n", 2);
			shell->exit_status = 1;
		}
		else
			ft_set_export_variable(argv[i], env_list, shell);
		i++;
	}
	
	if (shell->exit_status == 1)
		return (1);
	return (0);
}