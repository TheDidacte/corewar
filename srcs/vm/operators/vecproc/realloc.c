/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 01:16:55 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/07 21:03:32 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vm.h"

t_vecproc	*vecproc_realloc(t_garbage *gb, t_vecproc *vec)
{
	if (__builtin_expect(vec->mem == 0, 0))
		vec->mem = 2;
	else
		vec->mem *= 2;
	vec->d = gb_realloc(gb, vec->d, vec->mem * sizeof(*vec->d));
	return (vec);
}
