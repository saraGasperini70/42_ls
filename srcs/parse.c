#include "../includes/ft_ls.h"

void parse_flags(t_ls *ls, char **av, int ac, int i) {
	ls->start_path = ".";
	if (ac > 1) {
		// Parse command line arguments and set flags accordingly
		for (int i = 1; i < ac; i++) {
			if (av[i][0] == '-') {
				// Handle flags
				for (int j = 1; av[i][j] != '\0'; j++) {
					switch (av[i][j]) {
						case 'a': // all
							ls->flags.a = true;
							break;
						case 'l': // long format
							ls->flags.l = true;
							break;
						case 'r': // reverse order
							ls->flags.r = true;
							break;
						case 'R': // Recursive
							ls->flags.R = true;
							break;
						case 't': // sort by time
							ls->flags.t = true;
							break;
						default:
							ft_printf("Error: invalid option: -%c\n", av[i][j]);
							return ;
					}
				}
			} else {
				ls->start_path = av[i];

				ls->pdir = opendir(av[i]);
				if (ls->pdir == NULL) {
					ft_error(ft_strjoin("Cannot access '", ft_strjoin(ft_strdup(av[i]), "'")));
					return ;
				}
			}
		}
	}
}

static void add_node(t_syst **list, t_syst *node) {
	t_syst *last;

	if (*list == NULL) {
		*list = node;
		return;
	}
	last = *list;
	while(last->next != NULL) {
		last = last->next;
	}
	last->next = node;
}

void parse_contents(const char *path, t_syst **dirs, t_syst **files, t_flags flags) {

	DIR 			*pdir;
	struct dirent	*entry;
	char 			*dir_path;
	t_syst 			*node;

	*dirs = NULL;
	*files = NULL;
	pdir = opendir(path);
	if (pdir == NULL)
		return;

	while ((entry = readdir(pdir)) != NULL) {

		if (!flags.a && entry->d_name[0] == '.')
			continue;

		if (entry->d_name[0] == '.' && (entry->d_name[1] == '\0'
				|| (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
			continue;

		node = ft_calloc(1, sizeof(t_syst));
		if (node == NULL)
			break;
		dir_path = ft_strjoin(ft_strdup(path), ft_strjoin(ft_strdup("/"), ft_strdup(entry->d_name)));

		if (dir_path == NULL) {
			free(node);
			break;
		}

		node->name = ft_strdup(entry->d_name);
		node->path = dir_path;

		if (stat(node->path, &node->info) == -1) {
			free(node->name);
			free(node->path);
			free(node);
			continue;
		}

		if (S_ISDIR(node->info.st_mode))
			add_node(dirs, node);
		else
			add_node(files, node);
	}
	closedir(pdir);
}
