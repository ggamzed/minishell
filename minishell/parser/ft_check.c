/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:44 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:44 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_redirection(t_token_type type)
{
	if (type == REDIRECT_IN || type == REDIRECT_OUT
		|| type == REDIRECT_APPEND || type == HEREDOC)
		return (1);
	return (0);
}

int	ft_is_argument_token(t_token_type type)
{
	if (type == WORD || type == SINGLE_QUOTED_STRING
		|| type == DOUBLE_QUOTED_STRING || type == VARIABLE
		|| type == EXIT_STATUS)
		return (1);
	return (0);
}
