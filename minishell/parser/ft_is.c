/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <eecegokcece@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 18:22:06 by egokce            #+#    #+#             */
/*   Updated: 2025/09/23 18:22:06 by egokce           ###   ########.fr       */
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
