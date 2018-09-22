/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekelen <ekelen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 10:39:23 by lsimon            #+#    #+#             */
/*   Updated: 2018/09/22 14:23:59 by ekelen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"

t_manager	g_manager;

static t_stock		*get_linked(t_stock *curr, t_stype sz, int *err)
{
	t_stock	*new;

	if (!curr)
	{
		new = init_stock(sz);
		*err = new ? *err : *err + 1;
		return (new);
	}
	if (curr->free_bits < (size_t)sz + sizeof(t_malloc))
		curr->next = get_linked(curr->next, sz, err);
	return (curr);
}

void				*malloc(size_t req_size)
{
	int	err;

	err = 0;
	if (req_size <= TINY)
	{
		g_manager.tiny = get_linked(g_manager.tiny, TINY, &err);
		return (err ? NULL : \
		retrieve_chunk(get_usable_stock(g_manager.tiny, req_size),\
		req_size));
	}
	if (req_size <= SMALL)
	{
		g_manager.small = get_linked(g_manager.small, SMALL, &err);
		return (err ? NULL : \
		retrieve_chunk(get_usable_stock(g_manager.small, req_size),\
		req_size));
	}
	if (req_size > SMALL)
	{
		g_manager.large = add_large_node(g_manager.large, req_size, &err);
		return (err ? NULL : retrieve_large_tail(g_manager.large));
	}
	return (NULL);
}
