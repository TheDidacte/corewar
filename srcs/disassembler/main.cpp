/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/26 16:35:08 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/26 22:48:59 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <istream>
#include <fstream>

extern "C" {
#include "corewar.h"
#include "vm.h"
#include "asm.h"
}
#undef TRUE
#undef FALSE

size_t	incrementPc(const std::vector<uint8_t>& progBuff, size_t pc, size_t add);

const t_op	g_op_tab[17] = {
/*  name    nbarg          args_types                                  ic  cycles  desc            cod_byte  dir2  */
	{"live",  1, {T_DIR},                                               1,  10,   "alive",           FALSE, FALSE},
	{"ld",    2, {T_DIR | T_IND, T_REG},                                2,  5,    "load",            TRUE, FALSE},
	{"st",    2, {T_REG, T_IND | T_REG},                                3,  5,    "store",           TRUE, FALSE},
	{"add",   3, {T_REG, T_REG, T_REG},                                 4,  10,   "addition",        TRUE, FALSE},
	{"sub",   3, {T_REG, T_REG, T_REG},                                 5,  10,   "soustraction",    TRUE, FALSE},
	{"and",   3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, 6,  6,    "and",             TRUE, FALSE},
	{"or",    3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 7,  6,    "or",              TRUE, FALSE},
	{"xor",   3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 8,  6,    "xor",             TRUE, FALSE},
	{"zjmp",  1, {T_DIR},                                               9,  20,   "jump if zero",    FALSE, TRUE},
	{"ldi",   3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},         10, 25,   "load index",      TRUE, TRUE},
	{"sti",   3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG},         11, 25,   "store index",     TRUE, TRUE},
	{"fork",  1, {T_DIR},                                               12, 800,  "fork",            FALSE, TRUE},
	{"lld",   2, {T_DIR | T_IND, T_REG},                                13, 10,   "long load",       TRUE, FALSE},
	{"lldi",  3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},         14, 50,   "long load index", TRUE, TRUE},
	{"lfork", 1, {T_DIR},                                               15, 1000, "long fork",       FALSE, TRUE},
	{"aff",   1, {T_REG},                                               16, 2,    "aff",             TRUE, FALSE},
	{0, 0, {0}, 0, 0, 0, FALSE, FALSE}
};

void	quit(const char* errMsg) {
	if (errMsg)
		fprintf(stderr, "%s\n", errMsg);
	std::remove("disassembly.s");
	exit(EXIT_FAILURE);
}

uint32_t	swap32_endian(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

int32_t			load32(const std::vector<uint8_t>& progBuff, size_t& pc)
{
	int32_t		res;
	uint8_t		*r;
	int			i;

	r = (uint8_t*)&res;
	i = -1;
	while (++i < 4)
		r[i] = progBuff[pc + (3 - i)];
	pc = incrementPc(progBuff, pc, 4);
	return (res);
}

int16_t			load16(const std::vector<uint8_t>& progBuff, size_t& pc)
{
	int16_t		res;
	uint8_t		*r;

	r = (uint8_t*)&res;
	r[0] = progBuff[pc + 1];
	r[1] = progBuff[pc];
	pc = incrementPc(progBuff, pc, 2);
	return (res);
}

int8_t			load8(const std::vector<uint8_t>& progBuff, size_t& pc)
{
	int8_t		res;

	res = (int8_t)progBuff[pc];
	pc = incrementPc(progBuff, pc, 1);
	return (res);
}





static int		load_reg(
	const t_op& op,
	const std::vector<uint8_t>& progBuff,
	size_t& pc,
	int argId,
	std::ofstream& out
) {
	int		tmp;

	tmp = load8(progBuff, pc);
	out << " r" << tmp;
	if (!(tmp <= REG_NUMBER && tmp >= 1 && (T_REG & op.args_types[argId]))) {
		fprintf(stderr, "Error: argument %d (r%d) bad or not compatible with %s\n", argId, tmp, op.name);
		quit(nullptr);
	}
	return (0);
}

static int		load_dir(
	const t_op& op,
	const std::vector<uint8_t>& progBuff,
	size_t& pc,
	int argId,
	std::ofstream& out
) {
	int tmp;
	if (op.dir2)
		tmp = load16(progBuff, pc);
	else
		tmp = load32(progBuff, pc);
	out << " %" << tmp;
	if (!(T_DIR & op.args_types[argId])) {
		fprintf(stderr, "Error: argument %d (%%%d) bad or not compatible with %s\n", argId, tmp, op.name);
		quit(nullptr);
	}
	return (0);
}

static int		load_ind(
	const t_op& op,
	const std::vector<uint8_t>& progBuff,
	size_t& pc,
	int argId,
	std::ofstream& out
) {
	int		ind;

	ind = load16(progBuff, pc);
	out << " " << ind;
	if (!(T_IND & op.args_types[argId])) {
		fprintf(stderr, "Error: argument %d (%d) bad or not compatible with %s\n", argId, ind, op.name);
		quit(nullptr);
	}
	return (0);
}


int		read_one_arg(
	const t_op& op,
	const std::vector<uint8_t>& progBuff,
	size_t& pc,
	uint8_t cbArg,
	int argId,
	std::ofstream& out
) {
	switch (cbArg) {
		case REG_CODE:
			return (load_reg(op, progBuff, pc, argId, out));
		case DIR_CODE:
			return (load_dir(op, progBuff, pc, argId, out));
		case IND_CODE:
			return (load_ind(op, progBuff, pc, argId, out));
		default:
			quit("Error: invalid coding byte, cannot continue");
	}
	return (-1);
}

size_t	incrementPc(const std::vector<uint8_t>& progBuff, size_t pc, size_t add)
{
	pc += add;
	if (pc > progBuff.size()) {
		quit("Error: unexpected end of file.");
	}
	return (pc);
}

int		main(int argc, char **argv)
{
	if (argc != 2) {
		std::cout << "usage: " << argv[0] << " <champ.cor>" << std::endl;
		return (EXIT_SUCCESS);
	}

	int fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		quit(strerror(errno));
	size_t size = lseek(fd, 0, SEEK_END);
	if (size <= sizeof(t_header))
		quit("Error: too small/empty file");
	else if (size > CHAMP_MAX_SIZE * 10)
		quit("Error: woah, that's a really big file, don't you think ?");
	std::vector<uint8_t>  fileBuff(size, 0);

	lseek(fd, 0, SEEK_SET);
	read(fd, &fileBuff[0], size);
	std::ofstream out("disassembly.s", std::fstream::trunc);

	const t_header*             head = reinterpret_cast<const t_header*>(&fileBuff[0]);
	const std::vector<uint8_t>  progBuff(fileBuff.begin() + sizeof(t_header), fileBuff.end());
	// const std::vector<uint8_t>  progBuff = slice(fileBuff, sizeof(t_header), fileBuff.size());

	if (fileBuff.size() < sizeof(t_header))
		quit("Error: champion is too small, disassembly is impossible.");
	if (progBuff.size() > CHAMP_MAX_SIZE)
		fprintf(stderr, "Warning : Champion is too big for the VM\n");

	/* NEED TO CHECK THE HEADER !! */
	/* Read the header */
	if (swap32_endian(head->prog_size) != progBuff.size())
		fprintf(stderr, "Warning: Program size does not mach.\n");
	if (swap32_endian(head->magic) != COREWAR_EXEC_MAGIC)
		fprintf(stderr, "Warning: exec magic does not mach.\n");
	out << ".name \"" << head->prog_name << "\"" << std::endl;
	out << ".comment \"" << head->comment << "\"" << std::endl;
	out << std::endl;

	/* Read the prog */
	for (size_t pc = 0; pc < progBuff.size();) {
		if (progBuff[pc] > 16 || progBuff[pc] < 1) {
			fprintf(stderr, "Error: illegal instruction: %#hhx, cannot continue.\n", progBuff[pc]);
			quit(nullptr);
		}
		const t_op& op = g_op_tab[progBuff[pc] - 1];
		pc = incrementPc(progBuff, pc, 1);
		out << op.name;

		if (op.coding_byte == TRUE) {
			const uint8_t& cb = progBuff[pc];
			pc = incrementPc(progBuff, pc, 1);
			for (int i = 0; i < op.nb_args; ++i) {
				read_one_arg(op, progBuff, pc, (cb >> (6 - i * 2)) & 0b11, i, out);
			}
		}
		else if (op.coding_byte == FALSE) {
			read_one_arg(op, progBuff, pc, T_DIR, 0, out);
		}
		out << std::endl;
		// pc += 1;
	}
	out.close();
	printf("Source successfully written in ./disassembly.s !\n");
	return (0);
}
