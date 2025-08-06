#include "../minishell.h"

t_env	*ft_create_env_node(char *key, char *value)
{
	t_env	*node;

	node = ft_malloc(sizeof(t_env));
	node->key = ft_strdup(key);
	if (value)
    	node->value = ft_strdup(value);
	else
    	node->value = NULL;
	node->next = NULL;
	return (node);
}

void	ft_add_env_node(t_env **env_list, t_env *new_node)
{
	t_env	*current;

	if (!*env_list)
	{
		*env_list = new_node;
		return;
	}
	current = *env_list;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

char *ft_parse_env_value(char *env_str)
{
    char *equals_sign;
    char *value;

    equals_sign = ft_strchr(env_str, '='); // ADIM 1: '=' karakterini bul
    if (!equals_sign)
        return (NULL);
    value = ft_strdup(equals_sign + 1); // ADIM 2: '=' den sonraki kısmı kopyala
    return (value);
}

char *ft_parse_env_key(char *env_str)
{
    char *equals_sign;   // '=' karakteri
    char *key;

    equals_sign = ft_strchr(env_str, '='); // ADIM 1: '=' karakterini bul
    if (!equals_sign)    // '=' yoksa hatalı format
        return (NULL);
    key = ft_substr(env_str, 0, equals_sign - env_str); // ADIM 2: Başlangıçtan '=' e kadar olan kısmı al
    return (key);
}

int	ft_parsing_env_entry(char *env_str, t_env **env_list)
{
	t_env	*new_node;
	char	*key;
	char	*value;

	key = ft_parse_env_key(env_str); // ADIM 1: Key'i çıkar (PATH=/usr/bin → "PATH")
	if (!key)
		return (0);
	value = ft_parse_env_value(env_str); // ADIM 2: Value'yu çıkar (PATH=/usr/bin → "/usr/bin") + !!Value kontrol edilmemeli çünkü "KEY=" geçerli format
	new_node = ft_create_env_node(key, value);
	ft_add_env_node(env_list, new_node);
	free(key);
	free(value);
	return (1);
}

t_env	*ft_init_env(char **envp)
{
	t_env	*env_list; //-> sonuç olarak linked list dönecek
	int		i;

	env_list = NULL;
	i = 0;
	while (envp[i]) // her environment string'i için
	{
		ft_parsing_env_entry(envp[i], &env_list);
		i++;
	}
	return (env_list);
}


/*----------------------------------------------------------------------------------------------*/


// void print_all_env(t_env *env_list)
// {
//     t_env *current = env_list;
    
//     while (current)
//     {
//         // Value NULL değilse yazdır (boş string olsa bile)
//         if (current->value != NULL)
//         {
//             printf("%s=%s\n", current->key, current->value);
//         }
//         current = current->next;
//     }
// }

// int main(int argc, char **argv, char **envp)
// {
//     t_env *env_list;
    
//     (void)argc;
//     (void)argv;
    
//     // Environment'ı initialize et
//     env_list = init_env(envp);
    
//     // Tüm environment variable'ları yazdır (tıpkı 'env' komutu gibi)
//     print_all_env(env_list);
    
//     return (0);
// }