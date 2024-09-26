/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: a. <a.@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 05:36:00 by aoshimiz          #+#    #+#             */
/*   Updated: 2024/09/27 01:10:01 by a.               ###   ########.fr       */
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

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if (s1[i] != s2[i])
		{
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		}
		i++;
	}
	return (0);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strncat(char *dest, const char *src, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	while (src[j] != '\0' && j < n)
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (src[i] != '\0')
	{
		i++;
	}
	if (size > 0)
	{
		j = 0;
		while (j < size - 1 && src[j] != '\0')
		{
			dest[j] = src[j];
			j++;
		}
		dest[j] = '\0';
	}
	return (i);
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
	// 終端の '\0' も検索対象に含めるため、ここでもチェックする
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

	// 複製する長さを決定 (s が n より短い場合は s の長さまで)
	len = strnlen(s, n);
	// メモリを確保 (+1 は終端文字のため)
	dup = malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	// s から len バイトを dup にコピー
	strncpy(dup, s, len);
	// 終端文字を追加
	dup[len] = '\0';
	return (dup);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	i;
	size_t	j;

	dst_len = 0;
	src_len = 0;
	// dst の長さを取得 (バッファサイズを超えないように)
	while (dst_len < dstsize && dst[dst_len] != '\0')
		dst_len++;
	// src の長さを取得
	while (src[src_len] != '\0')
		src_len++;
	// バッファが小さすぎる場合
	if (dstsize <= dst_len)
		return (dstsize + src_len);
	// src を dst の末尾にコピー
	i = dst_len;
	j = 0;
	while (src[j] != '\0' && i + 1 < dstsize)
	{
		dst[i] = src[j];
		i++;
		j++;
	}
	// 終端文字を追加
	dst[i] = '\0';
	// 最終的な長さを返す
	return (dst_len + src_len);
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

int	ft_at_eof(FILE *file)
{
	if (file == NULL)
		return (1);      // ファイルが NULL なら EOF とみなす
	return (feof(file)); // feof 関数を使って EOF かどうかを確認
}

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putstr(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (ft_putstr("(null)"));
	while (str[i] != '\0')
	{
		ft_putchar(str[i]);
		i++;
	}
	return (i);
}

int	ft_putnbr(int n)
{
	int	count;

	count = 0;
	if (n == -2147483648)
	{
		count += ft_putstr("-2147483648");
		return (count);
	}
	if (n < 0)
	{
		count += ft_putchar('-');
		n = -n;
	}
	if (n >= 10)
	{
		count += ft_putnbr(n / 10);
	}
	count += ft_putchar(n % 10 + '0');
	return (count);
}

int	ft_putnbr_unsigned(unsigned int n)
{
	int	count;

	count = 0;
	if (n >= 10)
	{
		count += ft_putnbr_unsigned(n / 10);
	}
	count += ft_putchar(n % 10 + '0');
	return (count);
}

int	ft_puthex(unsigned int n)
{
	int		count;
	char	*hex;

	count = 0;
	hex = "0123456789abcdef";
	if (n >= 16)
	{
		count += ft_puthex(n / 16);
	}
	count += ft_putchar(hex[n % 16]);
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		count;

	i = 0;
	count = 0;
	va_start(args, format);
	while (format[i] != '\0')
	{
		if (format[i] == '%' && format[i + 1])
		{
			i++;
			if (format[i] == 'c')
				count += ft_putchar(va_arg(args, int));
			else if (format[i] == 's')
				count += ft_putstr(va_arg(args, const char *));
			else if (format[i] == 'd' || format[i] == 'i')
				count += ft_putnbr(va_arg(args, int));
			else if (format[i] == 'u')
				count += ft_putnbr_unsigned(va_arg(args, unsigned int));
			else if (format[i] == 'x')
				count += ft_puthex(va_arg(args, unsigned int));
			else if (format[i] == '%')
				count += ft_putchar('%');
		}
		else
		{
			count += ft_putchar(format[i]);
		}
		i++;
	}
	va_end(args);
	return (count);
}

long	ft_strtol(const char *str, char **endptr, int base)
{
	long	result;
	int		sign;
		int digit;

	result = 0;
	sign = 1;
	// const char *start = str;
	// 空白をスキップ
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
	{
		str++;
	}
	// 符号を確認
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
	{
		str++;
	}
	// 基数が0の場合、自動検出
	if (base == 0)
	{
		if (*str == '0')
		{
			if (str[1] == 'x' || str[1] == 'X')
			{
				base = 16;
				str += 2;
			}
			else
			{
				base = 8;
				str++;
			}
		}
		else
		{
			base = 10;
		}
	}
	else if (base == 16 && *str == '0' && (str[1] == 'x' || str[1] == 'X'))
	{
		str += 2; // "0x" や "0X" のプレフィックスを無視
	}
	// 数値部分の変換
	while (*str != '\0')
	{
		if (*str >= '0' && *str <= '9')
		{
			digit = *str - '0';
		}
		else if (*str >= 'a' && *str <= 'z')
		{
			digit = *str - 'a' + 10;
		}
		else if (*str >= 'A' && *str <= 'Z')
		{
			digit = *str - 'A' + 10;
		}
		else
		{
			break ;
		}
		// digit が基数以上の場合は変換終了
		if (digit >= base)
		{
			break ;
		}
		// オーバーフローのチェック
		if (result > (LONG_MAX - digit) / base)
		{
			result = (sign == 1) ? LONG_MAX : LONG_MIN;
			if (endptr != NULL)
			{
				*endptr = (char *)str;
			}
			return (result);
		}
		result = result * base + digit;
		str++;
	}
	// 結果の符号を考慮
	result *= sign;
	// 変換に成功した場所を設定
	if (endptr != NULL)
	{
		*endptr = (char *)str;
	}
	return (result);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*p1 = (const unsigned char *)s1;
	const unsigned char	*p2 = (const unsigned char *)s2;

	while (n--)
	{
		if (*p1 != *p2)
		{
			return (*p1 - *p2);
		}
		p1++;
		p2++;
	}
	return (0);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char *d = (unsigned char *)dst;
	const unsigned char *s = (const unsigned char *)src;

	if (d == NULL && s == NULL)
		return (NULL);

	while (n--)
	{
		*d++ = *s++;
	}
	return (dst);
}