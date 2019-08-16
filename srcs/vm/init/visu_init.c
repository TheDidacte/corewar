/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visu_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 18:06:53 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 21:28:24 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "init.h"
#include <signal.h>
#include <locale.h>

static void	init_ncwindows(t_vm *vm)
{
	int				dim[2];
	struct s_sidep	*sp;

	getmaxyx(stdscr, dim[1], dim[0]);
	if (dim[0] < 250 || dim[1] < 68)
		exit_vm(vm, gb_add(&vm->gb,
				ft_cprintf("Windows too small (%dx%d), minimum is (250x68)",
					dim[0], dim[1])));
	if (!(sp = &vm->visu.sidep)
		|| !(vm->visu.rootw = newwin(68, 250, 0, 0))
		|| !(vm->visu.arenaw = subwin(vm->visu.rootw, 64, 193, 2, 2))
		|| !(sp->rootw = subwin(vm->visu.rootw, 68, 54, 0, 196))
		|| !(sp->statusw = subwin(sp->rootw, 57, 48, 2, 199))
		|| !(sp->printw = subwin(sp->rootw, 6, 48, 60, 199)))
		exit_vm(vm, "Could not create ncurses windows for visualizer.");
}

/*
**	Because of 42s norme, this function is unreadable.
**	But in short we first setup the memory owner array, then
**	the player color, then the cursors col, then the freshly written memory col
*/

static void	init_colors(t_vm *vm)
{
	ft_memset(vm->mem_owner, PLAY0_COLOR - 1, MEM_SIZE);
	init_pair(CONTOUR_COLOR, COLOR_BRIGHT_BLACK, COLOR_BRIGHT_BLACK);
	init_pair(PLAY0_COLOR - 1, COLOR_BRIGHT_BLACK, COLOR_BLACK);
	init_pair(PLAY0_COLOR + 0, COLOR_GREEN, COLOR_BLACK);
	init_pair(PLAY0_COLOR + 1, COLOR_BLUE, COLOR_BLACK);
	init_pair(PLAY0_COLOR + 2, COLOR_RED, COLOR_BLACK);
	init_pair(PLAY0_COLOR + 3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(CURS0_COLOR + -1, COLOR_BLACK, COLOR_WHITE);
	init_pair(CURS0_COLOR + 0, COLOR_BLACK, COLOR_GREEN);
	init_pair(CURS0_COLOR + 1, COLOR_BLACK, COLOR_BLUE);
	init_pair(CURS0_COLOR + 2, COLOR_BLACK, COLOR_RED);
	init_pair(CURS0_COLOR + 3, COLOR_BLACK, COLOR_YELLOW);
	init_pair(FRESH0_COLOR + 0, COLOR_BRIGHT_GREEN, COLOR_BLACK);
	init_pair(FRESH0_COLOR + 1, COLOR_BRIGHT_BLUE, COLOR_BLACK);
	init_pair(FRESH0_COLOR + 2, COLOR_BRIGHT_RED, COLOR_BLACK);
	init_pair(FRESH0_COLOR + 3, COLOR_BRIGHT_YELLOW, COLOR_BLACK);
}

void		init_ncurses(t_vm *vm)
{
	setlocale(LC_ALL, "");
	vm->visu.op_per_sec = 10;
	vm->visu.paused = TRUE;
	initscr();
	noecho();
	curs_set(FALSE);
	start_color();
	nodelay(stdscr, TRUE);
	init_ncwindows(vm);
	init_colors(vm);
	visu_init_memview(vm);
	vm->verbosity = VE_VISU;
}
