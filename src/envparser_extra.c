/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envparser_extra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 05:35:08 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/17 05:37:37 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(t_list *env, char *key)
{
	while (env)
	{
		if (!ft_strcmp(((t_env *)env->content)->key, key))
			return (((t_env *)env->content)->value);
		env = env->next;
	}
	return (NULL);
}
