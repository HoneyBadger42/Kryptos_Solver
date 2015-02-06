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
const char	*ALPHABET = "KRYPTOSABCDEFGHIJLMNQUVWXZ";

int			usage(void)
{
	printf("usage: \033[92m./kryptos_vigenere\033[0m");
	printf(" <\033[95m-e\033[0m|\033[95m-d\033[0m> ");
	printf("<\033[94mkey\033[0m|\033[94mwordlist\033[0m> ");
	printf("<\033[96mstring\033[0m|\033[96mfile\033[0m>\n");
	printf("\n\033[93mExamples\033[0m:\n");
	printf("\033[96m> \033[92m./kryptos_vigenere\033[0m -e \"maclef\" \"coucou\"\n");
	printf("\033[96m> \033[94mMACLEF\033[0m: RFTKJA\n");
	printf("\033[96m> \033[92m./kryptos_vigenere\033[0m -e - \"coucou\"\n");
	printf("\033[96m> \033[94mKRYPTOSABCDEFGHIJLMNQUVWXZ\033[0m: DAXGDR\n");
	printf("\n----------------\n\n");
	printf("\033[95m> \033[92m./kryptos_vigenere\033[0m -d \"maclef\" \"RFTKJA\"\n");
	printf("\033[95m> \033[94mMACLEF\033[0m: COUCOU\n");
	printf("\033[95m> \033[92m./kryptos_vigenere\033[0m -d - \"DAXGDR\"\n");
	printf("\033[95m> \033[94mKRYPTOSABCDEFGHIJLMNQUVWXZ\033[0m: COUCOU\n");
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
	char	**key = NULL;

	if ((fd = open(av[2], O_RDONLY)) < 0)
	{
		key = (char **)malloc(sizeof(char *) * 2);
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

		printf("\033[93mPreparing bruteforce...\033[0m\n");
		key = (char **)malloc(sizeof(char *) * (1 << 22));
		while (read(fd, &c, 1) > 0)
		{
			if (c == '\n')
			{
				while (i < 63)
					tmp[i++] = 0;
				key[j++] = strdup(str_toupper(tmp));
				i = 0;
			}
			else
				tmp[i++] = c;
		}
		key[j] = NULL;
		free(tmp);
		close(fd);
		printf("\033[92mDone\033[0m -> \033[96m%d\033[0m words to test.\n\n", j);
	}
	return (key);
}

char		*rot_n(const char *str, int n)
{
	int		i, j;
	char	*res, *s = strdup(str);

	res = s;
	for (i=0; i < n; i++)
		for (j=0; j < (int)strlen(s)-1; j++)
		{
			res[j] ^= res[j+1];
			res[j+1] ^= res[j];
			res[j] ^= res[j+1];
		}
	free(s);
	return (res);
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
	char	*res, *ret;

	if (!(ret = (char *)malloc(sizeof(char) * 1024)))
		return (NULL);
	res = ret;
	for (i=0, j=1, k=0; src[i]; i++)
	{
		j = (!table[j]) ? 1 : j;	// always check each letter of the KEY

		if (flag == ENCODE)
		{
			res[k] = (src[i] >= 'A' && src[i] <= 'Z')
				? table[j++][get_pos(table[0], src[i])]
				: src[i];
		}
		else
		{
			res[k] = (src[i] >= 'A' && src[i] <= 'Z')
				? table[0][get_pos(table[j++], src[i])]
				: src[i];
		}
		k++;
	}
	res[k] = 0;
	for (i=0; table[i]; i++)	// get rid of memory leaks
		free(table[i]);			// because -SWAG-
	free(table);				// deal with it
	free(ret);
	return (res);
}

char		**init_vig_table(char *key)
{
	int		i, j, k;
	char	*tmp;
	char	**table = (char **)malloc(sizeof(char *) * 32);

	printf("\033[94m%s\033[0m: ", key);
	table[0] = strdup(ALPHABET);
	for (j=0, k = 1; key[j]; j++)
	{
		for (i=1; i < (int)strlen(ALPHABET); i++)
		{
			tmp = rot_n(ALPHABET, i);		// rotation algorithm
											// to generate a Vigenere table
			if (tmp[0] == key[j])
				table[k++] = strdup(tmp);
		}
	}
	table[k] = NULL;
	return (table);
}

char		*get_str(char *arg)
{
	int		fd;
	char	*str;

	if ((fd = open(arg, O_RDONLY)) < 0)
		str = strdup(str_toupper(arg));	// get a CAPS version of the tested string
	else
	{
		int		i = 0;
		char	c;
		char	*tmp = (char *)malloc(sizeof(char) * 1024);

		while (read(fd, &c, 1) > 0)
		{
			if (c != '\n')
				tmp[i++] = c;
			else
				tmp[i] = 0;
		}
		while (i < 1024)
			tmp[i++] = 0;
		str = strdup(str_toupper(tmp));
		free(tmp);
		close(fd);
	}
	return (str);
}

int			main(int ac, char *av[])
{
	int		x;
	char	*res, *str;
	char	**TABLE, **key;

	if (check_args(ac, av))
		return (-1);
	flag = (av[1][1] - 'c') * 32;	// the result of this operation
									// will be 32 (DECODE), or 64 (ENCODE)

	str = get_str(av[3]);

	key = get_key(av);	// get a clan version of the KEY

	for (x = 0; key[x]; x++)
	{
		TABLE = init_vig_table(key[x]);		// create a Vigenere table
											// (based on custom alphabet)
		free(key[x]);

		res = VIGENERE(str, TABLE);
		print_res(res);
	}
	free(str);

	return (0);
}
