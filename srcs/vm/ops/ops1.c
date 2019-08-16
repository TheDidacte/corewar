/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 17:39:38 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/24 17:40:09 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
**	The ops are cut into separate files because of 42s dumb norme.
*/

void			op_live(t_vm *vm, int proc, int reg_num[3])
{
	int			i;
	int			player;
	int			fail;

	player = vm->procs.d[proc].reg[reg_num[0]];
	i = 0;
	fail = 0;
	while (i < vm->players.len && vm->players.d[i].id != player)
		if (++i == vm->players.len)
			fail = 1;
	if (!fail)
	{
		vm->players.d[i].period_lives += 1;
		vm->players.d[i].last_live = vm->cycle_curr;
		if (vm->verbosity >= VE_OPS)
			ft_printf("\n");
		if (vm->verbosity >= VE_LIVE)
			ft_printf("A process shows that player %d (%s) is alive%s",
				vm->players.d[i].id, vm->players.d[i].head.prog_name,
				(vm->verbosity < VE_OPS) ? "\n" : "");
	}
	vm->procs.d[proc].live++;
}

void			op_ld(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].carry = vm->procs.d[proc].reg[reg_num[0]] ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[1]] = vm->procs.d[proc].reg[reg_num[0]];
}

void			op_st(t_vm *vm, int proc, int reg_num[3])
{
	int			addr;
	int			pc;

	pc = vm->procs.d[proc].pc;
	if (((vm->arena[circumem(pc + 1)] >> 4) & 0b11) == IND_CODE)
	{
		addr = pc + (load16(vm, pc + 3) % IDX_MOD);
		write32(vm, pc, addr, vm->procs.d[proc].reg[reg_num[0]]);
	}
	else
		vm->procs.d[proc].reg[reg_num[1]] = vm->procs.d[proc].reg[reg_num[0]];
}

void			op_add(t_vm *vm, int proc, int reg_num[3])
{
	int			res;

	res = vm->procs.d[proc].reg[reg_num[0]] + vm->procs.d[proc].reg[reg_num[1]];
	vm->procs.d[proc].carry = res ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = res;
}

void			op_sub(t_vm *vm, int proc, int reg_num[3])
{
	int			res;

	res = vm->procs.d[proc].reg[reg_num[0]] - vm->procs.d[proc].reg[reg_num[1]];
	vm->procs.d[proc].carry = res ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = res;
}
