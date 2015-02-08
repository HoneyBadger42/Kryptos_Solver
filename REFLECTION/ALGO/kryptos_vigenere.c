/*
 **
 ** Vigenere algo,
 ** optimized for the KRYPTOS enigma
 ** Fast and leaks-free program.
 **
 ** You can test it with strings, or files containing keys
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

int			flag, analysis_flag = 0;
const char	*ALPHABET = "KRYPTOSABCDEFGHIJLMNQUVWXZ";

int			usage(void)
{
	printf("usage: \033[92m./kryptos_vigenere\033[0m");
	printf(" <\033[95m-e\033[0m|\033[95m-d\033[0m> ");
	printf("<\033[94mkey\033[0m|\033[94mwordlist\033[0m> ");
	printf("<\033[96mstring\033[0m|\033[96mfile\033[0m> [\033[93m--syn\033[0m]\n");
	printf("\n\n\033[33mExamples\033[0m:\n");
	printf("\033[96m> \033[92m./kryptos_vigenere\033[0m -e \"maclef\" \"coucou\"\n");
	printf("\033[96m> \033[94mMACLEF\033[0m: RFTKJA\n");
	printf("\033[96m> \033[92m./kryptos_vigenere\033[0m -e - \"coucou\"\n");
	printf("\033[96m> \033[94mKRYPTOSABCDEFGHIJLMNQUVWXZ\033[0m: DAXGDR\n");
	printf("\n----------------\n\n");
	printf("\033[95m> \033[92m./kryptos_vigenere\033[0m -d \"maclef\" \"RFTKJA\"\n");
	printf("\033[95m> \033[94mMACLEF\033[0m: COUCOU\n");
	printf("\033[95m> \033[92m./kryptos_vigenere\033[0m -d - \"DAXGDR\"\n");
	printf("\033[95m> \033[94mKRYPTOSABCDEFGHIJLMNQUVWXZ\033[0m: COUCOU\n");
	printf("\n----------------\n\n");
	printf("Use \033[93m--syn\033[0m to launch a ");
	printf("dictionnary attack, with syntax analysis.\n\n");
	printf("This attack might take a few seconds, up to a few hours...\n");
	printf("(It depends of your processor, and the DICT you choose)\n");
	printf("(-> '\033[94mREFLECTION/DICT_english\033[0m' ");
	printf("is good enough to solve Part1 & 2, under 10min)\n");
	return (-1);
}

int			check_args(int ac, char *av[])
{
	if (ac != 4)
	{
		if (ac != 5)
			return (usage());
		else
		{
			if (!strcmp("--syn", av[4]))
				analysis_flag = 1;
			else
				return (usage());
		}
	}
	else if (!(!strcmp(av[1], "-e") || !strcmp(av[1], "-d")))
		return (usage());
	else if (!av[3][0])
		return (usage());
	return (0);
}

char		*str_toupper(char *str)
{
	int		i;

	for (i=0; str[i]; i++)
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
	return (str);
}

int			count_char(char *s, char c)
{
	int		i, j;

	for (i=0, j=0; s[i]; i++)
		if (s[i] == c)
			j++;
	j++;
	return (j);
}

char		**split(char *src, char split)
{
	int		i, j, k;
	int		size = count_char(src, split);
	char	**res = (char **)malloc(sizeof(char *) * size);

	for (i=0; i < size; i++)
	{
		res[i] = (char *)malloc(sizeof(char *) * 1024);
		for (j=0; j < 1024; j++)
			res[i][j] = 0;
	}
	for (i=0, k=0; src[i]; i++)
	{
		if (src[i] != split)
			res[k][j++] = src[i];
		else
		{
			j = 0;
			k++;
		}
	}
	res[k] = NULL;

	return (res);
}

char		*filetoupper(char *file)
{
	int		fd, i = 0;
	char	*res;
	char	buf[128] = {0};

	if ((fd = open(file, O_RDONLY)) < 0)
		return (NULL);

	while (read(fd, buf, 128) > 0)
		i++;
	i = (i * 128) + 128;
	close(fd);

	res = (char *)malloc(sizeof(char) * i);
	fd = open(file, O_RDONLY);
	read(fd, res, i);

	str_toupper(res);

	close(fd);
	return (res);
}

char		**file_to_needles(char *src_file)
{
	char	*file;
	char	**needles;

	file = filetoupper(src_file);
	needles = split(file, '\n');
	free(file);
	return (needles);
}

int			valid_words_num(char *haystack, char **needles)
{
	int		i, res;

	for (i=0, res=0; needles[i]; i++)
		if (strstr(haystack, needles[i]))
			res++;
	return (res);
}

char		**get_key(char *av[])
{
	int		i, fd;
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
		close(fd);
		printf("\033[93mPreparing bruteforce...\033[0m\n");
		key = file_to_needles(av[2]);
		for (i=0; key[i]; i++);
		printf("\033[92mDone\033[0m -> \033[96m%d\033[0m words to test.\n\n", i);
	}
	return (key);
}

char		*rot_n(const char *str, int n)
{
	int		i, j;
	char	*res = strdup(str);

	for (i=0; i < n; i++)
	{
		for (j=0; j < (int)(strlen(str) - 1); j++)
		{
			res[j] ^= res[j+1];
			res[j+1] ^= res[j];
			res[j] ^= res[j+1];
		}
	}
	return (res);
}

int			get_pos(char *s, char c)
{
	int		i;

	for (i = 0; s[i]; i++)
		if (s[i] == c)
			return (i);
	return (0);
}

char		*VIGENERE(char *src, char **table, char *key)
{
	int		i, j, k, Tpos, key_len = strlen(key);
	char	*res, *tmp = (char *)malloc(sizeof(char) * strlen(src));

	for (i=0, j=0, k=0; src[i]; i++)
	{
		j %= key_len;						// always check each letter of the KEY
		Tpos = get_pos(table[0], key[j]);	//find its pos on the reference ALPHABET

		if (flag == ENCODE)
		{
			if (src[i] >= 'A' && src[i] <= 'Z')
			{
				tmp[k] = table[Tpos][get_pos(table[0], src[i])];
				j++;
			}
			else
				tmp[k] = src[i];
		}
		else
		{
			if (src[i] >= 'A' && src[i] <= 'Z')
			{
				tmp[k] = table[0][get_pos(table[Tpos], src[i])];
				j++;
			}
			else
				tmp[k] = src[i];
		}
		k++;
	}
	tmp[k] = 0;
	res = strdup(tmp);
	free(tmp);
	return (res);
}

char		**init_vig_table(void)
{
	int		i;
	char	**table = (char **)malloc(sizeof(char *) * strlen(ALPHABET) + 1);

	table[0] = strdup(ALPHABET);
	for (i=1; i < (int)strlen(ALPHABET); i++)
		table[i] = rot_n(ALPHABET, i);	// rotation algorithm
										// to generate a Vigenere table
	table[i] = NULL;
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

void		putcustomstr(char *res, char *key)
{
	int		j;

	printf("\033[94m%s\033[0m: ", key);
	for (j=0; res[j]; j++)
		if (res[j] != ' ')
			printf("%c", res[j]);
	printf("\n");
}

void		display_res(char *str, char **key)
{
	static int	check = 0;
	int			i, j, qty;
	char		*res, **table;
	char		tmp_buf[1024] = {0};

	table = init_vig_table();	// create a Vigenere table
								// (based on custom alphabet)

	if (analysis_flag)
		printf("\033[95mDictionnary attack, with syntax analysis...\nBest candidates\033[0m:\n\n");

	for (i = 0; key[i]; i++)
	{
		if (strlen(key[i]))
		{
			res = VIGENERE(str, table, key[i]);

			if (!analysis_flag)
				putcustomstr(res, key[i]);
			else
			{
				qty = valid_words_num(res, key);
				if (qty > check)
				{
					check = qty;
					for (j=0; res[j]; j++)
						tmp_buf[j] = res[j];
					putcustomstr(tmp_buf, key[i]);
				}
			}
			free(res);
		}
	}

	for (i=0; table[i]; i++)	// get rid of memory leaks
		free(table[i]);			// because -SWAG-
	free(table);				// deal with it
}

int			main(int ac, char *av[])
{
	int		i;
	char	*str, **key;

	if (check_args(ac, av))
		return (-1);
	flag = (av[1][1] - 'c') * 32;	// the result of this operation
									// will be 32 (DECODE), or 64 (ENCODE)

	str = get_str(av[3]);
	key = get_key(av);	// get a clean version of the KEY (in caps)


    display_res(str, key);


	for (i=0; key[i]; i++)		// get rid of memory leaks
		free(key[i]);			// because -SWAG-
	free(key);					// deal with it
	free(str);
	return (0);
}
