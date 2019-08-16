/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 17:17:57 by mbenjell          #+#    #+#             */
/*   Updated: 2019/05/08 20:55:36 by mbenjell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int				main(int ac, char **av)
{
	int 		fd;
	int 		output;
	ssize_t 	i = 0;
	ssize_t 	j = 1;
	t_stream 	s;
	char 		c;

	while (j < 1000000)
	{
		fd = open(av[1], O_RDONLY);
		output = open("output.s", O_CREAT | O_WRONLY | O_TRUNC, 0644);
		int k = 0;
		init_stream(&s, fd);
		while (k < j)
		{
			c = ft_getc(&s);
			write(output, &c, 1);
			k++;
		}
		j++;
		close(fd);
		close(output);
		int ret = system("valgrind ./asm output.s");
		if (ret != 0 && ret != 256)
		{
			system("cp output.s crash2");
			printf("signal = %d\n", ret);
			exit(1);
		}
		// system("valgrind ./asm output.s");
		system("rm -rf output.s");
	}
	return (0);
}

// lancer comme ca : ./test [nom_champion]
// Pour voir le fichier output.s en temps reel
// while [ true ] ; do cat output.s ; echo "" ; clear ; done

// nettoyer des .s des tab : :%s/ \+/ /g
// nettoyer des .s des tab : :%s/\t\+/\t/g
