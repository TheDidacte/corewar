/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/06 13:50:24 by mbenjell          #+#    #+#             */
/*   Updated: 2019/05/08 21:32:20 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static void		add_states(t_automaton *a)
{
	create_state(a, start);
	create_state(a, state_1);
	create_state(a, state_2);
	create_state(a, state_3);

	create_state(a, end);
}

static void		connect_states(t_automaton *a)
{
	t_state 	**states;

	states = a->states;
	add_transition(states[start], states[state_1]);
	add_transition(states[state_1], states[state_2]);
	add_transition(states[state_2], states[state_3]);
	add_transition(states[state_3], states[end]);
}

static int		compose(t_automaton *a)
{
	t_state		**states;

	states = (t_state**)malloc(sizeof(t_state*) * (NB_STATES + 1));
	if (states == NULL)
		return (ERROR);
	memset(states, 0, sizeof(t_state*) * (NB_STATES + 1));
	a->states = states;
	return (SUCCESS);
}

void	code(void)
{
	t_env		data;
	t_automaton	*a;
	t_state		*start;

	a = create_automaton(&data);
	if (a == NULL)
		return ;
	compose(a);
	add_states(a);
	connect_states(a);
	start = a->states[0];
	data.state = start;
	run_automaton(a, data.state);
}
