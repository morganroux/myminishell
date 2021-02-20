#include "minishell.h"

t_command	g_globstruct;

void    init_globals(char *envp[])
{
    int len;
    int i;

    i = -1;
    ft_bzero(&g_globstruct, sizeof(t_command));
    len = get_strs_len(envp);
    g_globstruct.env_arr = (char **) malloc(sizeof(char *) * (len + 1));
    while (++i < len)
        g_globstruct.env_arr[i] = ft_strdup(envp[i]);
    g_globstruct.env_arr[i] = ft_strdup("");
}

/*
** SIGINT : ^C
** SIGQUIT : ^\
*/

void		sig_int(int signal)
{
	ft_putstr_fd("\n", STDOUT_FILENO);
	if (!(g_globstruct.pid))
		prompt(&g_globstruct);
	else
	{
		prompt(&g_globstruct);
		kill(g_globstruct.pid, signal);
	}
	g_globstruct.ret = 130;
}

void		sig_quit(int signal)
{
	if (g_globstruct.pid)
	{
		kill(g_globstruct.pid, signal);
		g_globstruct.ret = 131;
		ft_putstr_fd("\nquit (core dumped)\n", STDERR_FILENO);
	}
}

void    signal_callback(int signumber)
{
    if (signumber == SIGQUIT)
        sig_quit(signumber);
    if (signumber == SIGINT)
        sig_int(signumber);
}