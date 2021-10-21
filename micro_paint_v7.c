#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

char	*draw;
char	type, foreg, backg;
float	sqx, sqy, h, w;
int		width, height;

int checkhit(float x, float y)
{
	if (x < sqx || x > sqx + w || y < sqy || y > sqy + h)
		return (0);
	if (x - sqx < 1 || sqx + w - x < 1 || y - sqy < 1 || sqy + h - y < 1)
		return (2);
	return (1);
}

void drawing(void)
{
	int x = -1, y, hit;
	
	while (++x < width)
	{
	y = -1;
		while (++y < height)
		{
			hit = checkhit((float)x, (float)y);
			if (hit == 2 || (hit == 1 && type == 'R'))
				draw[x + y * width] = foreg;
		}
	}
}

void output(void)
{
	int	y = -1;

	while (++y < height)
	{
		write (1, draw + y * width, width);
		write (1, "\n", 1);
	}
	free(draw);
}

int parser(FILE *file)
{
	int	x = 0, result;

	if (fscanf(file, "%d %d %c\n", &width, &height, &backg) != 3
		|| width <= 0 || width > 300 || height <= 0 || height > 300
		|| !(draw = malloc(width * height)))
		return (1);
	while (x < width * height)
		draw[x++] = backg;
	while ((result = fscanf(file, "%c %f %f %f %f %c\n", &type, &sqx, &sqy, &w, &h, &foreg)) == 6)
	{
		if ((type != 'r' && type != 'R') || h <= 0 || w <= 0)
			break ;
		drawing();
	}
	if (result != -1)
		return (1);
	output();
	return (0);
}

int	main(int argc, char **argv)
{
	FILE	*file;

	file = fopen(argv[1], "r");
	if (argc == 2 && file && parser(file) == 0)
		return (0);
	if (argc != 2)
		write (1, "Error: argument\n", 16);
	else
		write (1, "Error: Operation file corrupted\n", 32);
	return (1);
	fclose(file);
}