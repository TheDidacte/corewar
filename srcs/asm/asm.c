/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 18:55:21 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/25 14:01:59 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

const t_op	g_op_tab[17] = {
	{
		"live", 1, {
			T_DIR}
		, 1, 10, "alive", 0, 0}
	, {
		"ld", 2, {
			T_DIR | T_IND, T_REG}
		, 2, 5, "load", 1, 0}
	, {
		"st", 2, {
			T_REG, T_IND | T_REG}
		, 3, 5, "store", 1, 0}
	, {
		"add", 3, {
			T_REG, T_REG, T_REG}
		, 4, 10, "addition", 1, 0}
	, {
		"sub", 3, {
			T_REG, T_REG, T_REG}
		, 5, 10, "soustraction", 1, 0}
	, {
		"and", 3, {
			T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}
		, 6, 6, "et (and r1, r2, r3 r1&r2 -> r3", 1, 0}
	, {
		"or", 3, {
			T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}
		, 7, 6, "ou (or r1, r2, r3 r1 | r2 -> r3", 1, 0}
	, {
		"xor", 3, {
			T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}
		, 8, 6, "ou (xor r1, r2, r3 r1^r2 -> r3", 1, 0}
	, {
		"zjmp", 1, {
			T_DIR}
		, 9, 20, "jump if zero", 0, 1}
	, {
		"ldi", 3, {
			T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}
		, 10, 25, "load index", 1, 1}
	, {
		"sti", 3, {
			T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}
		, 11, 25, "store index", 1, 1}
	, {
		"fork", 1, {
			T_DIR}
		, 12, 800, "fork", 0, 1}
	, {
		"lld", 2, {
			T_DIR | T_IND, T_REG}
		, 13, 10, "long load", 1, 0}
	, {
		"lldi", 3, {
			T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}
		, 14, 50, "long load index", 1, 1}
	, {
		"lfork", 1, {
			T_DIR}
		, 15, 1000, "long fork", 0, 1}
	, {
		"aff", 1, {
			T_REG}
		, 16, 2, "aff", 1, 0}
	, {
		0, 0, {
			0}
		, 0, 0, 0, 0, 0}
}
;

void	write_param(t_asm *my_asm, t_arg_type type, char *s)
{
	size_t	i;
	int		two;

	i = 0;
	two = g_op_tab[my_asm->current_op - 1].dir2 ? 2 : 4;
	if (type & T_IND)
		two = 2;
	if (type & T_DIR || type & T_REG)
		++s;
	if (type & T_LAB)
	{
		create_label_holder(my_asm, s, &i);
		write_nb_to_output(my_asm, two, 1);
		my_asm->cursor += two == 2 ? 1 : 3;
	}
	else if (!(type & T_REG))
		write_nb_to_output(my_asm, ft_atoi(s + i), two);
	else if (ft_atoi(s + i) <= REG_NUMBER && ft_atoi(s + i) > 0)
		write_nb_to_output(my_asm, ft_atoi(s + i), 1);
	else
		fail_msg(my_asm, "Reg number invalid");
}

void	free_asm(t_asm *my_asm)
{
	size_t	i;

	free(my_asm->output);
	free(my_asm->line);
	i = -1;
	while (++i < my_asm->label_pos)
		free(my_asm->labels[i].name);
	i = -1;
	while (++i < my_asm->label_holder_pos)
	{
		free(my_asm->labels_holder[i].name);
		free(my_asm->labels_holder[i].buff);
	}
	if (my_asm->labels)
	{
		free(my_asm->labels[(my_asm->label_pos / LABEL_COUNT + 1)
				* LABEL_COUNT].name);
		free(my_asm->labels_holder[(my_asm->label_holder_pos / LABEL_COUNT + 1)
				* LABEL_COUNT].name);
		free(my_asm->labels);
		free(my_asm->labels_holder);
	}
	free(my_asm->file_name);
}

int		get_asm(char *path, t_asm *my_asm)
{
	int		fd;
	size_t	i;

	init_asm(my_asm, &fd, path);
	while (get_next_line(fd, &my_asm->line) > 0)
	{
		init_asm_loop(my_asm, &i, my_asm->line);
		if (!ft_strncmp(my_asm->line + i, NAME_CMD_STRING,
				ft_strlen(NAME_CMD_STRING)))
			handle_name(my_asm, my_asm->line, &i);
		else if (!ft_strncmp(my_asm->line + i, COMMENT_CMD_STRING,
				ft_strlen(COMMENT_CMD_STRING)))
			handle_comment(my_asm, my_asm->line, &i);
		else if ((my_asm->current_op = get_op_id(my_asm->line + i)))
			handle_op(my_asm, my_asm->line + i);
		else if (my_asm->line[i] && my_asm->line[i] != COMMENT_CHAR
			&& ft_strchr(my_asm->line + i, LABEL_CHAR))
			handle_labels(my_asm, my_asm->line, &i);
		else if (my_asm->line[i] && my_asm->line[i] != COMMENT_CHAR)
			fail_msg(my_asm, "Unknown operation");
		free(my_asm->line);
	}
	clear_asm(my_asm, my_asm->line);
	return (1);
}

int		main(int ac, char **av)
{
	t_asm	my_asm;

	if (ac < 2)
	{
		ft_printf("Usage: %s <sourcefile.s>\n", av[0]);
		return (0);
	}
	ft_bzero(&my_asm, sizeof(my_asm));
	my_asm.path = av[1];
	my_asm.file_name = get_base_name(ft_strdup(av[1]));
	return (get_asm(av[1], &my_asm));
}
