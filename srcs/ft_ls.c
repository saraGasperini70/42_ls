#include "../includes/ft_ls.h"

int ft_error(char *str) {
	ft_printf("ft_ls: %s: %s\n", str, strerror(errno));
	return (1);
}

static int name_cmp(const char *left, const char *right)
{
	while (*left && *left == *right)
	{
		left++;
		right++;
	}
	return ((unsigned char)*left - (unsigned char)*right);
}

void print_colored_entry(t_syst *node, char *type) {
	(void)type;
	if (node == NULL) {
		return;
	}
	ft_printf("%s", node->name);
}

static void print_entries(t_syst *dirs, t_syst *files)
{
	while (dirs != NULL || files != NULL)
	{
		if (files == NULL
			|| (dirs != NULL && name_cmp(dirs->name, files->name) < 0))
		{
			print_colored_entry(dirs, "dir");
			dirs = dirs->next;
		}
		else
		{
			print_colored_entry(files, "file");
			files = files->next;
		}
		if (dirs != NULL || files != NULL)
			ft_printf(" ");
	}
}

int ft_ls(t_ls *ls) {
	// Implementation of the ft_ls function
	// This function will handle the logic for listing files and directories
	parse_contents(ls->start_path, &ls->dirs, &ls->files, ls->flags);
	t_syst *current;
	if (ls->dirs == NULL && ls->files == NULL) {
		return (0);
	}
	print_entries(ls->dirs, ls->files);
	ft_printf("\n");
	current = ls->dirs;
	while (current) {
		if (ls->flags.R)
		{
			ft_printf("%s:\n", current->path);
			t_ls sub_ls;
			sub_ls.flags = ls->flags;
			parse_contents(current->path, &sub_ls.dirs, &sub_ls.files,
				ls->flags);
			print_entries(sub_ls.dirs, sub_ls.files);
			ft_printf("\n");
		}
		current = current->next;
	}
	return 0;
}
