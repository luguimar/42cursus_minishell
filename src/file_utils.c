/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 02:21:10 by luguimar          #+#    #+#             */
/*   Updated: 2024/06/27 02:28:54 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	path_exists(char *path)
{
	struct stat	*buf;

	buf = malloc(sizeof(struct stat));
	if (stat(path, buf) == -1)
	{
		free(buf);
		return (0);
	}
	free(buf);
	return (1);
}

int	is_directory(char *path)
{
	struct stat	*buf;

	buf = malloc(sizeof(struct stat));
	if (stat(path, buf) == -1)
	{
		free(buf);
		return (0);
	}
	if (S_ISDIR(buf->st_mode))
	{
		free(buf);
		return (1);
	}
	free(buf);
	return (0);
}
