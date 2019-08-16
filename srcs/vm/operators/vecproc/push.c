/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 01:13:56 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 21:33:09 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

t_vecproc		*vecproc_push(t_garbage *gb, t_vecproc *vec, uint src)
{
	if (vec->len == vec->mem)
		vecproc_realloc(gb, vec);
	ft_memcpy(&vec->d[vec->len], &vec->d[src], sizeof(t_proc));
	vec->d[vec->len].reg = gb_malloc(gb, (OUR_REG_NB) * sizeof(t_register));
	ft_memcpy(vec->d[vec->len].reg,
		vec->d[src].reg,
		OUR_REG_NB * sizeof(t_register));
	vec->last_pid++;
	vec->d[vec->len].pid = vec->last_pid;
	++vec->len;
	return (vec);
}
