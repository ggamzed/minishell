#include "../minishell.h"

char	*ft_handle_word(char *value, t_shell *shell)
{
	char	*tilde;

	tilde = ft_expand_tilde(value, shell);
	if (tilde)
		return (tilde);
	return (ft_strdup(value, shell));
}
