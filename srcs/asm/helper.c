/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpoirier <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/14 19:30:37 by cpoirier          #+#    #+#             */
/*   Updated: 2019/05/24 17:52:05 by cpoirier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int		get_name(t_asm *a, char *s, char name[], size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	skip_whitespace(s, &i);
	if (s[i] != '"' && (a->curr_char += i))
		return (0);
	i++;
	j = 0;
	while (s[i + j] && s[i + j] != '"' && j < len)
	{
		name[j] = s[i + j];
		j++;
	}
	if ((!s[i + j] || !j || (j >= len && s[i + j] != '"'))
		&& (a->curr_char += i + j))
		return (0);
	name[j] = '\0';
	return (1);
}

int		get_op_id(char *s)
{
	size_t	i;

	i = 0;
	while (i < 16)
	{
		if (!ft_strncmp(s, g_op_tab[i].name, ft_strlen(g_op_tab[i].name))
			&& (s[ft_strlen(g_op_tab[i].name)] == ' '
				|| s[ft_strlen(g_op_tab[i].name)] == '\t'))
			return (i + 1);
		i++;
	}
	return (0);
}

void	write_header(t_asm *my_asm)
{
	unsigned int	n;
	int				k;
	size_t			pos;

	my_asm->header.magic = COREWAR_EXEC_MAGIC;
	ft_memcpy(my_asm->output, &my_asm->header, sizeof(t_header));
	n = COREWAR_EXEC_MAGIC;
	k = -1;
	while (++k < 4)
	{
		my_asm->output[3 - k] = n % 256;
		n >>= 8;
	}
	n = my_asm->header.prog_size;
	k = -1;
	pos = ((4 + PROG_NAME_LENGTH) / 8 + 1) * 8;
	while (++k < 4)
	{
		my_asm->output[pos + 3 - k] = n % 256;
		n >>= 8;
	}
}

char	*get_base_name(char *s)
{
	int		dot_pos;
	size_t	i;
	char	*res;

	i = -1;
	dot_pos = -1;
	while (s[++i])
	{
		if (s[i] == '.')
			dot_pos = (int)i;
	}
	if (dot_pos > 0)
		s[dot_pos] = '\0';
	res = ft_strjoin(s, ".cor");
	free(s);
	if (!res)
		fail_msg(0, "Malloc failed");
	return (res);
}
