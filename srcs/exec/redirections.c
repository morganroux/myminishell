/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkanaev <alkanaev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/29 14:17:29 by alkanaev          #+#    #+#             */
/*   Updated: 2021/01/29 14:31:47 by alkanaev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		*open_fds_in(t_command *content)
{
	char		*path;
	int			i;
	int			*fd;
	t_list_str	*files;

	files = content->files_in;
	i = 0;
	if (!(fd = malloc(sizeof(int) * ft_lstsize(files))))
		return (NULL);
	while (files != NULL)
	{
		path = files->content;
		fd[i] = open(path, O_RDONLY, S_IRWXU);
		if (fd[i] < 0)
			return (NULL);
		files = files->next;
		i++;
	}
	return (fd);
}

int		*open_fds_out(t_command *content)
{
	char		*path;
	int			i;
	int			*fd;
	t_list_str	*files_out;
	t_list_str	*files_append;

	files_out = content->files_out;
	files_append = content->files_append;
	i = 0;
	if (!(fd = malloc(sizeof(int) *
		(ft_lstsize(files_out) + ft_lstsize(files_append)))))
		return (NULL);
	while (files_out != NULL)
	{
		path = files_out->content;
		fd[i] = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
		if (fd[i] < 0)
			return (NULL);
		files_out = files_out->next;
		i++;
	}
	while (files_append != NULL)
	{
		path = files_append->content;
		fd[i] = open(path, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU);
		if (fd[i] < 0)
			return (NULL);
		files_append = files_append->next;
		i++;
	}
	return (fd);
}

int		open_redirections(t_list_cmd *cmd)
{
	t_command	*content;

	content = cmd->content;
	if ((content->fd_in = open_fds_in(content)) == NULL ||
		(content->fd_out = open_fds_out(content)) == NULL)
		return (-1);
	return (0);
}

int		close_fds(int *fds, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		if (close(fds[i++]) == -1)
			return (-1);
	}
	return (0);
}

int		close_redirections(t_list_cmd *cmd)
{
	t_command	*content;

	content = cmd->content;
	close_fds(content->fd_in, ft_lstsize(content->files_in));
	close_fds(content->fd_out, ft_lstsize(content->files_out));
	return (0);
}
