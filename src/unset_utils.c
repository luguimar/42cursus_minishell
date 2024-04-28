/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luguimar <luguimar@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:43:50 by luguimar          #+#    #+#             */
/*   Updated: 2024/04/28 08:32:10 by luguimar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_env(void *content)
{
	t_env	*env;

	env = (t_env *)content;
	free(env->key);
	free(env->value);
	free(env->full);
	free(env->export_version);
	free(env);
}

void	delete_env_node(t_list **env, char *key)
{
	t_list	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (!ft_strcmp(((t_env *)tmp->content)->key, key))
		{
			ft_lstdelone(env, tmp, free_env);
			return ;
		}
		tmp = tmp->next;
	}
}
