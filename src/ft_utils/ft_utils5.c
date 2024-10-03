/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 18:00:23 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_whitespace_and_sign(const char **str, int *sign)
{
	while (**str == ' ' || (**str >= '\t' && **str <= '\r'))
		(*str)++;
	if (**str == '-')
	{
		*sign = -1;
		(*str)++;
	}
	else if (**str == '+')
	{
		*sign = 1;
		(*str)++;
	}
	else
	{
		*sign = 1;
	}
	return (0);
}

int	get_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	return (-1);
}

long	handle_overflow(int sign, char **endptr, const char *str)
{
	if (endptr)
		*endptr = (char *)str;
	if (sign == 1)
		return (LONG_MAX);
	else
		return (LONG_MIN);
}

long	handle_digit_overflow(const char **current, char **endptr, int sign)
{
	if (endptr)
		*endptr = (char *)*current;
	return (handle_overflow(sign, endptr, *current));
}

long	ft_strtol(const char *str, char **endptr, int base)
{
	long		result;
	int			sign;
	const char	*current = str;
	int			digit;

	result = 0;
	sign = 1;
	if (base != 10)
		return (0);
	skip_whitespace_and_sign(&current, &sign);
	digit = get_digit(*current);
	while (digit >= 0)
	{
		if (result > (LONG_MAX - digit) / 10)
			return (handle_digit_overflow(&current, endptr, sign));
		result = result * 10 + digit;
		current++;
		digit = get_digit(*current);
	}
	if (endptr)
		*endptr = (char *)current;
	return (result * sign);
}
