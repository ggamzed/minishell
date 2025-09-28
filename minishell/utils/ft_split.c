/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:23 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:21:24 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	word_count(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			while (*s && *s != c)
				s++;
			i++;
		}
	}
	return (i);
}

static int	word_len(char const *s, char c)
{
	int	len;

	len = 0;
	if (!*s)
		return (0);
	while (*s && *s++ != c)
		len++;
	return (len);
}

char	**ft_split(char const *s, char c, t_shell *shell)
{
	char	**res;
	int		a;
	int		i;

	a = -1;
	i = 0;
	res = (char **)ft_malloc(sizeof(char *) * (word_count(s, c) + 1), shell);
	if (!s || !res)
		ft_cleanup_and_exit(shell, 1);
	while (++a < word_count(s, c))
	{
		while (s[i] && s[i] == c)
			i++;
		res[a] = ft_substr(s, i, word_len(&s[i], c), shell);
		if (!res[a])
			ft_cleanup_and_exit(shell, 1);
		i += word_len(&s[i], c);
	}
	return (res[a] = NULL, res);
}
