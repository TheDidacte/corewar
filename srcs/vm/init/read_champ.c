/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_champ.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 18:08:33 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 20:41:04 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"
#include "init.h"
#include <errno.h>
#include <fcntl.h>

/*
**	Filename is put in t_play.cor by read_champ()
**		it is then read by load_cor which replaces t_play.cor by the actual
**		.cor data address (inside t_vm.arena)
*/

static void		load_cor(t_vm *env, t_play *p, uint8_t *buffer)
{
	int			fd;
	t_garbage	*gb;
	char		*fname;
	uint		size;

	fname = p->fname;
	gb = &env->gb;
	fd = open(fname, O_RDONLY);
	if (fd == -1)
		exit_vm(env, gb_add(gb, ft_cprintf("%s '%s'", strerror(errno), fname)));
	if ((size = lseek(fd, 0, SEEK_END)) <= sizeof(t_header))
		exit_vm(env, gb_add(gb, ft_cprintf("Too small/empty file %s", fname)));
	size -= sizeof(t_header);
	if (size > CHAMP_MAX_SIZE)
		exit_vm(env, gb_add(gb, ft_cprintf("Champion %s too big", fname)));
	lseek(fd, 0, SEEK_SET);
	if (read(fd, &p->head, sizeof(t_header)) != sizeof(t_header)
		|| read(fd, buffer, size) != size)
		exit_vm(env, gb_add(gb, ft_cprintf("%s '%s'", strerror(errno), fname)));
	p->head.magic = swap32_endian(p->head.magic);
	p->head.prog_size = swap32_endian(p->head.prog_size);
	if (p->head.magic != COREWAR_EXEC_MAGIC)
		exit_vm(env, gb_add(gb, ft_cprintf("Exec magic not recognised")));
	if (p->head.prog_size != size)
		exit_vm(env, gb_add(gb, ft_cprintf("Prog size does not match")));
}

/*
**	This function also sets the PC
*/

void			champs_setup(t_vm *vm)
{
	int			i;
	int			spacing;
	int			offset;

	offset = 0;
	spacing = MEM_SIZE / vm->players.len;
	i = -1;
	while (++i < vm->procs.len)
	{
		load_cor(vm, &vm->players.d[i], &vm->arena[offset]);
		vm->procs.d[i].reg[1] = vm->players.d[i].id;
		vm->procs.d[i].pc = offset;
		offset += spacing;
		read_instruction(vm, i);
	}
}

int				read_champ(t_vm *vm, char **input, int i)
{
	t_play		*champ;

	if (vm->players.len == 4)
		exit_vm(vm, gb_add(&vm->gb, ft_cprintf("Too many champions !")));
	champ = &vm->players.d[vm->players.len++];
	champ->id = RESERVED_ID;
	if (!ft_strcmp(input[i], "-n") && ++i)
	{
		if (input[i] == NULL
			|| ft_strlen(input[i]) > 10
			|| ((champ->id = ft_atoi(input[i])) == RESERVED_ID))
			exit_vm(vm, gb_add(&vm->gb,
					ft_cprintf("Champ. id badly formatted ('%s')", input[i])));
		++i;
	}
	vecproc_push_empty(&vm->gb, &vm->procs);
	vm->procs.d[vm->procs.len - 1].play = vm->players.len - 1;
	champ->fname = input[i];
	return (i);
}
