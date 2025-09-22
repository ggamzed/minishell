#include "../minishell.h"

static char    *ft_get_cd_path(char **argv, t_env *env_list)
{
    char    *home;
    int        arg_count = 0;

    while (argv[arg_count])
        arg_count++;
    if (arg_count > 2)
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", 2);
        return (NULL);
    }

    if (!argv[1]) // argüman kontrolü - yoksa HOME'a git
    {
        home = ft_get_env_value("HOME", env_list);
        if (!home)
        {
            ft_putstr_fd("minishell: cd: HOME not set", 2);
            return (NULL);
        }
        return (home);
    }
    return (argv[1]); // argüman varsa onu kullan
}

//argüman verilmezse HOME dizinine gider + PWD ve OLDPWD environment değişkenlerini günceller
// int ft_builtin_cd(char **argv, t_env *env_list, t_shell *shell)
// {
//     char *path;
// 	char *new_pwd;
//     char old_cwd[PATH_MAX];
//     char new_cwd[PATH_MAX];
//     char resolved_path[PATH_MAX];

//     path = ft_get_cd_path(argv, env_list);
//     if (!path)
//         return (1);

//     // Path'i resolve et çünkü .. zaten var görüyor
//     if (realpath(path, resolved_path) == NULL)
//     {
//         ft_putstr_fd("minishell: cd: ", 2);
//         ft_putstr_fd(path, 2);
//         ft_putstr_fd(": No such file or directory\n", 2);
//         return (1);
//     }

//     // Resolved path'in varlığını kontrol et
//     if (access(resolved_path, F_OK) != 0)
//     {
//         ft_putstr_fd("minishell: cd: ", 2);
//         ft_putstr_fd(path, 2);
//         ft_putstr_fd(": No such file or directory\n", 2);
//         return (1);
//     }
//     if (getcwd(old_cwd, sizeof(old_cwd)))
//         ft_set_env_value("OLDPWD", old_cwd, &env_list, shell);
//     if (chdir(path) != 0)
//     {
//         ft_putstr_fd("minishell: cd: ", 2);
//         ft_putstr_fd(path, 2);
//         ft_putstr_fd(": ", 2);
//         perror("");
//         return (1);
//     }
//     if (getcwd(new_cwd, sizeof(new_cwd)))
//         ft_set_env_value("PWD", new_cwd, &env_list, shell);
//     return (0);
// }


int ft_builtin_cd(char **argv, t_env *env_list, t_shell *shell)
{
    char *path;
    char old_cwd[PATH_MAX];
    char new_cwd[PATH_MAX];
    char resolved_path[PATH_MAX];

    path = ft_get_cd_path(argv, env_list);
    if (!path)
        return (1);

    // Path'i resolve et
    if (realpath(path, resolved_path) == NULL)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        return (1);
    }

    // Resolved path'in varlığını kontrol et
    if (access(resolved_path, F_OK) != 0)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        return (1);
    }
    
    // Eski dizini kaydet
    if (getcwd(old_cwd, sizeof(old_cwd)))
        ft_set_env_value("OLDPWD", old_cwd, &env_list, shell);
    
    // Directory değiştir
    if (chdir(resolved_path) != 0)  // resolved_path kullan
    {
        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": ", 2);
        perror("");
        return (1);
    }
    
    // Yeni PWD'yi güncelle
    if (getcwd(new_cwd, sizeof(new_cwd)))
        ft_set_env_value("PWD", new_cwd, &env_list, shell);
    
    return (0);
}
