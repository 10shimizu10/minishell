/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/28 13:54:41 by a.               ###   ########.fr       */
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
		(*str)++;
	return (0);
}

int	detect_base(const char **str, int base)
{
	if (base == 0)
	{
		if (**str == '0')
		{
			if ((*str)[1] == 'x' || (*str)[1] == 'X')
			{
				base = 16;
				*str += 2;
			}
			else
			{
				base = 8;
				(*str)++;
			}
		}
		else
			base = 10;
	}
	else if (base == 16 && **str == '0' && ((*str)[1] == 'x'
			|| (*str)[1] == 'X'))
		*str += 2;
	return (base);
}

int	get_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'z')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'Z')
		return (c - 'A' + 10);
	return (-1);
}

long	ft_strtol(const char *str, char **endptr, int base)
{
	long	result;
	int		sign;
	int		digit;

	result = 0;
	sign = 1;
	skip_whitespace_and_sign(&str, &sign);
	base = detect_base(&str, base);
	while ((digit = get_digit(*str)) >= 0 && digit < base)
	{
		if (result > (LONG_MAX - digit) / base)
		{
			result = (sign == 1) ? LONG_MAX : LONG_MIN;
			if (endptr)
				*endptr = (char *)str;
			return (result);
		}
		result = result * base + digit;
		str++;
	}
	if (endptr)
		*endptr = (char *)str;
	return (result * sign);
}
