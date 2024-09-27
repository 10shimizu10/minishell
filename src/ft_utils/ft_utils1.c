/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 17:24:43 by a.               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *str)
{
	size_t	length;

	length = 0;
	while (str[length] != '\0')
	{
		length++;
	}
	return (length);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c)
		{
			return ((char *)s);
		}
		s++;
	}
	if (*s == (char)c)
	{
		return ((char *)s);
	}
	return (NULL);
}

char	*ft_strdup(const char *s1)
{
	size_t	i;
	char	*dup;

	i = 0;
	while (s1[i] != '\0')
	{
		i++;
	}
	dup = (char *)malloc(sizeof(char) * (i + 1));
	if (!dup)
	{
		return (NULL);
	}
	for (size_t j = 0; j <= i; j++)
	{
		dup[j] = s1[j];
	}
	return (dup);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	len;

	len = strnlen(s, n);
	dup = malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	ft_strncpy(dup, s, len);
	dup[len] = '\0';
	return (dup);
}

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		ptr[i] = (unsigned char)c;
		i++;
	}
	return (b);
}
