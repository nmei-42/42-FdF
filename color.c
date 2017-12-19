/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/17 19:41:25 by nmei              #+#    #+#             */
/*   Updated: 2017/12/18 15:32:03 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
**	ft_lint - Linear INTerpolation between two points given a decimal percent
**
**	We don't do: (start + decimal_percent * (end - start)) because if
**	start and end differ significantly in magnitude then we lose precision.
**
**	Test with: ft_lint(-16.0e30, 16.0, 1.0)
*/

double		ft_lint(double start, double end, double decimal_percent)
{
	if (start == end)
		return (start);
	return (start * (1.0 - decimal_percent) + (end * decimal_percent));
}

/*
**	color_lint - generates an intermediate color between c1 and c2.
**
**	Colors are stored in the following hex format:
**	0 x |  F F  |   F F   |  F F  |
**	    |  red  |  green  | blue  |
**
**	So what we can do is mask and isolate each color channel using bit shifting
**	and perform linear interpolation on each channel before recombining.
**
**	Each channel is 8 bits.
*/

int			color_lint(int c1, int c2, double decimal_percent)
{
	int	r;
	int	g;
	int	b;

	if (c1 == c2)
		return (c1);
	if (decimal_percent == 0.0)
		return (c1);
	if (decimal_percent == 1.0)
		return (c2);
	r = (int)ft_lint(((c1 >> 16) & 0xFF), ((c2 >> 16) & 0xFF), decimal_percent);
	g = (int)ft_lint(((c1 >> 8) & 0xFF), ((c2 >> 8) & 0xFF), decimal_percent);
	b = (int)ft_lint((c1 & 0xFF), (c2 & 0xFF), decimal_percent);
	return (r << 16 | g << 8 | b);
}

double		get_line_pcnt(double start, double end, double curr)
{
	if (curr == start)
		return (0.0);
	if (curr == end)
		return (1.0);
	if (start == end)
		return (0.0);
	return ((curr - start) / (end - start));
}

int			calc_vert_color(t_envars *env, t_map *map, t_vert curr_vert)
{
	int		color;
	double	divisor;
	double	dec_pcnt;

	divisor = map->max_z - map->min_z;
	if (divisor != 0)
		dec_pcnt = (curr_vert.z - map->min_z) / divisor;
	else
		dec_pcnt = 0.0;
	color = color_lint(env->c_min, env->c_max, dec_pcnt);
	return (color);
}

/*
**	toggle_colors
**	style 0: vanilla
**	style 1: red --> white
**	style 2: green --> white
**	style 3: blue --> white
**	style 4: XMAS
*/

void		toggle_colors(t_envars *env)
{
	if (env->color_style == 0)
	{
		env->c_max = 0xFFFFFF;
		env->c_min = 0xFFFFFF;
	}
	if (env->color_style == 1)
	{
		env->c_max = 0xFFFFFF;
		env->c_min = 0xFF0000;
	}
	if (env->color_style == 2)
	{
		env->c_max = 0xFFFFFF;
		env->c_min = 0x00FF00;
	}
	if (env->color_style == 3)
	{
		env->c_max = 0xFFFFFF;
		env->c_min = 0x0000FF;
	}
	if (env->color_style == 4)
	{
		env->c_max = 0xFF0000;
		env->c_min = 0x00FF00;
	}
}
