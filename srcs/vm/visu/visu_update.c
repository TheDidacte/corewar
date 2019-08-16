/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_update.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 22:29:22 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/24 17:32:16 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"
#include <ncurses.h>
#include <time.h>

static void	exit_vm2(t_vm *env, char *err_msg)
{
	if (env->verbosity == VE_VISU)
		clean_visu(env);
	write(1, err_msg, ft_strlen(err_msg));
	write(1, "\n", 1);
	gb_freeall(&env->gb);
	exit(EXIT_FAILURE);
}

void		visu_khandler(t_vm *vm)
{
	int		c;

	while ((c = getch()) != ERR)
	{
		if (c == NC_Q)
			vm->visu.op_per_sec -= 10;
		else if (c == NC_W)
			vm->visu.op_per_sec -= 1;
		else if (c == NC_E)
			vm->visu.op_per_sec += 1;
		else if (c == NC_R)
			vm->visu.op_per_sec += 10;
		else if (c == NC_SPC)
			vm->visu.paused = vm->visu.paused ? FALSE : TRUE;
		else if (c == NC_1)
			vm->visu.op_per_sec = 50;
		else if (c == NC_2)
			vm->visu.op_per_sec = 9999;
		else if (c == NC_ESC)
			exit_vm2(vm, "exit success !");
	}
	if (vm->visu.op_per_sec < 1)
		vm->visu.op_per_sec = 1;
}

/*
**	getch() f***s everything up so we have to redraw EVERYTHING, EVERYTIME
*/

void		visu_update(t_vm *vm)
{
	visu_khandler(vm);
	wattron(vm->visu.rootw, COLOR_PAIR(CONTOUR_COLOR));
	box(vm->visu.rootw, '*', '*');
	wattroff(vm->visu.rootw, COLOR_PAIR(CONTOUR_COLOR));
	visu_sidepview(vm);
	visu_memview(vm);
	wrefresh(vm->visu.rootw);
	wrefresh(vm->visu.arenaw);
	wrefresh(vm->visu.sidep.rootw);
	wrefresh(vm->visu.sidep.statusw);
}

void		visu_endloop(t_vm *vm, int winner)
{
	ft_strcpy(vm->visu.aff,
		gb_add(&vm->gb,
			ft_cprintf("Contestant %i, \"%s\", has won !\nPress ESC to quit.",
				vm->players.d[winner].id,
				vm->players.d[winner].head.prog_name)));
	while (1)
	{
		visu_khandler(vm);
		wattron(vm->visu.rootw, COLOR_PAIR(CONTOUR_COLOR));
		box(vm->visu.rootw, '*', '*');
		wattroff(vm->visu.rootw, COLOR_PAIR(CONTOUR_COLOR));
		visu_sidepview(vm);
		visu_memview(vm);
		wrefresh(vm->visu.rootw);
		wrefresh(vm->visu.arenaw);
		wrefresh(vm->visu.sidep.rootw);
		wrefresh(vm->visu.sidep.statusw);
	}
}
