/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/06 12:37:45 by mbenjell          #+#    #+#             */
/*   Updated: 2019/05/08 21:27:50 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <math.h>
# include <fcntl.h>
# include "minilibx/mlx.h"

#include <stdio.h>

# define SUCCESS	0
# define ERROR		-1
# define NB_STATES	21
# define SPACE				"\n\t "

# define BUFFER_SIZE		4096

typedef struct				s_stream
{
	char 					*file_name;
	int						fd;
	char					buff[BUFFER_SIZE];
	int						i;
	int						nread;
}							t_stream;

typedef enum		e_name
{
	start,
	state_1,
	state_2,
	state_3,
	end
}					t_name;

typedef void		t_pf(void);

typedef struct		s_state
{
	int				type;
	t_pf			*pf;
	struct s_state	**next_states;
	size_t			nb_transition;
	size_t			max;
}					t_state;

typedef struct		s_env
{
	t_state			*state;
}					t_env;

typedef struct		s_automaton
{
	void			*data;
	t_state			**states;
	int				nb_states;
}					t_automaton;

typedef enum 		e_test
{
	test1,
	test2,
	test3,
	test4,
	test5,
	test6,
	test7,
	test8,
	test9,
	test10,
	test11,
	test12,
	test13,
	test14,
	test15,
	test16,
	test17,
	test18,
	test19,
	test20,
	test21,
	test22,
	test23,
	test24
}					t_test;

t_state				*create_state(t_automaton *a, int type);
t_automaton			*create_automaton(void *data);
void				change_state(void *env, void *transition);
void				add_transition(t_state *src, t_state *dest);
void				run_automaton(t_automaton *a, t_state *start);

void				transition(void);
void				code(void);

int					init_stream(t_stream *s, int fd);
int					flusher(t_stream *s);
int					ft_getc(t_stream *s);
int					ft_getchar(t_stream *s);
int					skip_space(t_stream *s);

#endif
