/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_var.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:15 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:16 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_handle_word(char *value, t_shell *shell)
{
	char	*tilde;

	tilde = ft_expand_tilde(value, shell);
	if (tilde)
		return (tilde);
	return (ft_strdup(value, shell));
}
