/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_char.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 17:33:25 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/24 17:54:26 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"
#include <stdlib.h>

void		update_char(t_asm *my_asm, size_t i, size_t j)
{
	my_asm->curr_line = my_asm->labels_holder[i].line;
	my_asm->curr_char = ft_strlen(my_asm->labels_holder[i].buff)
		- ft_strlen(ft_strstr(my_asm->labels_holder[i].buff,
				my_asm->labels_holder[i].name)) - 1;
	if (j == my_asm->label_pos)
	{
		free(my_asm->line);
		my_asm->line = my_asm->labels_holder[i].buff;
		my_asm->labels_holder[i].buff = 0;
		fail_msg(my_asm, "Label not found");
	}
}

void		check_param(t_asm *my_asm, char *s, size_t current_param,
		t_arg_type types[3])
{
	int		old_pos;

	old_pos = my_asm->curr_char;
	if ((types[current_param] = get_arg_type(my_asm, s)) < 0)
		fail_msg(my_asm, "Syntax error for parameter");
	my_asm->curr_char = old_pos;
	if (!(g_op_tab[my_asm->current_op - 1].args_types[current_param]
			& types[current_param]))
		fail_msg(my_asm, "Invalid type");
	write_param(my_asm, types[current_param], s);
}
