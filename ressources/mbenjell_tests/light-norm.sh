grep -E '[=+\-\*&\^%><\/]$' **/*.c
ls -1 *.[ch] **/Makefile | wc -l
grep Created *.[ch] **/Makefile | wc -l
grep sizeof **/*.[ch]
grep "()" **/*.[ch]
grep "return" **/*.c
grep define **/*.[ch]
grep -n10 -E 'enum' **/*.[ch]
grep "];" **/*.[ch]
grep "\$(NAME)" **/Makefile
grep "clean" **/Makefie
grep "fclean" **/Makefile
grep "re" **/Makefile
grep "all" **/Makefile
touch main.c && make && ls -la **/*.o
ls -la doom-nukem
