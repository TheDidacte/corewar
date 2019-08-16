/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   labels.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:12:52 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/24 17:53:55 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int		read_label(t_asm *my_asm, t_label *label, char *s)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != LABEL_CHAR && ft_strchr(LABEL_CHARS, s[i])
		&& s[i] != COMMENT_CHAR)
		i++;
	if (s[i] != LABEL_CHAR || !i)
	{
		my_asm->curr_char += i;
		return (0);
	}
	free(label->name);
	if (!(label->name = (char *)ft_memalloc(i + 1)))
		fail_msg(0, "Realloc failed");
	ft_strncpy(label->name, s, i);
	return (1);
}

void	init_labels(t_asm *my_asm)
{
	if (!(my_asm->labels = ft_memalloc((LABEL_COUNT + 1) * sizeof(t_label)))
		|| !(my_asm->labels_holder = ft_memalloc((LABEL_COUNT + 1)
				* sizeof(t_label))))
		fail_msg(0, "Malloc failed");
	my_asm->labels[LABEL_COUNT].name = 0;
	my_asm->labels_holder[LABEL_COUNT].name = 0;
}

void	add_label(t_label **l, size_t *pos, char *name, size_t p)
{
	if (*pos && !(*pos % LABEL_COUNT))
	{
		if (!(*l = (t_label *)realloc(*l, sizeof(t_label) * (*pos + LABEL_COUNT
						+ 1))))
			fail_msg(0, "Realloc failed");
		(*l)[*pos + LABEL_COUNT].name = 0;
	}
	(*l)[*pos].name = name;
	(*l)[*pos].pos = p;
	(*l)[*pos].offset = 0;
	(*pos)++;
}

void	create_label_holder(t_asm *my_asm, char *s, size_t *i)
{
	char	*name;
	size_t	len;

	len = 0;
	while (ft_strchr(LABEL_CHARS, s[*i + len + 1]) && s[*i + len + 1])
		len++;
	if (!(name = (char *)malloc(len + 1)))
		fail_msg(0, "Malloc failed");
	ft_strncpy(name, s + *i + 1, len);
	name[len] = '\0';
	(*i) += len;
	add_label(&my_asm->labels_holder, &my_asm->label_holder_pos,
			name, my_asm->op_begin);
	my_asm->labels_holder[my_asm->label_holder_pos
		- 1].buff = ft_strdup(my_asm->line);
	my_asm->labels_holder[my_asm->label_holder_pos - 1].offset = my_asm->cursor
		- my_asm->op_begin;
	my_asm->labels_holder[my_asm->label_holder_pos
		- 1].line = my_asm->curr_line;
}

void	write_label_holders(t_asm *my_asm)
{
	size_t	i;
	size_t	j;

	i = -1;
	while (++i < my_asm->label_holder_pos)
	{
		j = -1;
		while (++j < my_asm->label_pos)
		{
			if (!ft_strcmp(my_asm->labels_holder[i].name,
					my_asm->labels[j].name))
			{
				write_back(my_asm, my_asm->labels_holder[i].pos
					+ my_asm->labels_holder[i].offset,
					(int)my_asm->labels[j].pos
					- (int)my_asm->labels_holder[i].pos);
				break ;
			}
		}
		update_char(my_asm, i, j);
	}
}
