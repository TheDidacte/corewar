/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   winner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/18 16:33:52 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/23 12:58:37 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "libft.h"

void		print_winner(t_vm *vm)
{
	int		i;
	int		win_i;
	t_play	*players;

	if (vm->cycle_dump && vm->cycle_curr == vm->cycle_dump)
		return ;
	players = vm->players.d;
	win_i = 0;
	i = 0;
	while (++i < vm->players.len)
	{
		if (players[i].last_live > players[win_i].last_live)
			win_i = i;
		else if (players[i].last_live == players[win_i].last_live)
			if (players[i].period_lives > players[win_i].period_lives)
				win_i = i;
	}
	if (vm->verbosity >= VE_WINNER)
		ft_printf("Player %i (%s) won \n", players[win_i].id,
			players[win_i].head.prog_name);
	else if (vm->verbosity == VE_VISU)
		visu_endloop(vm, win_i);
}

void		print_register(t_vm *vm, t_proc *proc)
{
	int		i;
	char	*out;

	out = NULL;
	out = ft_cprintf("Proc: %i |", proc - vm->procs.d);
	if (proc->reg[0] != 0)
		ft_printf("{red}r0 is set, WTF ?! (r0: %d)\n", proc->reg[0]);
	if (proc->reg[20] != 0)
		ft_printf("{red}r20 is set, WTF ?! (r20: %d)\n", proc->reg[20]);
	i = 0;
	while (++i < REG_NUMBER + MAX_ARGS_NUMBER)
	{
		ft_strcat_join(&out, ft_cprintf(" r%-2i: %-10i", i, proc->reg[i]));
		if (i == REG_NUMBER)
			ft_strcat_join(&out, " |");
	}
	i = ft_strlen(out);
	out[i] = '\n';
	write(STDOUT_FILENO, out, i + 1);
	free(out);
}
