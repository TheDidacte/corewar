/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_sidepview.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 18:17:49 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 20:39:20 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static void	print_cycles_info(t_vm *vm)
{
	wprintw(vm->visu.sidep.statusw,
		"\n\nCYCLE_TO_DIE: %-10d"
		"\nCycle checks: %-10d",
		vm->cycle_die,
		vm->die_cycle_checks);
}

static void	print_ops(t_vm *vm)
{
	int		j;
	int		play;
	char	*ptrf;

	wprintw(vm->visu.sidep.statusw, "\n");
	j = -1;
	while (++j < vm->procs.len)
	{
		if (vm->procs.d[j].op_cycles >= 0)
			ptrf = "\nP %-2d Op: %-6s| Cycles remaining: %-4d";
		else
			ptrf = "\nP %-2d No Op";
		play = vm->procs.d[j].play;
		wattron(vm->visu.sidep.statusw, COLOR_PAIR(PLAY0_COLOR + play));
		wprintw(vm->visu.sidep.statusw,
			ptrf,
			vm->procs.d[j].pid,
			g_op[vm->procs.d[j].op_id].name,
			vm->procs.d[j].op_cycles);
		wattroff(vm->visu.sidep.statusw, COLOR_PAIR(PLAY0_COLOR + play));
	}
}

static void	print_players(t_vm *vm)
{
	int		i;

	i = -1;
	while (++i < vm->players.len)
	{
		wprintw(vm->visu.sidep.statusw, "\n\nPlayer %d: ", vm->players.d[i].id);
		wattron(vm->visu.sidep.statusw, COLOR_PAIR(PLAY0_COLOR + i));
		wprintw(vm->visu.sidep.statusw, "%s", vm->players.d[i].head.prog_name);
		wattroff(vm->visu.sidep.statusw, COLOR_PAIR(PLAY0_COLOR + i));
		wprintw(vm->visu.sidep.statusw,
			"\n\tLast live: %d"
			"\n\tLives in current period: %d",
			vm->players.d[i].last_live, vm->players.d[i].period_lives);
	}
}

static void	print_proc_nb(t_vm *vm)
{
	wprintw(vm->visu.sidep.statusw,
		"\nProcesses: %d",
		vm->procs.len);
}

void		visu_sidepview(t_vm *vm)
{
	char	*s;

	werase(vm->visu.sidep.rootw);
	wattron(vm->visu.sidep.rootw, COLOR_PAIR(CONTOUR_COLOR));
	box(vm->visu.sidep.rootw, '*', '*');
	wmove(vm->visu.sidep.rootw, 59, 0);
	whline(vm->visu.sidep.rootw, '*', 73);
	wattroff(vm->visu.sidep.rootw, COLOR_PAIR(CONTOUR_COLOR));
	wmove(vm->visu.sidep.statusw, 0, 0);
	s = vm->visu.paused ? "PAUSED" : "RUNNING";
	wprintw(vm->visu.sidep.statusw,
		"** %s **"
		"\n\nMax cycles/second: %.0f"
		"\n\nCycle: %d",
		s, vm->visu.op_per_sec, vm->cycle_curr);
	print_proc_nb(vm);
	print_players(vm);
	print_cycles_info(vm);
	print_ops(vm);
	wmove(vm->visu.sidep.printw, 1, 0);
	wprintw(vm->visu.sidep.printw, "%s", vm->visu.aff);
}
