#include "minishell.h"

int	open_infile(char *infile)
{
	int	fd;

	fd = open(infile, O_RDONLY);
	return (fd);
}

int	get_outfile_flags(int append)
{
	if (append)
	{
		return (O_WRONLY | O_CREAT | O_APPEND);
	}
	return (O_WRONLY | O_CREAT | O_TRUNC);
}

int	open_outfile(char *outfile, int append)
{
	int	fd;
	int	flags;

	flags = get_outfile_flags(append);
	fd = open(outfile, flags, 0644);
	if (fd == -1)
	{
		perror(outfile);
		return (-1);
	}
	return (fd);
}
