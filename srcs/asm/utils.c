/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 18:13:30 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/25 13:55:23 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "asm.h"

void	fail_msg(t_asm *my_asm, char *s)
{
	int		i;

	if (my_asm)
	{
		ft_printf("<bold>%s:%d:%d: {red}error:{eoc} %s<rst>\n",
			my_asm->path, my_asm->curr_line, my_asm->curr_char + 1, s);
		ft_printf("%s\n", my_asm->line);
		i = -1;
		!my_asm->line ? my_asm->curr_char = 0 : 0;
		while (++i < my_asm->curr_char)
			(my_asm->line[i] == '\t') ? write(1, "\t", 1) : write(1, " ", 1);
		ft_printf("<bold>{grn}^<rst>\n");
		get_next_line(my_asm->fd, GNL_FLUSH);
		free_asm(my_asm);
	}
	else
		ft_printf("Error: %s\n", s);
	exit(EXIT_FAILURE);
}

void	write_back(t_asm *my_asm, size_t pos, int val)
{
	size_t		old_pos;

	old_pos = my_asm->cursor;
	my_asm->cursor = pos;
	write_nb_to_output(my_asm, val, my_asm->output[my_asm->cursor]);
	my_asm->cursor = old_pos;
}

void	write_nb(char *s, int nb, int byte_nb)
{
	int				i;

	i = -1;
	while (++i < byte_nb)
	{
		s[byte_nb - 1 - i] = nb % 256;
		nb >>= 8;
	}
}

void	skip_whitespace(char *s, size_t *i)
{
	while (s[*i] == ' ' || s[*i] == '\t')
		(*i)++;
}

int		label_exists(t_asm *my_asm, char *s, size_t j)
{
	size_t		i;

	i = -1;
	while (++i < my_asm->label_pos)
		if (i != j && !ft_strcmp(my_asm->labels[i].name, s))
			return (1);
	return (0);
}
