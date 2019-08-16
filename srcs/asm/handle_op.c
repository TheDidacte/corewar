/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_op.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:57:59 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/24 17:49:39 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	init_op(t_asm *my_asm, char *s, size_t *i, t_arg_type types[3])
{
	my_asm->op_begin = my_asm->cursor;
	ft_bzero(types, sizeof(t_arg_type) * 3);
	write_nb_to_output(my_asm, my_asm->current_op, 1);
	*i = 0;
	while (s[*i] != ' ' && s[*i] != '\t')
		(*i)++;
	skip_whitespace(s, i);
	if (my_asm->current_op != 1 && my_asm->current_op != 12
		&& my_asm->current_op != 15 && my_asm->current_op != 9)
		write_nb_to_output(my_asm, 0, 1);
	my_asm->curr_char += *i;
}

void	handle_current_op(t_asm *my_asm, char *s, size_t *i,
		size_t *current_param)
{
	size_t	initial_i;

	initial_i = *i;
	if (s[*i] == SEPARATOR_CHAR && *current_param + 1
		== (size_t)g_op_tab[my_asm->current_op - 1].nb_args)
		fail_msg(my_asm, "Too much parameters");
	else if (!s[*i] && *current_param + 1
		< (size_t)g_op_tab[my_asm->current_op - 1].nb_args)
		fail_msg(my_asm, "Missing parameters");
	if (s[*i])
		(*i)++;
	skip_whitespace(s, i);
	(*current_param)++;
	my_asm->curr_char += *i - initial_i;
}

void	handle_op(t_asm *my_asm, char *s)
{
	size_t		i;
	size_t		current_param;
	t_arg_type	types[3];

	if (!my_asm->header.prog_name[0] || !my_asm->header.comment[0])
		fail_msg(my_asm, "Error: Name and Comment must be declared before"
			" any instruction or label");
	init_op(my_asm, s, &i, types);
	current_param = 0;
	while (current_param < (size_t)g_op_tab[my_asm->current_op - 1].nb_args)
	{
		check_param(my_asm, s + i, current_param, types);
		while (s[i] && s[i] != COMMENT_CHAR && s[i] != SEPARATOR_CHAR)
		{
			i++;
			my_asm->curr_char++;
		}
		handle_current_op(my_asm, s, &i, &current_param);
	}
	if (my_asm->current_op != 1 && my_asm->current_op != 12
		&& my_asm->current_op != 15 && my_asm->current_op != 9)
		write_opcode(my_asm, types);
}
