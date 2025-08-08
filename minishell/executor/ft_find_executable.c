#include "../minishell.h"

//komutun tam yol ile mi verildiğini kontrol ediyoruz. (/ karakteri varlığından) -> (örnek /bin/ls tam yol, ama sadece ls verildiyse path araması yapılması gerek)
static char	*ft_check_direct_path(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0) // access -> executor.txt
			return (ft_strdup(cmd)); //strdup kullanmasaydık en son ft_find_executable da result free'lenmeye çalışırken orjinal cmd'yi free'lemeye çalışırdı
		return (NULL);
	}
	return (cmd); // Devam etmek için cmd'i geri döndür
}

// PATH dizinlerinde komut arama
static char	*ft_search_in_paths(char *cmd, char **paths)
{
	char	*temp;
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
		{
			ft_free_split(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*ft_find_executable(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*result;

	result = ft_check_direct_path(cmd);
	if (result != cmd)
		return (result);
	path_env = ft_get_env_value("PATH", env_list); // PATH environment'tan yol listesini al
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':'); // PATH'i ':' ile böl
	if (!paths)
		return (NULL);
	result = ft_search_in_paths(cmd, paths); // PATH dizinlerinde ara
	// if (!result)
	// 	ft_free_split(paths);
	return (result);
}
