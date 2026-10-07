#include "../includes/ft_ls.h"

int main(int ac, char **av) {
	t_ls *ls;
	ls = ft_calloc(1, sizeof(t_ls));
	parse_flags(ls, av, ac, 1);
	ft_ls(ls);
	free(ls->dirs);
	free(ls->files);
	free(ls);
	return (0);
}

/*
-a, --all
			  do not ignore entries starting with .
-l	 use a long listing format
-r, --reverse
			  reverse order while sorting

		-R, --recursive
			  list subdirectories recursively
			  -t	 sort by time, newest first; see --time
*/
