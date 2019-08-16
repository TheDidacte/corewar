/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 17:59:57 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/23 15:46:16 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "vm.h"

void				loop(t_vm *vm)
{
	int		cycle;

	cycle = 0;
	while (run_vm_cycle(vm))
	{
		cycle++;
		if (vm->cycle_dump == cycle)
		{
			dump_memory(vm);
			break ;
		}
	}
}

static inline void	loop_refresh(t_vm *vm, float *dt, clock_t *t)
{
	while (*dt > 0.3)
	{
		*t = clock();
		visu_update(vm);
		*dt -= (float)(clock() - *t) / CLOCKS_PER_SEC;
	}
}

static inline float	cal_dt(t_vm *vm, clock_t t)
{
	return (1. / vm->visu.op_per_sec) - ((float)(clock() - t) / CLOCKS_PER_SEC);
}

void				visu_loop(t_vm *vm)
{
	clock_t		t;
	float		dt;
	int			alive;
	int			skip_render;

	skip_render = 0;
	alive = 1;
	while (alive)
	{
		t = clock();
		if (!vm->visu.paused)
			alive = run_vm_cycle(vm);
		if (!skip_render)
			visu_update(vm);
		dt = cal_dt(vm, t);
		if (dt < -0.f)
			skip_render += skip_render >= 10 ? -10 : 2;
		else
		{
			skip_render -= skip_render ? 1 : 0;
			loop_refresh(vm, &dt, &t);
			if (!vm->visu.paused)
				usleep(dt * 1000000);
		}
	}
}
