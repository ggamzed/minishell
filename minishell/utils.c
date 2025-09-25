#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

int	ft_is_space(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

void	ft_putstr_fd(char *s, int fd)
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

char	*ft_substr(char const *s, unsigned int start, size_t len, t_shell *shell)
{
	char	*substr;
	size_t	i;
	size_t	s_len;

	if (!s)
		return (NULL);
	if (len == 0)
        return (ft_strdup("", shell));
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup("", shell));
	if (len > s_len - start)
		len = s_len - start;
	substr = ft_malloc(len + 1, shell);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

int	ft_strlen(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strdup(const char *s, t_shell *shell)
{
	char	*dup;
	int		len;
	int		i;

	len = ft_strlen(s);
	dup = NULL;
	dup = ft_malloc(len + 1, shell);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	*ft_malloc(size_t size, t_shell *shell)
{
	void	*ptr;
	t_mem	*mem_node;

	ptr = malloc(size);
	if (!ptr)
	{
		ft_putstr_fd("malloc failed", 2);
		return (NULL);
	}
	if (!shell || !shell->mem_tracker)
        return (ptr);
	mem_node = malloc(sizeof(t_mem));
	if (!mem_node)
	{
		ft_putstr_fd("malloc for memory tracker failed", 2);
		free(ptr);
		return (NULL);
	}
	mem_node->ptr = ptr;
	mem_node->next = *shell->mem_tracker;
	*shell->mem_tracker = mem_node;
	return (ptr);
}

void	ft_free_mem_tracker(t_mem **mem_tracker)
{
	t_mem	*curr;
	t_mem	*tmp;
	
	if (!mem_tracker)
		return ;
	curr = *mem_tracker;
	while (curr)
	{
		tmp = curr->next;
		if (curr->ptr)
		{
			free(curr->ptr);
			curr->ptr = NULL;
		}
		free(curr);
		curr = tmp;
	}
	*mem_tracker = NULL;
}

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

char	*ft_itoa(int n, t_shell *shell)
{
	char	*str;
	int		len;
	int		temp;
	int		is_negative;

	is_negative = (n < 0);
	temp = n;
	len = (n == 0) ? 1 : 0;
	while (temp != 0)
	{
		temp /= 10;
		len++;
	}
	if (is_negative)
		len++;
	str = ft_malloc(len + 1, shell);
	str[len] = '\0';
	if (n == 0)
		str[0] = '0';
	else
	{
		if (is_negative)
		{
			str[0] = '-';
			n = -n;
		}
		while (n > 0)
		{
			str[--len] = (n % 10) + '0';
			n /= 10;
		}
	}
	return (str);
}

char	*ft_strjoin(char const *s1, char const *s2, t_shell *shell)
{
	char	*joined;
	int		len1;
	int		len2;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = ft_malloc(len1 + len2 + 1, shell);
	i = 0;
	while (i < len1)
	{
		joined[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < len2)
	{
		joined[i + j] = s2[j];
		j++;
	}
	joined[i + j] = '\0';
	return (joined);
}
int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
			|| (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strjoin_free(char *s1, char *s2, t_shell *shell)
{
	char	*result;

	if (!s1 || !s2)
	{
		if (s1)
			free(s1);
		if (s2)
			free(s2);
		return (NULL);
	}
	result = ft_strjoin(s1, s2, shell);
	return (result);
}

char	*ft_strchr(const char *s, int c)
{
	char	ch;

	ch = (char)c;
	while (*s)
	{
		if (*s == ch)
			return ((char *)s);
		s++;
	}
	if (ch == '\0')
		return ((char *)s);
	return (NULL);
}

int	ft_is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;

	// Boşlukları atla (whitespace karakterleri)
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;

	// İşaret kontrolü
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}

	// Rakamları işle
	while (ft_is_digit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}

	return (result * sign);
}



int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (!s1 || !s2 || n == 0)
		return (0);
	i = 0;
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

//-----------------------SPLIT--------------------

char	**malloc_error(char **arr, size_t i)
{
	while (arr[i])
		free(arr[i++]);
	return (free(arr), NULL);
}

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
	res = (char **)ft_malloc(sizeof(char *) * (word_count(s, c) + 1), shell); // freelenen malloc kullan
	if (!s || !res)
		return (NULL);
	while (++a < word_count(s, c))
	{
		while (s[i] && s[i] == c)
			i++;
		res[a] = ft_substr(s, i, word_len(&s[i], c), shell);
		if (!res[a])
			return (malloc_error(res, 0));
		i += word_len(&s[i], c);
	}
	return (res[a] = NULL, res);
}

//-----------------------------------------------