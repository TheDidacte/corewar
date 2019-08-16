/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:24:23 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/29 21:28:06 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H
# include "corewar.h"
# include "libft.h"
# include <ncurses.h>

/*
**	################
**	    Defines
**	################
*/

# define RESERVED_ID	-2147483648
# define TIME_OF_WRITE	0.3
# define OUR_REG_NB	(REG_NUMBER + 1 + MAX_ARGS_NUMBER)

/*
**	Verbosity levels are defined so new ones can be added in between easily
*/

# define VE_BAD			-999
# define VE_VI_NOTINIT	-2
# define VE_VISU		-1
# define VE_WINNER		0
# define VE_AFF			1
# define VE_LIVE		2
# define VE_GREET		3
# define VE_PLAYDEATH	4
# define VE_PROCDEATH	5
# define VE_OPS			6
# define VE_CYCLE		7
# define VE_REGISTER	8
# define VE_PC_MOVE		9

/*
**	Ncurses sucks and doesnt define all the colors nor the keys
*/

# define COLOR_BRIGHT_BLACK		8
# define COLOR_BRIGHT_RED		9
# define COLOR_BRIGHT_GREEN		10
# define COLOR_BRIGHT_YELLOW	11
# define COLOR_BRIGHT_BLUE		12
# define COLOR_BRIGHT_MAGENTA	13
# define COLOR_BRIGHT_CYAN		14
# define COLOR_BRIGHT_WHITE		15

# define NC_Q	113
# define NC_W	119
# define NC_E	101
# define NC_R	114
# define NC_SPC	32
# define NC_ESC	27
# define NC_1	49
# define NC_2	50

/*
**	Define the COLOR_PAIR we'll use for players, cursors & newly written mem,
**		PLAY0_COLOR-1 & CURS0_COLOR-1 are reserved for unset memory.
**	/!\ code assumes CONTOUR_COLOR < FRESH0_COLOR
*/

# define CONTOUR_COLOR		1
# define PLAY0_COLOR		3
# define DELT_CURS_COLOR	10
# define CURS0_COLOR		13
# define DELT_FRESH_COLOR	20
# define FRESH0_COLOR		23

# define AFF_BUFF_LEN	245

/*
**	#################
**	   Declarations
**	#################
*/

typedef int32_t	t_register;
struct s_proc;
struct s_play;
struct s_vecproc;
struct s_vecplay;
struct s_vm;

/*
**	################
**	   Definitions
**	################
*/

/*
**	t_proc is only 32bytes,
**	which means each proces can be copied in a single AVX register !
**	->it would be a bit faster than using ft_memcpy
**	But anyways, wht's slowing us down is the grabage collector..
*/

typedef struct	s_proc
{
	int					pc;
	uint8_t				op_id;
	uint8_t				play;
	int16_t				new_pc;
	t_register			*reg;
	uint				pid;
	int					live;
	int16_t				op_cycles;
	int8_t				carry;
}				t_proc;

typedef struct	s_vecproc
{
	struct s_proc		*d __attribute__((aligned(16)));
	int					last_pid;
	int					len;
	int					mem;
}				t_vecproc;

typedef struct	s_play
{
	int					id;
	int					last_live;
	int					period_lives;
	char				*fname;
	t_header			head;
}				t_play;

typedef struct	s_vecplay
{
	struct s_play		d[MAX_PLAYERS];
	int					len;
}				t_vecplay;

/*
**							rootw
**	+-------------------------+------------------------------+
**	|                         |        sidepw->rootw         |
**	|                         |                              |
**	|                         |  +------------------------+  |
**	|                         |  |                        |  |
**	|                         |  |                        |  |
**	|                         |  |        statusw         |  |
**	|         arenaw          |  |                        |  |
**	|                         |  |                        |  |
**	|                         |  |                        |  |
**	|                         |  +------------------------+  |
**	|                         |  +------------------------+  |
**	|                         |  |        printw          |  |
**	|                         |  +------------------------+  |
**	+-------------------------+------------------------------+
*/

typedef struct	s_visu
{
	float				op_per_sec;
	t_bool				paused;
	WINDOW				*rootw;
	WINDOW				*arenaw;
	char				aff[AFF_BUFF_LEN];
	struct		s_sidep
	{
		WINDOW				*rootw;
		WINDOW				*statusw;
		WINDOW				*printw;
	}					sidep;
}				t_visu;

/*
**	OPs
**		in here, id == op_code
*/

typedef void(*t_opfun)(struct s_vm*, int, int[3]);

typedef struct	s_vm_op
{
	const char			*name;
	int					nb_args;
	t_arg_type			args_type[MAX_ARGS_NUMBER];
	int					id;
	int					cycles;
	t_bool				coding_byte;
	t_bool				modcarry;
	t_bool				dir2;
	t_bool				ldx_rel;
	t_opfun				fun;
}				t_vm_op;

extern const t_vm_op g_op[16];

/*
**	#################
**	  MAIN STRUCT
**	#################
*/

typedef struct	s_vm
{
	t_visu				visu;
	int					verbosity;
	int					cycle_dump;
	int					dump_width;
	int					cycle_die;
	int					cycle_curr;
	int					die_cycle_checks;
	int					cycle_last_check;
	struct s_vecplay	players;
	struct s_vecproc	procs;
	struct s_garbage	gb;
	char				*tmpstr;
	uint8_t				arena[MEM_SIZE];
	uint8_t				mem_owner[MEM_SIZE];
	clock_t				time_write[MEM_SIZE];
}				t_vm;

/*
**	##################
**	    OPERATORS
**	##################
*/

t_vecproc		*vecproc_push_empty(t_garbage *gb, t_vecproc *vec);
t_vecproc		*vecproc_push(t_garbage *gb, t_vecproc *vec, uint src);
t_vecproc		*vecproc_realloc(t_garbage *gb, t_vecproc *vec);
void			vecproc_del_dead(t_garbage *gb, t_vecproc *v);

/*
**	##################
**	    FUNCTIONS
**	##################
*/
uint32_t		swap32_endian(uint32_t val);
uint16_t		swap16_endian(uint16_t val);
void			dump_memory(t_vm *vm);

/*
**	arena_mem_*.c
**		The write functions need the procs PC
**		to be located on the instructions id byte.
*/
int32_t			load32(t_vm *vm, t_register pc);
int16_t			load16(t_vm *vm, t_register pc);
int8_t			load8(t_vm *vm, t_register pc);
uint8_t			*write32(t_vm *vm, int pc, int aptr, uint32_t data);
uint8_t			*write16(t_vm *vm, int pc, int aptr, uint16_t data);
uint8_t			*write8(t_vm *vm, int pc, int aptr, uint8_t data);
int				circumem(int ptr);

int				usage(const char *pname);
void			exit_vm(t_vm *env, char *err_msg);

void			read_argv_init(t_vm *env, int argc, char **argv);
void			print_register(t_vm *vm, t_proc *proc);
void			print_winner(t_vm *vm);

/*
**	Functions that actually run the vm
*/
int				run_vm_cycle(t_vm *vm);
void			loop(t_vm *env);
void			visu_loop(t_vm *vm);
void			visu_endloop(t_vm *vm, int winner);

/*
**	visu
*/
void			clean_visu(t_vm *vm);
void			visu_update(t_vm *vm);
void			visu_khandler(t_vm *vm);
void			visu_init_memview(t_vm *vm);
void			visu_memview(t_vm *vm);
void			visu_sidepview(t_vm *vm);

/*
**	OPs
*/
void			read_instruction(t_vm *vm, int proc);
int				read_one_arg(t_vm *vm, t_proc *proc, uint8_t cb, int cur_arg);
int				load_arg_into_regs(t_vm *vm, t_proc *proc, int reg_num[3]);

void			op_live(t_vm *vm, int proc, int reg_num[3]);
void			op_ld(t_vm *vm, int proc, int reg_num[3]);
void			op_st(t_vm *vm, int proc, int reg_num[3]);
void			op_add(t_vm *vm, int proc, int reg_num[3]);
void			op_sub(t_vm *vm, int proc, int reg_num[3]);
void			op_and(t_vm *vm, int proc, int reg_num[3]);
void			op_or(t_vm *vm, int proc, int reg_num[3]);
void			op_xor(t_vm *vm, int proc, int reg_num[3]);
void			op_zjmp(t_vm *vm, int proc, int reg_num[3]);
void			op_ldi(t_vm *vm, int proc, int reg_num[3]);
void			op_sti(t_vm *vm, int proc, int reg_num[3]);
void			op_fork(t_vm *vm, int proc, int reg_num[3]);
void			op_lld(t_vm *vm, int proc, int reg_num[3]);
void			op_lldi(t_vm *vm, int proc, int reg_num[3]);
void			op_lfork(t_vm *vm, int proc, int reg_num[3]);
void			op_aff(t_vm *vm, int proc, int reg_num[3]);
#endif
