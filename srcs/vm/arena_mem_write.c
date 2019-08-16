/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_mem_write.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 13:38:45 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/25 14:27:59 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "vm.h"

uint8_t			*write32(t_vm *vm, int pc, int aptr, uint32_t data)
{
	uint8_t		*r;
	int			i;
	uint8_t		new_owner;
	int			tmp;

	r = (uint8_t*)&data;
	new_owner = vm->mem_owner[pc];
	new_owner += new_owner >= FRESH0_COLOR ? 0 : DELT_FRESH_COLOR;
	i = -1;
	while (++i < 4)
	{
		tmp = circumem(aptr + (3 - i));
		vm->arena[tmp] = r[i];
		if (vm->verbosity == VE_VISU)
		{
			vm->mem_owner[tmp] = new_owner;
			vm->time_write[tmp] = clock();
		}
	}
	return (&vm->arena[aptr]);
}

uint8_t			*write16(t_vm *vm, int pc, int aptr, uint16_t data)
{
	uint8_t		new_owner;
	int			aptr1;

	aptr = circumem(aptr);
	aptr1 = circumem(aptr + 1);
	vm->arena[aptr1] = (uint8_t)data;
	vm->arena[aptr] = (uint8_t)(data >> 8);
	if (vm->verbosity == VE_VISU)
	{
		new_owner = vm->mem_owner[pc];
		new_owner += new_owner >= FRESH0_COLOR ? 0 : DELT_FRESH_COLOR;
		vm->mem_owner[aptr1] = new_owner;
		vm->mem_owner[aptr] = new_owner;
		vm->time_write[aptr1] = clock();
		vm->time_write[aptr] = clock();
	}
	return (&vm->arena[aptr]);
}

uint8_t			*write8(t_vm *vm, int pc, int aptr, uint8_t data)
{
	uint8_t		new_owner;

	aptr = circumem(aptr);
	vm->arena[aptr] = data;
	if (vm->verbosity == VE_VISU)
	{
		vm->time_write[aptr] = clock();
		new_owner = vm->mem_owner[pc];
		new_owner += new_owner >= FRESH0_COLOR ? 0 : DELT_FRESH_COLOR;
		vm->mem_owner[aptr] = new_owner;
	}
	return (&vm->arena[aptr]);
}
