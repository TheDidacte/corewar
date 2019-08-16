/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_arg_into_regs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 22:52:46 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/23 23:02:45 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static t_bool	load_cb(t_vm *vm, t_proc *proc, int reg_num[3])
{
	int		i;
	uint8_t	cb;
	t_bool	fail;

	fail = FALSE;
	i = 0;
	cb = vm->arena[(proc->pc + 1) % MEM_SIZE];
	proc->new_pc = (proc->pc + 2) % MEM_SIZE;
	while (i < g_op[proc->op_id].nb_args)
	{
		reg_num[i] = read_one_arg(vm, proc, (cb >> (6 - i * 2)) & 0b11, i + 1);
		if (reg_num[i] == -1)
			fail = TRUE;
		i++;
	}
	return (fail);
}

static t_bool	load_nocb(t_vm *vm, t_proc *proc, int reg_num[3])
{
	t_bool	fail;

	fail = FALSE;
	proc->new_pc = (proc->pc + 1) % MEM_SIZE;
	reg_num[0] = read_one_arg(vm, proc, T_DIR, 1);
	if (reg_num[0] == -1)
		fail = TRUE;
	return (fail);
}

/*
**	/!\ Beware !
**	No leaks are possible because load_cb and load_nocb cannot crash
**		& because vm->verbosity cannot change after initiation !
**	Modifying the above may cause issues !
*/

int				load_arg_into_regs(t_vm *vm, t_proc *proc, int reg_num[3])
{
	t_bool	fail;

	fail = FALSE;
	if (vm->verbosity >= VE_OPS)
		vm->tmpstr = ft_cprintf("P%5d | %s", proc->pid, g_op[proc->op_id].name);
	if (g_op[proc->op_id].coding_byte == TRUE)
		fail = load_cb(vm, proc, reg_num);
	else
		fail = load_nocb(vm, proc, reg_num);
	if (vm->verbosity >= VE_OPS && !fail)
		ft_putstr(vm->tmpstr);
	if (vm->verbosity >= VE_OPS)
		ft_memfree(&vm->tmpstr);
	return (!fail);
}
