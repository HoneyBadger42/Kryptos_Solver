#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int			count_char(char *s, char c)
{
	int		i, j;

	for (i=0, j=0; s[i]; i++)
		if (s[i] == c)
			j++;
	j++;
	return (j);
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

	for (i=0; res[i]; i++)
		if (res[i] >= 'a' && res[i] <= 'z')
			res[i] -= 32;

	close(fd);
	return (res);
}

int			main(int ac, char *av[])
{
	char	*file;

	if (ac != 2)
		return (-1);
	file = filetoupper(av[1]);
	printf("%d\n", count_char(file, '\n'));
	free(file);
	return (0);
}
