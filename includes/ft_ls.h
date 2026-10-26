#ifndef FT_LS_H
#define FT_LS_H

#include "../libs/42_libft/libft.h"
#include "../libs/42_ft_printf/ft_printf.h"
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

typedef enum e_color {
	ALWAYS,
	NEVER,
	AUTO
} t_color;

typedef struct s_map { // trying to reimplement C++ std::map if needed
	struct s_map 	*prev;
	char 			*key;
	void 			*value;
	size_t 			len;
	struct s_map 	*next;
} t_map;

typedef struct s_flags {
	bool a; 	// all DONE
	bool l; 	// long format DONE
	bool r; 	// reverse order DONE
	bool R; 	// recursive DONE
	bool t; 	// sort by time DONE
	bool l1; 	// list one per line DONE
	bool p; 	// = append / to directories DONE
	t_color	color; 	// color output TODO
	bool h; 	// human readable sizes TODO
} t_flags;

typedef enum e_type {
	FILE_TYPE,
	DIR_TYPE
} t_type;

typedef struct s_syst { // Stores file/directory information
	struct stat 	info;
	t_type 			type;
	char 			*name;
	char 			*path;
	struct s_syst 	*next;
} t_syst;

typedef struct s_ls {
	t_flags 		flags;
	char 			*start_path;
	DIR 			*pdir;
	struct dirent 	*entry;
	t_syst 			*files;

} t_ls;

int	 ft_error(char *str);
int	 ft_ls(t_ls *ls);
void parse_contents(const char *path, t_syst **files, t_flags flags);
int parse_flags(t_ls *ls, char **av, int ac, int i);
void free_syst_list(t_syst *list);

#endif
