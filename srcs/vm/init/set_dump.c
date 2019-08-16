/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_dump.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 17:44:25 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 17:48:51 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "init.h"

void		read_dump_cycle(t_vm *env, char *input)
{
	if (env->dump_width)
		exit_vm(env, "Dump cycle set multiple times !");
	env->dump_width = 32;
	if (input == NULL
		|| ft_strlen(input) > 10
		|| (env->cycle_dump = ft_atoi(input)) <= 0)
		exit_vm(env, gb_add(&env->gb,
			ft_cprintf("Dump cycle badly formatted ('%s')", input)));
}

void		read_ndump_cycle(t_vm *env, char **argv, int *agid)
{
	if (env->dump_width)
		exit_vm(env, "Dump cycle set multiple times !");
	++(*agid);
	if (argv[*agid] == NULL
		|| ft_strlen(argv[*agid]) > 10
		|| (env->dump_width = ft_atoi(argv[*agid])) <= 0
		|| env->dump_width > 1024)
		exit_vm(env, gb_add(&env->gb,
			ft_cprintf("Dump width badly formatted ('%s')", argv[*agid])));
	++(*agid);
	if (argv[*agid] == NULL
		|| ft_strlen(argv[*agid]) > 10
		|| (env->cycle_dump = ft_atoi(argv[*agid])) <= 0)
		exit_vm(env, gb_add(&env->gb,
			ft_cprintf("Dump cycle badly formatted ('%s')", argv[*agid])));
}
