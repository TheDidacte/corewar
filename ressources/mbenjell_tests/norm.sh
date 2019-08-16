# The Norm (** extrait **)

echo "****************************************************************"
echo "                   Operateurs en debut de ligne.                "
echo "****************************************************************"

echo ""

# Vous pouvez retourner à la ligne lors d’une même instruction ou structure de contrôle
# mais vous devez rajouter une indentation par parenthèse ou opérateur d’affectation.
# Les opérateurs doivent être en début de ligne

grep -E '[=+\-\*&\^%><\/]$' **/*.c

read -p "==========================> NEXT  <=========================="

echo "****************************************************************"
echo "aligner le nom du typedef avec le nom de la struct,union ou enum"
echo "****************************************************************"

echo ""

read -p "==========================> NEXT  <=========================="

# Lorsque vous déclarez une struct, union ou enum avec un typedef, toutes les
# règles s’appliquent et vous devez aligner le nom du typedef avec le nom de la struct,union ou enum.

# Checker a la loupe...

vim **/*.h

# ******************************

# II.2 Formatage

echo "****************************************************************"
echo "						header standard de 42					  "
echo "****************************************************************"

echo ""

# Tous vos fichiers devront commencer par le header standard de 42 dès la première
# ligne. Ce header est disponible par defaut dans les éditeurs emacs et vim sur les dumps.

ls -1 *.[ch] **/Makefile | wc -l
grep Created *.[ch] **/Makefile | wc -l

read -p "==========================> NEXT  <=========================="

# Vous devez indenter votre code avec des tabulations de la taille de 4 espaces.
# Ce n’est pas équivalent à 4 espace, ce sont bien des tabulations.

# ======== > TO DO

# Une seule instruction par ligne.

echo "****************************************************************"
echo " Les étoiles des ptr doivent être collés au nom de la variable. "
echo "****************************************************************"

echo ""

# Les étoiles des pointeurs doivent être collés au nom de la variable.

grep sizeof **/*.[ch]

read -p "==========================> NEXT  <=========================="

# II.3 Paramètres de fonction

echo "****************************************************************"
echo " 		fonction sans param prototypée avec le mot clef void 	  "
echo "****************************************************************"

echo ""

# Une fonction qui ne prend pas d’argument doit explicitement être prototypée avec le mot void comme argument.

grep "()" **/*.[ch]

read -p "==========================> NEXT  <=========================="

# II.4 Fonctions

echo "****************************************************************"
echo " 	  Le retour d’une fonction doit se faire entre parenthèses    "
echo "****************************************************************"

echo ""

# Le retour d’une fonction doit se faire entre parenthèses.

grep "return" **/*.c

read -p "==========================> NEXT  <=========================="

# II.5 Typedef, struct, enum et union Partie obligatoire

# Lorsque vous déclarez une struct, union ou enum avec un typedef, toutes les
# règles s’appliquent et vous devez aligner le nom du typedef avec le nom de la struct, union ou enum.

# II.6 Headers

# Tous les includes de .h doivent se faire au début du fichier (.c ou .h).

echo "****************************************************************"
echo " 				  includes au debut des .h  					  "
echo "****************************************************************"

echo ""

# Une inclusion de header (.h) dont on ne se sert pas est interdite.

echo "****************************************************************"
echo "Constantes de préprocesseur (#define) juste des chaines/valeurs "
echo "****************************************************************"

echo ""

# Les constantes de préprocesseur (#define) que vous créez ne doivent être
# utilisés que pour associer des valeurs littérales et constantes, et rien d’autre.

grep define **/*.[ch]

read -p "==========================> NEXT  <=========================="

# Les #define rédigés dans le but de contourner la norme et/ou obfusquer du code
# interdit par la norme sont interdites. Ce point doit être vérifiable par des humains.

echo "****************************************************************"
echo "			Seuls les noms de macros sont en majuscules 	      "
echo "****************************************************************"

echo ""

# Seuls les noms de macros sont en majuscules

grep -n10 -E 'enum' **/*.[ch]

read -p "==========================> NEXT  <=========================="

# Les tableaux à taille variable (VLA - Variable Length Array)

echo "****************************************************************"
echo "							VLA interdit 					      "
echo "****************************************************************"

echo ""

grep "];" *.[ch]


echo "****************************************************************"
echo "				Règles obligatoires dans le Makefile 	          "
echo "****************************************************************"

echo ""

# Les règles $(NAME), clean, fclean, re et all sont obligatoires.

grep "\$(NAME)" **/Makefile
grep "clean" **/Makefile
grep "fclean" **/Makefile
grep "re" **/Makefile
grep "all" **/Makefile

read -p "==========================> NEXT  <=========================="

echo "****************************************************************"
echo "							Makefile relink 				      "
echo "****************************************************************"

echo ""

# Le projet est considéré comme non fonctionnel si le Makefile relink.

touch main.c && make && ls -la **/*.o
ls -la doom-nukem
