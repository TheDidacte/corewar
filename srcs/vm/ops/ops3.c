/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 17:40:43 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 17:39:40 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void			op_sti(t_vm *vm, int proc, int reg_num[3])
{
	int			addr;
	t_proc		*p;

	p = &vm->procs.d[proc];
	addr = p->pc + (p->reg[reg_num[1]] + p->reg[reg_num[2]]) % IDX_MOD;
	write32(vm, p->pc, addr, p->reg[reg_num[0]]);
}

void			op_fork(t_vm *vm, int proc, int reg_num[3])
{
	t_proc		*parent;
	t_proc		*child;

	vecproc_push(&vm->gb, &vm->procs, proc);
	parent = &vm->procs.d[proc];
	child = &vm->procs.d[vm->procs.len - 1];
	child->pc = circumem(parent->pc + (parent->reg[reg_num[0]] % IDX_MOD));
	child->new_pc = 1;
	if (vm->verbosity >= VE_OPS)
		ft_printf(" (%d)", child->pc);
	read_instruction(vm, vm->procs.len - 1);
}

void			op_lld(t_vm *vm, int proc, int reg_num[3])
{
	vm->procs.d[proc].carry = vm->procs.d[proc].reg[reg_num[0]] ? 0 : 1;
	vm->procs.d[proc].reg[reg_num[1]] = vm->procs.d[proc].reg[reg_num[0]];
}

void			op_lldi(t_vm *vm, int proc, int reg_num[3])
{
	int32_t		res;
	t_proc		*p;

	p = &vm->procs.d[proc];
	res = load32(vm, p->pc + (p->reg[reg_num[0]] + p->reg[reg_num[1]]));
	p->carry = res ? 0 : 1;
	p->reg[reg_num[2]] = res;
}

void			op_lfork(t_vm *vm, int proc, int reg_num[3])
{
	t_proc		*parent;
	t_proc		*child;

	vecproc_push(&vm->gb, &vm->procs, proc);
	parent = &vm->procs.d[proc];
	child = &vm->procs.d[vm->procs.len - 1];
	child->pc = circumem(parent->pc + parent->reg[reg_num[0]]);
	child->new_pc = 1;
	if (vm->verbosity >= VE_OPS)
		ft_printf(" (%d)", child->pc);
	read_instruction(vm, vm->procs.len - 1);
}
