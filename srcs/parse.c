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
static int compare_nodes(t_syst *left, t_syst *right, t_flags flags)
{
	int result = 0;
	if (flags.t && left->info.st_mtime != right->info.st_mtime)
	{
		if (left->info.st_mtime > right->info.st_mtime)
			return (-1);
		return (1);
	}
	else
		result = ft_strncmp(left->name, right->name, ft_strlen(left->name));
	if (flags.r)
		result = -result;
	return (result);
}

static void add_node(t_syst **list, t_syst *node, t_flags flags) {
	t_syst *last;

	if (*list == NULL || compare_nodes(node, *list, flags) < 0) {
		node->next = *list;
		*list = node;
		return;
	}
	last = *list;
	while(last->next != NULL && compare_nodes(node, last->next, flags) >= 0) {
		last = last->next;
	}
	node->next = last->next;
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
			add_node(dirs, node, flags);
		else
			add_node(files, node, flags);
	}
	closedir(pdir);
}
