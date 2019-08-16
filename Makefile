# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/26 22:06:19 by marvin            #+#    #+#              #
#    Updated: 2019/05/26 22:42:28 by awoimbee         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
###############                  VARIABLES                       ###############
################################################################################

NAME =	corewar
ASM  =	asm
DISA =	disassembler

CFLAGS	=	-g3 -Wall -Wextra -Ofast -march=native#-fsanitize=address #-Werror -Ofast -march=native

SRC_NAME_CORE =	main.c								\
				dump_memory.c						\
				swap_endianess.c					\
				loop.c								\
				arena_mem_load.c					\
				arena_mem_write.c					\
				run_cycle.c							\
				circular_mem.c						\
				winner.c							\
				\
				init/read_argv.c					\
				init/read_champ.c					\
				init/visu_init.c					\
				init/set_dump.c						\
				\
				visu/visu_update.c					\
				visu/visu_memview.c					\
				visu/visu_sidepview.c				\
				\
				ops/load_arg_into_regs.c			\
				ops/read_one_arg.c					\
				ops/ops1.c							\
				ops/ops2.c							\
				ops/ops3.c							\
				ops/ops4.c							\
				\
				operators/vecproc/push.c			\
				operators/vecproc/push_empty.c		\
				operators/vecproc/realloc.c			\
				operators/vecproc/vector_del_dead.c

SRC_NAME_ASM =	asm.c					\
				get_arg_type.c			\
				write.c					\
				labels.c				\
				helper.c				\
				handle_op.c				\
				get_asm.c				\
				update_char.c			\
				utils.c

SRC_NAME_DISA = main.cpp

ASM_FD = asm/
CORE_FD = vm/
DISA_FD = disassembler/

ASM_SRC_SUBFOLDERS =
CORE_SRC_SUBFOLDERS =	operators/vecproc		\
						visu					\
						init					\
						ops
BUILD_FOLDER =	build

################################################################################
###############                  CONSTANTS                       ###############
################################################################################
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	NUMPROC := $(shell grep -c ^processor /proc/cpuinfo)
	EXEC_RPATH := \$$ORIGIN
else ifeq ($(UNAME_S),Darwin)
	NUMPROC := $(shell sysctl hw.ncpu | awk '{print $$2}')
	EXEC_RPATH := @loader_path #https://blogs.oracle.com/dipol/dynamic-libraries,-rpath,-and-mac-os
endif

CC = gcc
CXX = g++ -std=c++14
LDLIBS = -lft -lncurses -lpthread
LDFLAGS = -L./libft
CFLAGS += -MMD -I./includes -I./libft

SRC_FOLDERS =	$(addprefix $(ASM_FD), $(ASM_SRC_SUBFOLDERS))		\
				$(addprefix $(CORE_FD), $(CORE_SRC_SUBFOLDERS))		\
				$(ASM_FD) $(CORE_FD) $(DISA_FD)

REPO_PATH = $(patsubst %/,%,$(dir $(realpath $(firstword $(MAKEFILE_LIST)))))
BUILD_PATH = $(REPO_PATH)/$(BUILD_FOLDER)
SRC_PATH =	srcs
OBJ_FOLDER = $(BUILD_FOLDER)/obj
LFT = libft/libft.a

OBJ_CORE = $(addprefix $(OBJ_FOLDER)/$(CORE_FD), $(SRC_NAME_CORE:.c=.c.o))
OBJ_ASM  = $(addprefix $(OBJ_FOLDER)/$(ASM_FD),  $(SRC_NAME_ASM:.c=.c.o))
OBJ_DISA = $(addprefix $(OBJ_FOLDER)/$(DISA_FD), $(SRC_NAME_DISA:.cpp=.cpp.o))

################################################################################
#################                  RULES                       #################
################################################################################
all : $(LFT)
	@$(MAKE) -j$(NUMPROC) $(NAME)   --no-print-directory
	@$(MAKE) -j$(NUMPROC) $(ASM) --no-print-directory
	@$(MAKE) disassembler --no-print-directory

############## LIBS ############
$(LFT) :
	@printf "$(YLW)Making libft...$(EOC)\n"
	@$(MAKE) -j$(NUMPROC) -s -C libft/
################################

$(DISA) : $(LFT) $(OBJ_DISA)
	@printf "$(GRN)Linking $@...$(EOC)\n"
	$(CXX) $(CFLAGS) $(OBJ_DISA) -o $@ $(LDFLAGS) $(LDLIBS)

$(NAME) : $(LFT) $(OBJ_CORE)
	@printf "$(GRN)Linking $@...$(EOC)\n"
	$(CC) $(CFLAGS) $(OBJ_CORE) -o $@ $(LDFLAGS) $(LDLIBS)

$(ASM)	: $(LFT) $(OBJ_ASM)
	@printf "$(GRN)Linking $@...$(EOC)\n"
	$(CC) $(CFLAGS) $(OBJ_ASM) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD_FOLDER) :
	@mkdir -p $(BUILD_PATH) 2> /dev/null
	@mkdir -p $(OBJ_FOLDER) 2> /dev/null
	@mkdir -p $(addprefix $(OBJ_FOLDER)/,$(SRC_FOLDERS)) 2> /dev/null

$(BUILD_FOLDER)/obj :
	mkdir -p $(BUILD_FOLDER) $(BUILD_FOLDER)/obj 2> /dev/null
	mkdir -p $(addprefix $(OBJ_FOLDER)/,$(SRC_FOLDERS)) 2> /dev/null

#objects
$(OBJ_FOLDER)/%.c.o : $(SRC_PATH)/%.c | $(BUILD_FOLDER)/obj
	@printf "\t$(CC) (...) $@\n"
	@$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_FOLDER)/%.cpp.o : $(SRC_PATH)/%.cpp | $(BUILD_FOLDER)/obj
	@printf "\t$(CXX) (...) $@\n"
	@$(CXX) $(CFLAGS) -o $@ -c $<

# Add rules written in .d files (by gcc -MMD)
# The '-' makes it doesn't care if the file exists or not
-include $(OBJ_CORE:.o=.d) $(OBJ_ASM:.o=.d)

obj_clean :
	@printf "$(RED)Cleaning $(OBJ_FOLDER)$(EOC)\n"
	@rm -rf $(OBJ_FOLDER)

clean :
	@printf "$(RED)Cleaning $(BUILD_FOLDER)$(EOC)\n"
	@rm -rf $(BUILD_FOLDER)
	@printf "$(YLW)Cleaning libft...$(EOC)\n"
	@make -s fclean -C libft

fclean : clean
	@printf "$(RED)Cleaning $(NAME) & $(ASM) & $(DISA)$(EOC)\n"
	@rm -f $(NAME)
	@rm -f $(ASM)
	@rm -f $(DISA)

sclean	:	obj_clean
re		:	fclean		all
sre		:	obj_clean		all

.PHONY: all obj_clean clean fclean re sre

RED = \033[1;31m
GRN = \033[1;32m
YLW = \033[1;33m
INV = \033[7m
EOC = \033[0m
