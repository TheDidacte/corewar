/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/06 15:51:46 by mbenjell          #+#    #+#             */
/*   Updated: 2019/04/12 18:02:35 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void			add_transition(t_state *src, t_state *dest)
{
	src->next_states[src->nb_transition++] = dest;
}

t_state			*create_state(t_automaton *a, int type)
{
	t_state		*state;
	t_state		**next;

	state = (t_state*)malloc(sizeof(t_state));
	if (state == NULL)
		return (NULL);
	memset(state, 0, sizeof(t_state));
	next = (t_state**)malloc(sizeof(t_state*) * (NB_STATES + 1));
	if (next == NULL)
		return (NULL);
	memset(next, 0, sizeof(t_state*) * (NB_STATES + 1));
	a->states[a->nb_states++] = state;
	state->next_states = next;
	state->pf = transition;
	state->type = type;
	return (state);
}

t_automaton		*create_automaton(void *data)
{
	t_automaton	*a;

	a = (t_automaton*)malloc(sizeof(t_automaton));
	if (a == NULL)
		return (NULL);
	memset(a, 0, sizeof(t_automaton));
	a->data = data;
	return (a);
}
