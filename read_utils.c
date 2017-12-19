/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 16:42:25 by nmei              #+#    #+#             */
/*   Updated: 2017/12/18 15:31:20 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <libft.h>
#include "fdf.h"

static int		free_lst_or_map(t_list **lst, t_map *map)
{
	t_list	*next;

	if (lst)
	{
		while (*lst)
		{
			next = (*lst)->next;
			ft_memdel(&(*lst)->content);
			ft_memdel((void **)lst);
			*lst = next;
		}
	}
	if (map)
	{
		ft_memdel((void **)&map->verts);
		ft_memdel((void **)&map->xverts);
	}
	return (1);
}

static int		get_lines(int fd, t_list **llst, t_map *map)
{
	ssize_t	ret;
	t_list	*new;
	char	*line;

	while ((ret = ft_get_next_line(fd, &line)) == 1)
	{
		if (map->width == -1)
			map->width = (int)ft_num_words(line, ' ');
		else if (map->width != (int)ft_num_words(line, ' '))
			return (free_lst_or_map(llst, NULL));
		if ((new = ft_lstnew(line, ft_strlen(line) + 1)) == NULL)
			return (free_lst_or_map(llst, NULL));
		ft_lstadd(llst, new);
		map->height++;
		ft_strdel(&line);
	}
	if (map->width <= 0 || ret == -1)
		return (free_lst_or_map(llst, NULL));
	return (0);
}

static t_vert	gen_vert(int row, int col, char *z_str, t_map *m)
{
	t_vert	new_v;

	new_v.x = (double)col;
	new_v.y = (double)row;
	new_v.z = (double)ft_atoi(z_str);
	new_v.w = 1;
	new_v.color = 0x00FFFFFF;
	m->max_z = (new_v.z > m->max_z) ? new_v.z : m->max_z;
	m->min_z = (new_v.z < m->min_z) ? new_v.z : m->min_z;
	return (new_v);
}

static int		fill_map(t_map *m, t_list *llst)
{
	int		row;
	int		col;
	char	**split;
	t_list	*cur_elem;

	cur_elem = llst;
	row = (m->height - 1);
	while (row >= 0)
	{
		col = 0;
		if ((split = ft_strsplit((char *)cur_elem->content, ' ')) == NULL)
			return (free_lst_or_map(&llst, m));
		while (col < m->width)
		{
			m->verts[row * m->width + col] = gen_vert(row, col, split[col], m);
			col++;
		}
		ft_splitdel(&split);
		row--;
		cur_elem = cur_elem->next;
	}
	free_lst_or_map(&llst, NULL);
	return (0);
}

int				read_file(int fd, t_map *map)
{
	t_list	*line_lst;

	line_lst = NULL;
	map->width = -1;
	map->height = 0;
	map->max_z = -2147483648;
	map->min_z = 2147483647;
	if (get_lines(fd, &line_lst, map) == 1)
		return (1);
	map->verts = ft_memalloc(sizeof(t_vert) * map->width * map->height);
	map->xverts = ft_memalloc(sizeof(t_vert) * map->width * map->height);
	return (fill_map(map, line_lst));
}
