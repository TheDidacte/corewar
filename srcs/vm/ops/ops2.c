/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 17:40:27 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/24 17:40:29 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void			op_and(t_vm *vm, int proc, int reg_num[3])
{
	int			res;

	res = vm->procs.d[proc].reg[reg_num[0]] & vm->procs.d[proc].reg[reg_num[1]];
	vm->procs.d[proc].carry = res ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = res;
}

void			op_or(t_vm *vm, int proc, int reg_num[3])
{
	int			res;

	res = vm->procs.d[proc].reg[reg_num[0]] | vm->procs.d[proc].reg[reg_num[1]];
	vm->procs.d[proc].carry = res ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = res;
}

void			op_xor(t_vm *vm, int proc, int reg_num[3])
{
	t_register	res;

	res = vm->procs.d[proc].reg[reg_num[0]] ^ vm->procs.d[proc].reg[reg_num[1]];
	vm->procs.d[proc].carry = res ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[2]] = res;
}

void			op_zjmp(t_vm *vm, int proc, int reg_num[3])
{
	if (vm->procs.d[proc].carry)
	{
		vm->procs.d[proc].new_pc = circumem(
				vm->procs.d[proc].pc
				+ (vm->procs.d[proc].reg[reg_num[0]] % IDX_MOD));
		if (vm->verbosity >= VE_OPS)
			ft_printf(" OK");
	}
	else if (vm->verbosity >= VE_OPS)
		ft_printf(" FAILED");
}

void			op_ldi(t_vm *vm, int proc, int reg_num[3])
{
	t_register	addr_rel;
	t_register	data;

	addr_rel = (vm->procs.d[proc].reg[reg_num[0]]
			+ vm->procs.d[proc].reg[reg_num[1]]) % IDX_MOD;
	data = load32(vm, vm->procs.d[proc].pc + addr_rel);
	vm->procs.d[proc].reg[reg_num[2]] = data;
}
