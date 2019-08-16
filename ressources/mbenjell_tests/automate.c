/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   automate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/06 13:49:04 by mbenjell          #+#    #+#             */
/*   Updated: 2019/05/08 21:29:48 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void			transition(void)
{
	printf("pointeur sur fonction\n");
}

void			change_state(void *env, void *transition)
{
	t_env		*e;
	t_state		*current;
	t_state		**next;

	e = (t_env*)env;
	current = e->state;
	next = current->next_states;
	current->pf();
	e->state = *next;
}

void			run_automaton(t_automaton *a, t_state *start)
{
	t_env		*e;
	t_state		*current;
	int			transition;

	transition = 0;
	e = (t_env*)a->data;
	e->state = start;
	current = e->state;
	while (current->type != end)
	{
		change_state(e, &transition);
		if (current->type == ERROR || e->state == NULL)
			break ;
		printf("run A : %p\n", current);
	}
}
