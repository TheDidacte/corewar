/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_memview.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 18:57:21 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 20:38:10 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "vm.h"

void		visu_init_memview(t_vm *vm)
{
	int				i;
	int				mem_pc;
	uint			mem;

	i = -1;
	while (++i < vm->players.len)
	{
		mem = 0;
		mem_pc = vm->procs.d[i].pc;
		wmove(vm->visu.arenaw, mem_pc / 64, mem_pc % 64);
		wattron(vm->visu.arenaw, COLOR_PAIR(PLAY0_COLOR + i));
		while (mem < vm->players.d[i].head.prog_size)
		{
			if (mem_pc % 64 == 0)
				wprintw(vm->visu.arenaw, "\n");
			wprintw(vm->visu.arenaw, " %02hhx", vm->arena[mem_pc]);
			vm->mem_owner[mem_pc] = i + PLAY0_COLOR;
			mem_pc++;
			mem++;
		}
		wattroff(vm->visu.arenaw, COLOR_PAIR(i + PLAY0_COLOR));
	}
}

static void	memview_cursors(t_vm *vm)
{
	int				pr;
	int				pc;
	int				color_id;

	pr = -1;
	while (++pr < vm->procs.len)
	{
		pc = vm->procs.d[pr].pc;
		wmove(vm->visu.arenaw, pc / 64, (pc % 64) * 3 + 1);
		color_id = vm->mem_owner[pc];
		color_id -= color_id >= FRESH0_COLOR ? DELT_FRESH_COLOR : 0;
		color_id += DELT_CURS_COLOR;
		wattron(vm->visu.arenaw, COLOR_PAIR(color_id));
		wprintw(vm->visu.arenaw, "%02hhx", vm->arena[pc]);
		wattroff(vm->visu.arenaw, COLOR_PAIR(color_id));
	}
}

void		visu_memview(t_vm *vm)
{
	int				i;
	int				mem;
	clock_t			cur_time;
	float			t;

	wmove(vm->visu.arenaw, 0, 0);
	mem = 0;
	cur_time = clock();
	while (mem != 4096)
	{
		i = -1;
		while (++i < 64)
		{
			wattron(vm->visu.arenaw, COLOR_PAIR(vm->mem_owner[mem + i]));
			wprintw(vm->visu.arenaw, " %02hhx", vm->arena[mem + i]);
			wattroff(vm->visu.arenaw, COLOR_PAIR(vm->mem_owner[mem + i]));
			t = ((cur_time - vm->time_write[mem + i]) / (float)CLOCKS_PER_SEC);
			if (vm->mem_owner[mem + i] >= FRESH0_COLOR
				&& t >= TIME_OF_WRITE)
				vm->mem_owner[mem + i] -= DELT_FRESH_COLOR;
		}
		mem += 64;
		wprintw(vm->visu.arenaw, "\n");
	}
	memview_cursors(vm);
}
