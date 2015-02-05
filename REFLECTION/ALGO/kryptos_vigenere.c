/*
 **
 ** Vigenere algo,
 ** optimized for the KRYPTOS enigma
 ** Fast and leaks-free program (you can use it while BruteForcing keys).
 **
 ** - - - - -
 **
 ** The first key is "PALIMPSEST" (use it to solve the first part)
 ** The second one is "ABSCISSA" (use it to solve the second part)
 **
 ** - - - - -
 **
 ** Compile with:
 ** -> gcc kryptos_vigenere.c -o kryptos_vigenere
 **
 ** (c) Raphael Bobillot
 **
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define DECODE 32
#define ENCODE 64

int			flag;
char		**TABLE;
const char	*ALPHABET = "KRYPTOSABCDEFGHIJLMNQUVWXZ";

int			usage(void)
{
	printf("usage: \033[92m./kryptos_vigenere\033[0m <-e|-d> <key | wordlist> <string>\n");
	printf("\n\033[93mexamples\033[0m:\n");
	printf("\033[96m> \033[92m./kryptos_vigenere\033[0m -e \"maclef\" \"coucou\"\n");
	printf("\033[96m>\033[0m RFTKJA\n");
	printf("\033[96m> \033[92m./kryptos_vigenere\033[0m -e - \"coucou\"\n");
	printf("\033[96m>\033[0m DAXGDR\n");
	printf("\n----------------\n\n");
	printf("\033[95m> \033[92m./kryptos_vigenere\033[0m -d \"maclef\" \"RFTKJA\"\n");
	printf("\033[95m>\033[0m COUCOU\n");
	printf("\033[95m> \033[92m./kryptos_vigenere\033[0m -d - \"DAXGDR\"\n");
	printf("\033[95m>\033[0m COUCOU\n");
	return (-1);
}

int			check_args(int ac, char *av[])
{
	if (ac != 4)
		return (usage());
	else if (!(!strcmp(av[1], "-e") || !strcmp(av[1], "-d")))
		return (usage());
	else if (!av[3][0])
		return (usage());
	return (0);
}

void		print_res(char *s)
{
	int		i;

	for (i=0; s[i]; i++)
		if (s[i] != ' ')
			printf("%c", s[i]);
	printf("\n");
}

char		*str_toupper(char *str)
{
	int		i;

	for (i = 0; str[i]; i++)
		str[i] -= (str[i] >= 'a' && str[i] <= 'z') ? 0x20 : 0x00;
	return (str);
}

char		**get_key(char *av[])
{
	int		fd;
	char	**key = (char **)malloc(sizeof(char *) * (1 << 25));

	if ((fd = open(av[2], O_RDONLY)) < 0)
	{
		if (!av[2])
		{
			key[0] = strdup(ALPHABET);
			key[1] = NULL;
		}
		else
		{
			key[0] = (!av[2][0] || !strcmp(av[2], "-"))
				? strdup(ALPHABET) : strdup(str_toupper(av[2]));
			key[1] = NULL;
		}
	}
	else
	{
		char	c = 0;
		int		i = 0, j = 0;
		char	*tmp = (char *)malloc(sizeof(char) * 64);

		printf("\033[93mPreparing bruteforce...\n");
		while (read(fd, &c, 1) > 0)
		{
			if (c == '\n')
			{
				while (i < 63)
					tmp[i++] = 0;
				key[j++] = strdup(tmp);
				i = 0;
			}
			else
				tmp[i++] = c;
		}
		key[j] = NULL;
		free(tmp);
		printf("\033[92mDone...\033[0m\n\n");
	}
	return (key);
}

char		*rot_n(const char *str, int n)
{
	int		i, j;
	char	*s = strdup(str);

	for (i=0; i < n; i++)
		for (j=0; j < (int)strlen(s)-1; j++)
		{
			s[j] ^= s[j+1];
			s[j+1] ^= s[j];
			s[j] ^= s[j+1];
		}
	return (s);
}

int			get_pos(char *s, char c)
{
	int		i;

	for (i=1; s[i]; i++)
		if (s[i] == c)
			return (i);
	return (0);
}

char		*VIGENERE(char *src, char **table)
{
	int		i, j, k;
	char	*res;

	res = (char *)malloc(sizeof(char) * strlen(src) + 1);
	for (i=0, j=1, k=0; src[i]; i++)
	{
		j = (!table[j]) ? 1 : j;	// always check each letter of the KEY

		if (flag == ENCODE)
			res[k] = (src[i] >= 'A' && src[i] <= 'Z')
				? table[j++][get_pos(table[0], src[i])]
				: src[i];
		else
			res[k] = (src[i] >= 'A' && src[i] <= 'Z')
				? table[0][get_pos(table[j++], src[i])]
				: src[i];
		k++;
	}
	res[k] = 0;
	return (res);
}

void		init_vig_table(char *key)
{
	int		i, j, k;
	char	*tmp;

	TABLE[0] = strdup(ALPHABET);
	for (j=0, k = 1; key[j]; j++)
	{
		for (i=1; i < (int)strlen(ALPHABET); i++)
		{
			tmp = rot_n(ALPHABET, i);		// rotation algorithm
			// to generate a Vigenere table
			if (tmp[0] == key[j])
				TABLE[k++] = strdup(tmp);
		}
	}
	TABLE[k] = NULL;
}

int			main(int ac, char *av[])
{
	int		i, x;
	char	*res, **key, *str;

	if (check_args(ac, av))
		return (-1);
	flag = (av[1][1] - 'c') * 32;	// the result of this operation
	// will be 32 (DECODE), or 64 (ENCODE)

	str = strdup(str_toupper(av[3]));	// get a CAPS version of the tested string

	key = get_key(av);	// get a clan version of the KEY

	for (x = 0; key[x]; x++)
	{
		TABLE = (char **)malloc(sizeof(char *) * 1024);
		printf("\033[94m%s\033[0m: ", key[x]);
		init_vig_table(key[x]);	// create a Vigenere table
		// (based on custom alphabet)

		res = VIGENERE(str, TABLE);
		print_res(res);

		for (i=0; TABLE[i]; i++)	// get rid of memory leaks
			free(TABLE[i]);		// because -SWAG-
		free(TABLE);			// deal with it
	}
	for (i=0; key[i]; i++)	// still getting rid of memory leaks
		free(key[i]);		// still because -SWAG-
	free(key);			// then goddam deal with it

	return (0);
}
