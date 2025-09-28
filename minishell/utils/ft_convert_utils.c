/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egokce <egokce@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 09:21:13 by egokce            #+#    #+#             */
/*   Updated: 2025/09/28 09:21:14 by egokce           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static unsigned int	ft_num_len(long n)
{
	int	len;

	len = 1;
	if (n < 0)
	{
		len++;
		n *= -1;
	}
	while (n > 9)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n, t_shell *shell)
{
	char	*str;
	size_t	len;
	long	long_n;

	long_n = (long)n;
	len = ft_num_len(long_n);
	str = ft_malloc((len + 1) * sizeof(char), shell);
	if (!str)
		ft_cleanup_and_exit(shell, 1);
	if (long_n < 0)
	{
		long_n *= -1;
		str[0] = '-';
	}
	if (long_n == 0)
		str[0] = '0';
	str[len] = '\0';
	while (long_n > 0)
	{
		str[--len] = (long_n % 10) + '0';
		long_n /= 10;
	}
	return (str);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_is_digit(str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
