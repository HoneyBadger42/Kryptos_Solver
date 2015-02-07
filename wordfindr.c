#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

const char *haystack = "ITWASTOTALLYINVISIBLEHOWSTHATPOSSIBLE?THEYUSEDTHEEARTHSMAGNETICFIELDXTHEINFORMATIONWASGATHEREDANDTRANSMITTEDUNDERGRUUNDTOANUNKNOWNLOCATIONXDOESLANGLEYKNOWABOUTTHIS?THEYSHOULDITSBURIEDOUTTHERESOMEWHEREXWHOKNOWSTHEEXACTLOCATION?ONLYWWTHISWASHISLASTMESSAGEXTHIRTYEIGHTDEGREESFIFTYSEVENMINUTESSIXPOINTFIVESECONDSNORTHSEVENTYSEVENDEGREESEIGHTMINUTESFORTYFOURSECONDSWESTIDBYROWS";

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

	for (i=0; res[i]; i++)
		if (res[i] >= 'a' && res[i] <= 'z')
			res[i] -= 32;

	close(fd);
	return (res);
}

char		**file_to_needles(char *src_file)
{
	int		i, res;
	char	*file;
	char	**needles;

	file = filetoupper(src_file);
	needles = split(file, '\n');
	free(file);
	for (i=0, res=0; needles[i]; i++)
		if (strstr(haystack, needles[i]))
			res++;
	return (needles);
}

int			main(int ac, char *av[])
{
	char **need;
	if (ac != 2)
		return (-1);
	need = file_to_needles(av[1]);
	int i;
	for (i=0; need[i]; i++)
		printf("%s\n", need[i]);
	return (0);
}
