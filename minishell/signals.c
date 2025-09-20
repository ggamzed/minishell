#include "minishell.h"

/* ctrl+c */
void ft_handle_sigint(int sig)
{
	g_signal = sig;              // Global değişkene signal numarasını kaydet (2)
	//write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();            // readline'a yeni satıra geçtiğimizi söyle
	rl_replace_line("", 0);      // Mevcut satırı boş string ile değiştir
	rl_redisplay();              // Prompt'u yeniden göster (minishell$ )
	rl_done = 1;
}

/* ctrl+\ */
void ft_handle_sigquit(int sig)
{
	(void)sig; //SIGQUIT için hiçbir şey yapma
}

/*
 * Interactive Mode Signal Setup
 * Minishell kullanıcı ile etkileşim halindeyken kullanılan ayarlar
 */
void ft_setup_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    memset(&sa_int, 0, sizeof(sa_int));
    memset(&sa_quit, 0, sizeof(sa_quit));

    sa_int.sa_handler = ft_handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;  // SA_RESTART istersen koyabilirsin
    sigaction(SIGINT, &sa_int, NULL);

    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}


/*
 * Child Process'ler İçin Signal Ignore
 * Fork edilen child process'lerde signal'leri görmezden gel
 */
void ft_ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/*
 * Default Signal Behavior'a Dön
 * Execve öncesi child process'lerde default davranışa döndür
 */
void ft_default_signals(void)
{
	signal(SIGINT, SIG_DFL);   // SIGINT için default davranış (process'i öldür)
	signal(SIGQUIT, SIG_DFL);  // SIGQUIT için default davranış (process'i öldür + core dump)
}
