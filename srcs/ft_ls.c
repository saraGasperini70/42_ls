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

char *get_file_permissions(mode_t mode) {
	static char permissions[11];

	permissions[0] = S_ISDIR(mode) ? 'd' : '-';
	permissions[1] = (mode & S_IRUSR) ? 'r' : '-';
	permissions[2] = (mode & S_IWUSR) ? 'w' : '-';
	permissions[3] = (mode & S_IXUSR) ? 'x' : '-';
	permissions[4] = (mode & S_IRGRP) ? 'r' : '-';
	permissions[5] = (mode & S_IWGRP) ? 'w' : '-';
	permissions[6] = (mode & S_IXGRP) ? 'x' : '-';
	permissions[7] = (mode & S_IROTH) ? 'r' : '-';
	permissions[8] = (mode & S_IWOTH) ? 'w' : '-';
	permissions[9] = (mode & S_IXOTH) ? 'x' : '-';
	permissions[10] = '\0';

	return permissions;
}

void print_long_entry(t_syst *current) {
	struct passwd	*user;
	struct group	*group;
	char			*time, *modes;

	if (current == NULL)
		return;
	user = getpwuid(current->info.st_uid);
	group = getgrgid(current->info.st_gid);
	modes = get_file_permissions(current->info.st_mode);
	char time_buf[13];
	strftime(time_buf, sizeof(time_buf),
    	"%b %e %H:%M", localtime(&current->info.st_mtime));
	ft_printf("%s %d %s %s %d %s %s\n",
		modes,
		(int)current->info.st_nlink,
		user != NULL ? user->pw_name : "?",
		group != NULL ? group->gr_name : "?",
		(int)current->info.st_size,
		time_buf,
		current->name);
}

void print_entry(t_syst *node)
{
	if (node != NULL)
		ft_printf("%s", node->name);
}

static void print_entries(t_syst *dirs, t_syst *files)
{
	while (dirs != NULL || files != NULL)
	{
		if (files == NULL
			|| (dirs != NULL && name_cmp(dirs->name, files->name) < 0))
		{
			print_entry(dirs);
			dirs = dirs->next;
		}
		else
		{
			print_entry(files);
			files = files->next;
		}
		if (dirs != NULL || files != NULL)
			ft_printf(" ");
	}
}

long block_len(t_syst *dirs, t_syst *files)
{
	long total = 0;
	while (dirs != NULL || files != NULL)
	{
		if (files == NULL
			|| (dirs != NULL && name_cmp(dirs->name, files->name) < 0))
		{
			total += dirs->info.st_blocks;
			dirs = dirs->next;
		}
		else
		{
			total += files->info.st_blocks;
			files = files->next;
		}
	}
	return total;
}

static void print_long_entries(t_syst *dirs, t_syst *files)
{
	long total = block_len(dirs, files);
	ft_printf("total %d\n", (int)(total / 2));
	while(dirs != NULL || files != NULL)
	{
		if (files == NULL
			|| (dirs != NULL && name_cmp(dirs->name, files->name) < 0))
		{
			print_long_entry(dirs);
			dirs = dirs->next;
		}
		else
		{
			print_long_entry(files);
			files = files->next;
		}
	}
}


static void list_recursive(const char *path, t_flags flags)
{
    t_syst *dirs;
    t_syst *files;
    t_syst *current;

    parse_contents(path, &dirs, &files, flags);

    if (flags.l)
        print_long_entries(dirs, files);
    else
        print_entries(dirs, files);

    current = dirs;
    while (current != NULL)
    {
        ft_printf("\n%s:\n", current->path);
        list_recursive(current->path, flags);
        current = current->next;
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
	if (ls->flags.R)
		list_recursive(ls->start_path, ls->flags);
	else if (ls->flags.l)
		print_long_entries(ls->dirs, ls->files);
	else
		print_entries(ls->dirs, ls->files);
	return 0;
}
