/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdemirci <gdemirci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:27 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 20:44:21 by gdemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_substr(char const *s,
			unsigned int start, size_t len, t_shell *shell)
{
	char	*substr;
	size_t	i;
	size_t	s_len;

	if (!s)
		ft_cleanup_and_exit(shell, 1);
	if (len == 0)
		return (ft_strdup("", shell));
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup("", shell));
	if (len > s_len - start)
		len = s_len - start;
	substr = ft_malloc(len + 1, shell);
	if (!substr)
		ft_cleanup_and_exit(shell, 1);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	*ft_strdup(const char *s, t_shell *shell)
{
	char	*dup;
	int		len;
	int		i;

	if (!s)
		ft_cleanup_and_exit(shell, 1);
	len = ft_strlen(s);
	dup = ft_malloc(len + 1, shell);
	if (!dup)
		ft_cleanup_and_exit(shell, 1);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strjoin(char const *s1, char const *s2, t_shell *shell)
{
	char	*joined;
	int		i;
	int		j;

	if (!s1 || !s2)
		ft_cleanup_and_exit(shell, 1);
	joined = ft_malloc(ft_strlen(s1) + ft_strlen(s2) + 1, shell);
	if (!joined)
		ft_cleanup_and_exit(shell, 1);
	i = 0;
	while (i < ft_strlen(s1))
	{
		joined[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < ft_strlen(s2))
	{
		joined[i + j] = s2[j];
		j++;
	}
	joined[i + j] = '\0';
	return (joined);
}

static size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	unsigned long		i;
	size_t				sizesrc;

	i = 0;
	if (dstsize == 0)
		return (ft_strlen(src));
	else if (dstsize != 0)
	{
		while ((src[i] != '\0') && (i < dstsize - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	sizesrc = ft_strlen(src);
	return (sizesrc);
}

char	*ft_strtrim(char const *s1, t_shell *shell)
{
	char	*str;
	size_t	start;
	size_t	end;

	if (!s1)
		ft_cleanup_and_exit(shell, 1);
	start = 0;
	while (s1[start] && ft_is_white_space(s1[start]))
		start++;
	end = ft_strlen(s1);
	while (end > start && ft_is_white_space(s1[end - 1]))
		end--;
	str = ft_malloc(((end - start + 1) * sizeof(char)), shell);
	if (!str)
		ft_cleanup_and_exit(shell, 1);
	ft_strlcpy(str, &s1[start], end - start + 1);
	str[end - start] = '\0';
	return (str);
}
