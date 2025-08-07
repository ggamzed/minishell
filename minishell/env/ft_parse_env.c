#include "../minishell.h"

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
