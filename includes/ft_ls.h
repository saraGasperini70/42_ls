#ifndef FT_LS_H
#define FT_LS_H

#include "../libs/42_libft/libft.h"
#include "../libs/42_ft_printf/ft_printf.h"
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct s_map { // trying to reimplement C++ std::map if needed
	struct s_map 	*prev;
	char 			*key;
	void 			*value;
	size_t 			len;
	struct s_map 	*next;
} t_map;

typedef struct s_flags {
	bool a; // all
	bool l; // long format
	bool r; // reverse order
	bool R; // recursive
	bool t; // sort by time
} t_flags;

typedef struct s_syst { // Stores file/directory information
	struct stat 	info;
	char 			*name;
	char 			*path;
	struct s_syst 	*content;
	struct s_syst 	*next;
} t_syst;

typedef struct s_ls {
	t_flags 		flags;
	char 			*start_path;
	DIR 			*pdir;
	struct dirent 	*entry;
	t_syst 			*dirs;
	t_syst 			*files;

} t_ls;

int	 ft_error(char *str);
int	 ft_ls(t_ls *ls);
void parse_contents(const char *path, t_syst **dirs, t_syst **files, t_flags flags);
void parse_flags(t_ls *ls, char **av, int ac, int i);


#endif
