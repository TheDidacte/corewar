/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/02 14:51:50 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 21:28:01 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include "vm.h"

const t_vm_op g_op[16] = {
	{
		"live", 1, {
			T_DIR}
		, 1, 10, 0, 0, 0, 0, op_live}
	, {
		"ld", 2, {
			T_DIR | T_IND, T_REG}
		, 2, 5, 1, 1, 0, 0, op_ld}
	, {
		"st", 2, {
			T_REG, T_IND | T_REG}
		, 3, 5, 1, 0, 0, 0, op_st}
	, {
		"add", 3, {
			T_REG, T_REG, T_REG}
		, 4, 10, 1, 1, 0, 0, op_add}
	, {
		"sub", 3, {
			T_REG, T_REG, T_REG}
		, 5, 10, 1, 1, 0, 0, op_sub}
	, {
		"and", 3, {
			T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}
		, 6, 6, 1, 1, 0, 0, op_and}
	, {
		"or", 3, {
			T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}
		, 7, 6, 1, 1, 0, 0, op_or}
	, {
		"xor", 3, {
			T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}
		, 8, 6, 1, 1, 0, 0, op_xor}
	, {
		"zjmp", 1, {
			T_DIR}
		, 9, 20, 0, 0, 1, 0, op_zjmp}
	, {
		"ldi", 3, {
			T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}
		, 10, 25, 1, 0, 1, 0, op_ldi}
	, {
		"sti", 3, {
			T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}
		, 11, 25, 1, 0, 1, 0, op_sti}
	, {
		"fork", 1, {
			T_DIR}
		, 12, 800, 0, 0, 1, 0, op_fork}
	, {
		"lld", 2, {
			T_DIR | T_IND, T_REG}
		, 13, 10, 1, 1, 0, 1, op_lld}
	, {
		"lldi", 3, {
			T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}
		, 14, 50, 1, 1, 1, 1, op_lldi}
	, {
		"lfork", 1, {
			T_DIR}
		, 15, 1000, 0, 0, 1, 1, op_lfork}
	, {
		"aff", 1, {
			T_REG}
		, 16, 2, 1, 0, 0, 0, op_aff}
	,
}
;

void	clean_visu(t_vm *vm)
{
	delwin(vm->visu.arenaw);
	delwin(vm->visu.sidep.rootw);
	delwin(vm->visu.sidep.statusw);
	delwin(vm->visu.sidep.printw);
	echo();
	nocbreak();
	curs_set(TRUE);
	reset_shell_mode();
	endwin();
}

/*
**	Lots of times we pass strerror() to exit_vm,
**	this function should return a const char* (according to man),
**		but macOS is a special little snowflake and returns a malloced string.
*/

void	exit_vm(t_vm *env, char *err_msg)
{
	if (env->verbosity == VE_VISU)
		clean_visu(env);
	write(2, "Error:\n", 7);
	write(2, err_msg, ft_strlen(err_msg));
	write(2, "\n", 1);
	gb_freeall(&env->gb);
	exit(EXIT_FAILURE);
}

int		usage(const char *pname)
{
	ft_printf("<bold>Usage: %s"
		" [-visu | -verbose] [-rand]"
		" [-dump nbr_cycles | -ndump width nbr_cycles]"
		" [[-n number] champion.cor]<rst>\n"
		"\t-dump        : Dumps the VMs 'RAM' after nbr_cycles, 32bits/line\n"
		"\t-ndump       : Dumps the VMs 'RAM' after nbr_cycles, nbits/line\n"
		"\t-rand        : Fill the VM mem w/ random values, to test stability\n"
		"\t-vi(su)      : enable the ncurses visualizer mode\n"
		"\t-ve(rbosity) : verbosity level, it is cumulative\n"
		"\t\t0: winner (Player X (champion_name) won)\n"
		"\t\t1: aff\n"
		"\t\t2: live (A process shows that player X (champion_name) is alive)"
		"{red}<- DEFAULT SETTING{eoc}\n"
		"\t\t3: greetings message (list of competitors)\n"
		"\t\t4: player death\n"
		"\t\t5: process creation-death\n"
		"\t\t6: OPs\n"
		"\t\t7: Show Cycles\n"
		"\t\t8: Print registers at start of cycle (r17-20 are special regs)\n"
		"\t###### CONTROLS IN THE VISUALIZER ######\n"
		"\t\tQ: -10cycle/s  W: -1cycle/s  E: +1cycle/s  R: +10 cycle/s\n"
		"\t\t1: 50cycle/s  2: ~9999 cycle/s\n"
		"\t\tspace: pause/unpause\n"
		"\t\tesc: quit\n", pname);
	return (0);
}

void	print_greetings(t_vm *vm)
{
	int			i;

	ft_printf("Introducing contestants...\n");
	i = -1;
	while (++i < vm->players.len)
	{
		ft_printf("* Player %i, weighing %ld bytes, \"%s\" (\"%s\") !\n",
			vm->players.d[i].id,
			vm->players.d[i].head.prog_size,
			vm->players.d[i].head.prog_name,
			vm->players.d[i].head.comment);
	}
}

int		main(int argc, char **argv)
{
	t_vm		vm;

	if (argc == 1)
		return (usage(argv[0]));
	ft_bzero(&vm, sizeof(vm));
	gb_init_existing(&vm.gb);
	vm.verbosity = VE_LIVE;
	read_argv_init(&vm, argc, argv);
	if (vm.verbosity >= VE_GREET)
		print_greetings(&vm);
	if (vm.verbosity == VE_VISU)
		visu_loop(&vm);
	else
		loop(&vm);
	print_winner(&vm);
	if (vm.verbosity == VE_VISU)
		clean_visu(&vm);
	gb_freeall(&vm.gb);
	return (0);
}
