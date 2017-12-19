/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/15 19:07:49 by nmei              #+#    #+#             */
/*   Updated: 2017/12/18 14:48:35 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include "fdf.h"

/*
**	xform() - where the magic happens...
**	Given a map with a set of vertices (x, y, z, w)
**	1) translate the map so that the *CENTER* of the map is @ (0,0) origin
**	2) Scale the x, y, and z evenly (for zoom in and out effects)
**	3-5) apply rotations in the order of x, y, then z axes
**	4) Now, shift the center of our map @ (0,0) origin to the middle of the
**	   drawing window i.e. (WIN_W / 2, WIN_H / 2)
**	5) If we want to translate the map around starting from the middle
**	   of our window, then apply those transformations then.
**	6) Calculate the color of our resulting vertex
**
**	Note: calculation of vertex color doesn't strictly have to occur in this
**	function, but can be useful if I decide to implement color based on
**	transformed Z values instead of the current implementation where color is
**	based on the untransformed Z values.
*/

static t_vert	xform(t_envars *env, t_map *map, int row, int col)
{
	t_vert	res;
	t_vert	curr_vert;
	double	mid_map_w;
	double	mid_map_h;
	double	mid_map_z;

	mid_map_w = (double)(map->width - 1) / 2;
	mid_map_h = (double)(map->height - 1) / 2;
	mid_map_z = (double)(map->max_z + map->min_z) / 2;
	curr_vert = map->verts[idx(row, col, map->width)];
	res = m_v_mult(trans_mat(-mid_map_w, -mid_map_h, -mid_map_z), curr_vert);
	res = m_v_mult(scale_mat(env->x_sca, env->y_sca, env->z_sca), res);
	res = m_v_mult(x_rot_mat(env->x_rot, 0), res);
	res = m_v_mult(y_rot_mat(env->y_rot, 0), res);
	res = m_v_mult(z_rot_mat(env->z_rot, 0), res);
	res = m_v_mult(trans_mat((WIN_W / 2), (WIN_H / 2), 0), res);
	res = m_v_mult(trans_mat(env->x_shift, env->y_shift, env->z_shift), res);
	res.color = calc_vert_color(env, map, curr_vert);
	return (res);
}

/*
**	init_line() - init the vars needed for Bresenham line drawing algorithm
**	Because p1 --> p2 may be a line with many different positive or negative
**	slopes we need to account for all 8 possibilities.
**
**	1) Take the absolute value of dx and dy in the case that x1/y1 are
**	smaller than x0/y0
**	2) If x0 < x1 then we are moving right so the amount we add to x is +1
**	   If x0 > x1 then we are moving left so the amount we add to x is -1
**	3) If y0 < y1 then we are moving down so the amount we add to y is +1
**	   If y0 > y1 then we are heading up so the amount we add to y is -1
**	4) If dx > dy then we have a flatter line so make dx the driving axis.
**	5) If dy > dx then we have a steeper line so make dy the driving axis.
**
**	Note: you might think 3) is mistaken in directions but recall that in our
**	graphics coordinate system 0,0 is the top left and 0, WIN_H is the bottom
**	left.
*/

static t_line	init_line(t_vert p1, t_vert p2)
{
	t_line	nl;

	nl.x0 = (int)p1.x;
	nl.y0 = (int)p1.y;
	nl.x1 = (int)p2.x;
	nl.y1 = (int)p2.y;
	nl.dx = ft_abs(nl.x1 - nl.x0);
	nl.sx = (nl.x0 < nl.x1) ? 1 : -1;
	nl.dy = ft_abs(nl.y1 - nl.y0);
	nl.sy = (nl.y0 < nl.y1) ? 1 : -1;
	nl.err = ((nl.dx > nl.dy) ? nl.dx : -nl.dy) / 2;
	nl.dec_pcnt = 0.0;
	return (nl);
}

/*
**	draw_line() - int ver. of Bresenham line drawing algorithm between p1 & p2
**	1) Initialize variables for line drawing (see above)
**	2) determine for our transformed x0, y0 how far we are to our goal to get
**	   a line percent.
**	3) Use the line percent determined in 2) to linearly interpolate a color
**	4) Check if our new pixel is occluded and paint it, if it's not.
**	5) If our error term is > -dx we have moved (right or left) and need to
**	   update our x0 position accordingly.
**	6) If our error term is < dy we have moved (up or down) and need to update
**	   our y0 position accordingly.
*/

static void		draw_line(t_envars *env, t_vert p1, t_vert p2)
{
	t_line	l;

	l = init_line(p1, p2);
	while (1)
	{
		l.dec_pcnt = (l.dx > l.dy) ? get_line_pcnt(p1.x, p2.x, l.x0)
					: get_line_pcnt(p1.y, p2.y, l.y0);
		l.color = color_lint(p1.color, p2.color, l.dec_pcnt);
		if (is_occluded(l, env, p1, p2) == 0)
			img_pixel_put(&env->image, l.x0, l.y0, l.color);
		if (l.x0 == l.x1 && l.y0 == l.y1)
			break ;
		l.err2 = l.err;
		if (l.err2 > -l.dx)
		{
			l.err -= l.dy;
			l.x0 += l.sx;
		}
		if (l.err2 < l.dy)
		{
			l.err += l.dx;
			l.y0 += l.sy;
		}
	}
}

/*
**	position_verts() - Given a map and a set of transformations that we wish
**	to do to it, this function will apply the desired transformations. The
**	transformations will be applied starting from the bottom right corner of the
**	map.
**
**	A future improvement would be to parallelize the positioning of the
**	vertices since the task is embarassingly parallel... (though only if
**	we are really really desparate for performance)
*/

static void		position_verts(t_envars *env)
{
	t_vert	res;
	int		row;
	int		col;
	int		map_w;

	map_w = env->map.width;
	row = env->map.height;
	while (--row > -1)
	{
		col = map_w;
		while (--col > -1)
		{
			res = xform(env, &env->map, row, col);
			env->map.xverts[idx(row, col, map_w)] = res;
			img_pixel_put(&env->image, res.x, res.y, res.color);
		}
	}
}

/*
**	render() - we're going to call this A LOT
**	1) Clear existing image
**	2) Clear our z-buffer values (only if we're actually z-buffering)
**	3) Apply transformations to all our vertices
**	4) Loop through all our transformed vertices starting from the bottom right
**	   corner. If we can, draw a line between our current xformed vertex and
**	   the vertices to the right and below in our transormed vertex grid.
**	5) Now, put the resulting image to the window.
*/

void			render(t_envars *env)
{
	t_vert	res;
	int		row;
	int		col;
	int		map_w;

	clear_img(&env->image);
	if (env->no_zbuf_mode == 0)
		clear_zbuf(&env->map);
	position_verts(env);
	map_w = env->map.width;
	row = env->map.height;
	while (--row > -1)
	{
		col = map_w;
		while (--col > -1)
		{
			res = env->map.xverts[idx(row, col, map_w)];
			if (row + 1 < env->map.height)
				draw_line(env, res, env->map.xverts[idx(row + 1, col, map_w)]);
			if (col + 1 < map_w)
				draw_line(env, res, env->map.xverts[idx(row, col + 1, map_w)]);
		}
	}
	mlx_put_image_to_window(env->mlx, env->win, env->image.image, 0, 0);
}
