/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_one_arg.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/17 22:57:41 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/23 11:19:18 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
**	In here, 1 <= cur_arg <= MAX_ARGS_NUMBER
*/

static int		load_reg(t_vm *vm, t_proc *proc, int cur_arg)
{
	int		tmp;

	tmp = load8(vm, proc->new_pc);
	proc->new_pc = (proc->new_pc + 1) % MEM_SIZE;
	if (vm->verbosity >= VE_OPS)
		ft_strcat_join(&vm->tmpstr, ft_cprintf(" r%d", tmp));
	if (tmp <= REG_NUMBER && tmp >= 1
		&& (T_REG & g_op[proc->op_id].args_type[cur_arg - 1]))
		return (tmp);
	else
		return (-1);
}

static int		load_dir(t_vm *vm, t_proc *proc, int cur_arg)
{
	if (g_op[proc->op_id].dir2)
	{
		proc->reg[REG_NUMBER + cur_arg] = load16(vm, proc->new_pc);
		proc->new_pc = (proc->new_pc + 2) % MEM_SIZE;
	}
	else
	{
		proc->reg[REG_NUMBER + cur_arg] = load32(vm, proc->new_pc);
		proc->new_pc = (proc->new_pc + 4) % MEM_SIZE;
	}
	if (vm->verbosity >= VE_OPS)
		ft_strcat_join(&vm->tmpstr,
			ft_cprintf(" %d", proc->reg[REG_NUMBER + cur_arg]));
	if (!(T_DIR & g_op[proc->op_id].args_type[cur_arg - 1]))
		return (-1);
	else
		return (REG_NUMBER + cur_arg);
}

static int		load_ind(t_vm *vm, t_proc *proc, int cur_arg)
{
	int		ind;

	ind = load16(vm, proc->new_pc);
	if (g_op[proc->op_id].ldx_rel)
		proc->reg[REG_NUMBER + cur_arg] = load32(vm, proc->pc + ind % IDX_MOD);
	else
		proc->reg[REG_NUMBER + cur_arg] = load32(vm, proc->pc + ind);
	proc->new_pc = (proc->new_pc + 2) % MEM_SIZE;
	if (vm->verbosity >= VE_OPS)
		ft_strcat_join(&vm->tmpstr, ft_cprintf(" %d", ind));
	if (!(T_IND & g_op[proc->op_id].args_type[cur_arg - 1]))
		return (-1);
	else
		return (REG_NUMBER + cur_arg);
}

int				read_one_arg(t_vm *vm, t_proc *proc, uint8_t cb, int cur_arg)
{
	if (cb == REG_CODE)
		return (load_reg(vm, proc, cur_arg));
	else if (cb == DIR_CODE)
		return (load_dir(vm, proc, cur_arg));
	else if (cb == IND_CODE)
		return (load_ind(vm, proc, cur_arg));
	else
		return (-1);
}
