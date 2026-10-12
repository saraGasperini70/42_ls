#include "../includes/ft_ls.h"

int main(int ac, char **av) {
	t_ls *ls;
	ls = ft_calloc(1, sizeof(t_ls));
	int result = 0;
	if (ls == NULL) {
		ft_printf("Error: Memory allocation failed\n");
		return (1);
	}
	result = parse_flags(ls, av, ac, 1);
	if (result != 0) {
		return (1);
	}
	result = ft_ls(ls);
	if (result != 0) {
		return (1);
	}
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
