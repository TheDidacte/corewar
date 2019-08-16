/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_mem_load.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 20:56:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/22 20:32:33 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"

int32_t			load32(t_vm *vm, t_register pc)
{
	int32_t		res;
	uint8_t		*r;
	int			i;

	r = (uint8_t*)&res;
	i = -1;
	while (++i < 4)
		r[i] = vm->arena[circumem(pc + (3 - i))];
	return (res);
}

int16_t			load16(t_vm *vm, t_register pc)
{
	int16_t		res;
	uint8_t		*r;

	r = (uint8_t*)&res;
	r[0] = vm->arena[circumem(pc + 1)];
	r[1] = vm->arena[circumem(pc)];
	return (res);
}

int8_t			load8(t_vm *vm, t_register pc)
{
	int8_t		res;

	res = (int8_t)vm->arena[circumem(pc)];
	return (res);
}
