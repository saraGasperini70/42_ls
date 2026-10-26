#include "../includes/ft_ls.h"

int parse_flags(t_ls *ls, char **av, int ac, int i) {
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
						case '1': // list one per line
							ls->flags.l1 = true;
							break;
						case 'p': // append / to directories
							ls->flags.p = true;
							break;
						case 'h': // human readable sizes
							ls->flags.h = true;
							break;
						case '-':
							// Handle --color option
							if (ft_strncmp(av[i], "--color", 7) == 0) {
								ls->flags.color = AUTO;
							} else if (ft_strncmp(av[i], "--color=always", 13) == 0) {
								ls->flags.color = ALWAYS;
							} else if (ft_strncmp(av[i], "--color=never", 13) == 0) {
								ls->flags.color = NEVER;
							} else {
								ft_printf("Error: invalid option: %s\n", av[i]);
								return (1);
							}
							break;
						default:
							ft_printf("Error: invalid option: -%c\n", av[i][j]);
							return (1);
					}
				}
			} else {
				ls->start_path = av[i];
				if (access(av[i], F_OK) == -1) {
					char *message;
					char *full_message;

					message = ft_strjoin("Cannot access '", av[i]);
					full_message = message != NULL
						? ft_strjoin(message, "'") : NULL;
					free(message);
					if (full_message != NULL) {
						ft_error(full_message);
						free(full_message);
					} else {
						ft_error("Cannot access path");
					}
					return (1);
				}
			}
		}
	}
	return (0);
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


void parse_contents(const char *path, t_syst **files, t_flags flags) {

	DIR 			*pdir;
	struct dirent	*entry;
	t_syst 			*node;

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
		char *path_copy = ft_strjoin("/", entry->d_name);
		if (path_copy == NULL) {
			free(node);
			break;
		}
		char *dir_path = ft_strjoin(path, path_copy);

		if (dir_path == NULL) {
			free(node);
			free(path_copy);
			break;
		}

		node->name = ft_strdup(entry->d_name);
		node->path = dir_path;
		node->type = (entry->d_type == DT_DIR) ? DIR_TYPE : FILE_TYPE;
		free(path_copy);
		if (node->name == NULL) {
			free(node->path);
			free(node);
			break;
		}

		if (stat(node->path, &node->info) == -1) {
			free(node->name);
			free(node->path);
			free(node);
			continue;
		}

		add_node(files, node, flags);
	}
	closedir(pdir);
}
