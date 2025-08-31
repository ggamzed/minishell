#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

void ft_env(char **envp)
{
    int i = 0;
    while (envp[i])
    {
        printf("%s\n", envp[i]);
        i++;
    }
}

int main(int argc, char **argv, char **envp)
{
    char *input;

    while (1)
    {
        input = readline("minishell$ ");  // prompt veriyoruz
        if (!input)  // Ctrl+D (EOF)
        {
            printf("exit\n");
            break;
        }

        if (*input)  // boş satır değilse history'ye ekle
            add_history(input);

        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }
        else if (strcmp(input, "env") == 0)
            ft_env(envp);
        else
            printf("Komut bulunamadı: %s\n", input);

        free(input);
    }

    return 0;
}



/*
gcc env_list.c -o env_list -lreadline
-lreadline → readline kütüphanesini bağlar.

readline() ve add_history() standart C kütüphanesinde yok.

Bu fonksiyonları kullanmak için -lreadline ile linklemek gerekiyor.
*/

/*
add_history() fonksiyonu readline kütüphanesine ait bir fonksiyon.

Yani readline kullanmadan add_history() çalıştıramazsın, çünkü:

History mekanizması readline’in iç yapısında tutuluyor.

add_history() aslında yazdığın satırı readline’in internal history listesine ekliyor.

add_history kullanmadan readline() kullanbilirsin.
*/