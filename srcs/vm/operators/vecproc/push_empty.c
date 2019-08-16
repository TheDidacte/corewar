/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_empty.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 18:35:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 21:32:53 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_vecproc		*vecproc_push_empty(t_garbage *gb, t_vecproc *vec)
{
	if (vec->len == vec->mem)
		vecproc_realloc(gb, vec);
	ft_bzero(&vec->d[vec->len], sizeof(*vec->d));
	vec->d[vec->len].reg = gb_memalloc(gb, (OUR_REG_NB) * sizeof(t_register));
	vec->last_pid++;
	vec->d[vec->len].pid = vec->last_pid;
	++vec->len;
	return (vec);
}
