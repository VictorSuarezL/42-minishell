/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanz-su <vsanz-su@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 13:36:00 by vsanz-su          #+#    #+#             */
/*   Updated: 2023/11/03 17:45:03 by vsanz-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

size_t	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

void	ft_putchar(char c, int *n_chr)
{
	*n_chr += write(1, &c, 1);
}

void	ft_putstr(char *s, int *n_chr)
{
	int	i;

	i = 0;
	if (!s)
	{
		return (ft_putstr("(null)", n_chr));
	}
	while (s[i])
	{
		ft_putchar(s[i], n_chr);
		i++;
	}
}

void	ft_put_nbr_base(char *str, long long int nbr, int *n_chr)
{
	int	size;

	size = ft_strlen(str);
	if (nbr < 0)
	{
		ft_putchar('-', n_chr);
		nbr = -nbr;
	}
	if (nbr > 0)
	{
		ft_put_nbr_base(str, nbr / size, n_chr);
		*n_chr += write(1, str + nbr % size, 1);
	}
}
