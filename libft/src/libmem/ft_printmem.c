/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printmem.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 18:42:50 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/26 20:10:26 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_printmem(const void *mem, ssize_t len, int dump_width)
{
	const uint8_t	*ptr;
	ssize_t			size;
	int				i;
	char			*out;

	out = NULL;
	size = len;
	ptr = mem;
	ft_printf("len: %ld\n", len);
	while (size > 0)
	{
		ft_asprintf(&out, "0x%04hx : ", len - size);
		i = -1;
		while (++i < dump_width && i < size)
			ft_strcat_join(&out, ft_cprintf("%02hhx ", ptr[i]));
		while (i++ < dump_width)
			ft_strcat_join(&out, ft_cprintf("   "));
		ft_printf("%s\n", out);
		ft_memfree(&out);
		size -= dump_width;
		ptr += dump_width;
	}
}