/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops4.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 17:41:03 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/24 17:41:12 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void			op_aff(t_vm *vm, int proc, int reg_num[3])
{
	uint8_t	c;
	int		cursor;

	c = vm->procs.d[proc].reg[reg_num[0]] % 256;
	if (vm->verbosity >= VE_AFF)
		ft_printf("{grn}aff: %c{eoc}\n", c);
	else if (vm->verbosity == VE_VISU)
	{
		cursor = ft_strlen(vm->visu.aff);
		if (cursor >= AFF_BUFF_LEN - 1)
		{
			ft_bzero(vm->visu.aff, AFF_BUFF_LEN);
			cursor = 0;
		}
		vm->visu.aff[cursor] = c;
		vm->visu.aff[cursor + 1] = ' ';
	}
}
