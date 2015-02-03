#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int			**CHARS, SIZE = 128;

void		print_char_analysis(int **CHARS, float total_len)
{
	int		c, i, n;
	float	ratio;

	for (i=0; i < SIZE; i++)
	{
		if ((n = CHARS[i][0]) > 0)
		{
			c = CHARS[i][1];
			ratio = (float)(n * 100 / total_len);

			if (c < 32 || c == 127)
				printf("'0x%02x' =>  %.02f%c\n", c, ratio, '%');
			else
				printf("'%c'    =>  %.02f%c\n", c, ratio, '%');
		}
	}
}

void		sort_analysis_table(int **table, int size)
{
	int		i, j, k;

	for (i=0; i < size; i++)
	{
		for (j=0; j < size - 1; j++)
		{
			if (table[j][0] < table[j+1][0])
			{
				for (k=0; k < 2; k++)
				{
					table[j][k] ^= table[j+1][k];
					table[j+1][k] ^= table[j][k];
					table[j][k] ^= table[j+1][k];
				}
			}
		}
	}
}

double		fill_table(int **table, char *av[])
{
	int		ci, i, fd;
	char	cc;
	double	total_len;

	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		for (i=0, total_len=0.00; av[1][i]; i++, total_len += 1)
		{
			ci = av[1][i];
			if (ci >= 0 && ci <= 127)
				table[ci][0]++;
		}
	}
	else
	{
		for (total_len=0.00; read(fd, &cc, 1) > 0; total_len += 1)
		{
			ci = (int)cc;
			if (ci >= 0 && ci <= 127)
				table[ci][0]++;
		}
		close(fd);
	}
	return (total_len);
}

int			**new_char_tab(int **table)
{
	int		i;

	table = (int **)malloc(sizeof(int *) * SIZE);
	for (i=0; i < (int)SIZE; i++)
	{
		table[i] = (int *)malloc(sizeof(int) * 2);
		table[i][0] = 0;
		table[i][1] = i;
	}
	table[i] = NULL;
	return (table);
}

int			main(int ac, char *av[])
{
	if (ac != 2)
		return (printf("usage: %s <string | file>\n", av[0]));
	else if (!av[1][0])
		return (printf("usage: %s <string | file>\n", av[0]));

	int		i;
	double	total_len;

	CHARS = new_char_tab(CHARS);

	total_len = fill_table(CHARS, av);

	sort_analysis_table(CHARS, SIZE);

	print_char_analysis(CHARS, total_len);

	for (i=0; i < SIZE; i++)
		free(CHARS[i]);
	free(CHARS);
	return (0);
}
