/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dump_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:12:50 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 17:46:40 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "corewar.h"
#include "libft.h"
#include "vm.h"

/*
**	Using ft_strcat_join is a bit extreme here as we could pre-calculate the
**		lenght of the string and do a single malloc().
**	But it works, it's safe and it's fast enough.
*/

static char	*strcat_free(char *s1, char *s2)
{
	char		*new_str;
	size_t		len_s2;
	size_t		len_s1;

	new_str = NULL;
	if (!s1 && s2)
		new_str = s2;
	else if (s1 && s2)
	{
		len_s1 = ft_strlen(s1);
		len_s2 = ft_strlen(s2);
		if (!(new_str = ft_strnew(len_s1 + len_s2)))
			return (NULL);
		ft_mempcpy(ft_mempcpy(new_str, s1, len_s1), s2, len_s2 + 1);
		free(s1);
		free(s2);
	}
	return (new_str);
}

void		dump_memory(t_vm *vm)
{
	const uint8_t	*ptr;
	int				size;
	int				i;
	char			*out;

	if (vm->dump_width <= 0)
		vm->dump_width = 64;
	out = NULL;
	size = MEM_SIZE;
	ptr = vm->arena;
	while (size > 0)
	{
		ft_asprintf(&out, "0x%04hx : ", ptr - vm->arena);
		i = -1;
		while (++i < vm->dump_width && i < size)
			out = strcat_free(out, ft_cprintf("%02hhx ", ptr[i]));
		while (i++ < vm->dump_width)
			out = strcat_free(out, ft_cprintf("   "));
		ft_printf("%s\n", out);
		ft_memfree(&out);
		size -= vm->dump_width;
		ptr += vm->dump_width;
	}
}
