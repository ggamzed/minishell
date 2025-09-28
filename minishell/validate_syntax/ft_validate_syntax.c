/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_validate_syntax.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:38 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:21:38 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_validate_syntax(char *line)
{
	if (ft_validate_quotes(line) == 1)
		return (1);
	if (ft_validate_pipes(line) == 1)
		return (1);
	if (ft_validate_redirections(line) == 1)
		return (1);
	return (0);
}
