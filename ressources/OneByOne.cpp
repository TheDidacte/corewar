/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OneByOne.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoimbee <awoimbee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 14:36:17 by awoimbee          #+#    #+#             */
/*   Updated: 2019/05/27 15:14:54 by awoimbee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// g++ -g -std=c++14 -Wall -Wextra -Wpedantic OneByOne.cpp -o donateToAwoimbee

#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <istream>
#include <fstream>
#include <unistd.h>

#define TMP_FILE "onebyone.tmp"
#define MAX_FSIZE (1024 * 1024 * 1024) // 1 gigabyte

void	quit(const char* errMsg) {
	if (errMsg)
		fprintf(stderr, "%s\n", errMsg);
	std::remove(TMP_FILE);
	exit(EXIT_FAILURE);
}

int		main(
	int          argc,
	const char** argv
) {
	/* USAGE */
	if (argc != 3) {
		fprintf(stdout,
			"usage: %s <prog> <input_file> [<args>]\n"
			"\tExemples:\n"
			"\t\t %s ./corewar ./champ.cor '-dump 5'\n"
			"\t\t %s './corewar -dump 5' ./champ.cor\n",
			argv[0], argv[0], argv[0]);
		return (EXIT_SUCCESS);
	}

	/* READ INPUT FILE & CREATE TMP FILE STREAM */
	int                  fd;
	size_t               size;
	std::vector<uint8_t> fileBuff;
	std::ofstream        out;

	fd = open(argv[2], O_RDONLY);
	if (fd == -1)
		quit(strerror(errno));
	size = lseek(fd, 0, SEEK_END);
	if (size == 0 || size > MAX_FSIZE)
		quit("Error: input file is empty or too big.");
	fileBuff.resize(size, 0);
	lseek(fd, 0, SEEK_SET);
	read(fd, &fileBuff[0], size); //we use read because c++ streams are hard to protect against /dev/*
	try {
		out.open(TMP_FILE, std::fstream::trunc);
	}
	catch (std::ios_base::failure& e) {
		quit(e.what());
	}

	/* THE MAGIC SAUCE */
	std::vector<char> cmd;
	int               ret;

	if (argv[2] == nullptr)
		argv[2] = "";    // yolooo
	cmd.resize(
		strlen(argv[0])
		+ strlen(argv[1])
		+ strlen(argv[2]));

	for (size_t i = 0; i < fileBuff.size(); ++i) {
		out << fileBuff[i];
		sprintf(&cmd[0], "%s %s %s", argv[0], argv[1], argv[2]);
		ret = system(&cmd[0]);
		if (ret != 0 && ret != 1) {
			fprintf(stderr,
				"Child process terminated with return value %d\n"
				"You can check " TMP_FILE " to investigate the issue\n", ret);
			return (0);
		}
	}
	out.close();
	printf("Test successfull !\n");
	return (0);

}
