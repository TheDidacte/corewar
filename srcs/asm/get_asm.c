/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_asm.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 20:23:20 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/25 14:04:11 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	init_asm(t_asm *my_asm, int *fd, char *path)
{
	char *tmp;

	if ((*fd = open(path, O_RDONLY)) == -1)
		fail_msg(my_asm, strerror(errno));
	init_labels(my_asm);
	tmp = ft_strnew(sizeof(t_header));
	write_n_to_output(&my_asm->output, &my_asm->cursor, tmp, sizeof(t_header));
	my_asm->fd = *fd;
	free(tmp);
}

void	handle_name(t_asm *my_asm, char *s, size_t *i)
{
	if (my_asm->header.prog_name[0])
		fail_msg(my_asm, "Error: Name already declared");
	my_asm->curr_char += ft_strlen(NAME_CMD_STRING);
	if (!get_name(my_asm, s + *i + ft_strlen(NAME_CMD_STRING),
			my_asm->header.prog_name, PROG_NAME_LENGTH))
		fail_msg(my_asm, "Syntax error: Name not well-formated");
}

void	handle_comment(t_asm *my_asm, char *s, size_t *i)
{
	if (my_asm->header.comment[0])
		fail_msg(my_asm, "Error: Comment already declared");
	my_asm->curr_char += ft_strlen(COMMENT_CMD_STRING);
	if (!get_name(my_asm, s + *i + ft_strlen(COMMENT_CMD_STRING),
			my_asm->header.comment, COMMENT_LENGTH))
		fail_msg(my_asm, "Syntax error: Comment not well-formated");
}

void	handle_labels(t_asm *my_asm, char *s, size_t *i)
{
	size_t		initial_i;

	initial_i = *i;
	if (!my_asm->header.prog_name[0] || !my_asm->header.comment[0])
		fail_msg(my_asm, "Error: Name and Comment must be declared before"
			" any instruction or label");
	add_label(&my_asm->labels, &my_asm->label_pos,
		0, my_asm->cursor);
	if (!read_label(my_asm, my_asm->labels + my_asm->label_pos - 1, s + *i))
		fail_msg(my_asm, "Lexical error on label definition");
	else if (label_exists(my_asm, my_asm->labels[my_asm->label_pos - 1].name,
			my_asm->label_pos - 1))
		fail_msg(my_asm, "Label already exists");
	else
	{
		*i += ft_strlen(my_asm->labels[my_asm->label_pos - 1].name) + 1;
		skip_whitespace(s, i);
		my_asm->curr_char += *i - initial_i;
		if ((my_asm->current_op = get_op_id(s + *i)))
			handle_op(my_asm, s + *i);
		else if (s[*i] && s[*i] != COMMENT_CHAR)
			fail_msg(my_asm, "Unknown operation");
	}
}

void	clear_asm(t_asm *my_asm, char *s)
{
	free(s);
	if (!my_asm->header.prog_name[0] || !my_asm->header.comment[0])
		fail_msg(my_asm, "Error: Name and Comment must be declared before"
			" any instruction or label");
	write_label_holders(my_asm);
	my_asm->header.prog_size = my_asm->cursor - sizeof(t_header);
	write_header(my_asm);
	write_output(my_asm);
	free_asm(my_asm);
}
