#include "../minishell.h"

t_token	*ft_create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = ft_malloc(sizeof(t_token));
	token->type = type;
	token->value = value; // value'yu direkt ata, kopyalama
	token->next = NULL;
	return (token);
}

void	ft_add_token(t_token **token_list, t_token *new_token)
{
	t_token	*current;

	if (!*token_list)
	{
		*token_list = new_token;
		return;
	}
	current = *token_list;
	while (current->next)
		current = current->next;
	current->next = new_token;
}
