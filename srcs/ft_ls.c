#include "../includes/ft_ls.h"

int ft_error(char *str) {
	ft_printf("ft_ls: %s: %s\n", str, strerror(errno));
	return (1);
}

char *add_spaces(int count) {
	static char spaces[100];
	int i = 0;
	if (count < 0)
		count = 0;
	for (i = 0; i < count; i++) {
		spaces[i] = ' ';
	}
	spaces[i] = '\0';
	return spaces;
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

static int number_len(long number)
{
	int length = 1;

	if (number < 0)
	{
		length++;
		number = -number;
	}
	while (number >= 10)
	{
		number /= 10;
		length++;
	}
	return (length);
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

void print_long_entry(t_syst *current, int permission_w, int link_w, int user_w, int group_w, int size_w, int time_w) {
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
	ft_printf("%s%s",
		add_spaces(permission_w - ft_strlen(modes)),
		modes);

	ft_printf(" %s%d",
		add_spaces(link_w - number_len((long)(int)current->info.st_nlink)),
		(int)current->info.st_nlink);

	ft_printf(" %s%s",
		add_spaces(user_w - ft_strlen(user != NULL ? user->pw_name : "?")),
		user != NULL ? user->pw_name : "?");

	ft_printf(" %s%s",
		add_spaces(group_w - ft_strlen(group != NULL ? group->gr_name : "?")),
		group != NULL ? group->gr_name : "?");

	ft_printf(" %s%d",
		add_spaces(size_w - number_len((long)(int)current->info.st_size)),
		(int)current->info.st_size);

	ft_printf(" %s%s",
		add_spaces(time_w - ft_strlen(time_buf)),
		time_buf);
	ft_printf(" %s\n",
		current->name);
}

void print_entry(t_syst *node)
{
	if (node != NULL)
		ft_printf("%s", node->name);
}

static void print_entries(t_syst *files, t_flags flags)
{
	while (files != NULL)
	{
		print_entry(files);
		if (flags.p && S_ISDIR(files->info.st_mode))
			ft_printf("/");
		files = files->next;
		if (files != NULL) {
			ft_printf(" ");
			if (flags.l1)
				ft_printf("\n");
		}
	}
	ft_printf("\n");
}

long block_len(t_syst *files)
{
	long total = 0;
	while (files != NULL)
	{
		total += files->info.st_blocks;
		files = files->next;
	}
	return total;
}

static void print_long_entries(t_syst *files)
{
	long total = block_len(files);
	ft_printf("total %d\n", (int)(total / 2));
	int permission_w = 0, link_w = 0, user_w = 0, group_w = 0, size_w = 0, time_w = 0; // for each column, find the max width for proper alignment
	t_syst *temp = files;
	while (temp != NULL)
	{
		int len;
		len = ft_strlen(get_file_permissions(temp->info.st_mode));
		if (len > permission_w)
			permission_w = len;
		len = number_len((long)(int)temp->info.st_nlink);
		if (len > link_w)
			link_w = len;
		struct passwd *user = getpwuid(temp->info.st_uid);
		if (user != NULL) {
			len = ft_strlen(user->pw_name);
			if (len > user_w)
				user_w = len;
		}
		struct group *group = getgrgid(temp->info.st_gid);
		if (group != NULL) {
			len = ft_strlen(group->gr_name);
			if (len > group_w)
				group_w = len;
		}
		len = number_len((long)(int)temp->info.st_size);
		if (len > size_w)
			size_w = len;
		char time_buf[13];
		strftime(time_buf, sizeof(time_buf),
			"%b %e %H:%M", localtime(&temp->info.st_mtime));
		len = ft_strlen(time_buf);
		if (len > time_w)
			time_w = len;
		temp = temp->next;
	}
	while (files != NULL)
	{
		print_long_entry(files, permission_w, link_w, user_w, group_w, size_w, time_w);
		files = files->next;
	}
}

static void list_recursive(const char *path, t_flags flags, bool print_header)
{
	t_syst	*files;
	t_syst	*current;

	parse_contents(path, &files, flags);
	if (print_header)
		ft_printf("%s:\n", path);

	if (flags.l)
		print_long_entries(files);
	else
		print_entries(files, flags);

	current = files;
	while (current != NULL)
	{
		if (S_ISDIR(current->info.st_mode)
			&& ft_strncmp(current->name, ".", 2) != 0
			&& ft_strncmp(current->name, "..", 3) != 0)
		{
			ft_printf("\n");
			list_recursive(current->path, flags, true);
		}
		current = current->next;
	}
	free_syst_list(files);
}

int ft_ls(t_ls *ls) {
	// Implementation of the ft_ls function
	// This function will handle the logic for listing files and directories
	parse_contents(ls->start_path, &ls->files, ls->flags);
	t_syst *current;
	current = ls->files;
	if (ls->files == NULL) {
		return (1);
	}
	if (ls->flags.R) {
		list_recursive(ls->start_path, ls->flags, true);
	}
	else if (ls->flags.l)
		print_long_entries(ls->files);
	else
		print_entries(ls->files, ls->flags);
	return 0;
}
