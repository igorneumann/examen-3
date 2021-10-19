#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int		width, height;
char	*draw;
char	type, foreg, backg;
float	sqx, sqy, w, h;

int check_hit(float x, float y)
{
	if (x < sqx || x > sqx + w || y < sqy || y > sqy + h)
		return (0);
	if (x - sqx < 1 || sqx + w - x < 1 || y - sqy < 1 || sqy + h - y < 1)
		return (2);
	return (1);
}

void output(void)
{
	int y = -1, c = 10;
	
	while (++y < height)
	{
		write(1, draw + y * width, width);
		write(1, &c, 1);
	}
	free(draw);
}

void drawing(void)
{
	int x = -1, y, hit;

	while(++x < width)
	{
		y = -1;
		while (++y < height)
		{
			hit = check_hit((float)x, (float)y);
			if (hit == 2 || (hit == 1 && type == 'R'))
				draw[x + y * width] = foreg;
		}
	}
}

int parser(FILE *file)
{
	int i = 0, result;

	if (fscanf(file, "%d %d %c\n", &width, &height, &backg) != 3)
		return (1);
	if (width < 1 || width > 300 || height < 1 || height > 300)
		return (1);
	if ((draw = malloc(width * height)) == NULL)
		return (1);
	while (i < width * height)
		draw[i++] = backg;
	while ((result = fscanf(file, "%c %f %f %f %f %c\n", &type, &sqx, &sqy, &w, &h, &foreg)) == 6)
	{
		if (w <= 0 || h <= 0 || (type != 'r' && type != 'R'))
			return (1);
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

	if (argc != 2)
	{
		write (1, "Error: argument\n", 16);
		return (1);
	}
	else if (!(file = fopen(argv[1], "r")) || parser(file))
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	fclose(file);
	return (0);
}