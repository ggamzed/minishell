/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_word_split_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:20:25 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:20:26 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_skip_token_group(t_token **current)
{
	while ((*current)->next && (*current)->next->space_flag == 0)
		*current = (*current)->next;
	*current = (*current)->next;
}

int	ft_count_split_words(char **split_words)
{
	int	count;

	if (!split_words)
		return (1);
	count = 0;
	while (split_words[count])
		count++;
	return (count);
}
