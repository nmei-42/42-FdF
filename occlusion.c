/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   occlusion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/17 23:47:26 by nmei              #+#    #+#             */
/*   Updated: 2017/12/18 13:33:53 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
**	is_occluded() - stupid simple implementation of a z-buffer
**	1) If we want to just display vertex points just always return 1
**	2) If we want to see the effect of no z-buffer always return 0;
**	3) Check if our line x and y are within the bounds of our window. If they
**	   are out of bounds we don't want to continue since that will give us a
**	   segfault when we try to check the z-buffer
**	4) Since we don't actually know the z value for our line pixel lets make
**	   our best guess via linear interpolation based on the progress of the line
**	5) If our z estimate is greater than what's in the zbuffer great! Let's
**	   update our zbuffer and return 0 (not occluded!)
**	6) If our z estimate is smaller than what's in the zbuffer, also great!
**	   That just means our current z estimate is being occluded!
*/

int		is_occluded(t_line l, t_envars *env, t_vert p1, t_vert p2)
{
	int		*zbuffer;
	int		z_est;

	zbuffer = (&env->map)->zbuf;
	if (env->point_mode == 1)
		return (1);
	if (env->no_zbuf_mode == 1)
		return (0);
	if (l.y0 >= 0 && l.y0 < WIN_H && l.x0 >= 0 && l.x0 < WIN_W)
	{
		z_est = (int)ft_lint(p1.z, p2.z, l.dec_pcnt);
		if (z_est > zbuffer[idx(l.y0, l.x0, WIN_W)])
		{
			zbuffer[idx(l.y0, l.x0, WIN_W)] = (int)z_est;
			return (0);
		}
	}
	return (1);
}
