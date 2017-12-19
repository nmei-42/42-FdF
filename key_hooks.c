/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/17 18:28:08 by nmei              #+#    #+#             */
/*   Updated: 2017/12/18 14:46:44 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include <mlx.h>
#include "fdf.h"

static void	cleanup_env(t_envars *env)
{
	free((&env->map)->verts);
	free((&env->map)->xverts);
	mlx_destroy_image(env->mlx, (&env->image)->image);
}

static void	reset_env(t_envars *env)
{
	env->x_shift = 0;
	env->y_shift = 0;
	env->z_shift = 0;
	env->x_rot = 0;
	env->y_rot = 0;
	env->z_rot = 0;
	env->x_sca = 25;
	env->y_sca = 25;
	env->z_sca = 25;
}

/*
**	keydown_hook - similar to key_pressed_hook but will only be called when
**	a key has been pressed AND released.
**
**	The following key are mapped:
**	'r' (15) = reset the map back to original zoom, rotations, and shifts
**	'esc' (53) = quit the program
**	'p' (35) = point mode (turns on or off drawing of lines)
**	'q' (12) = no zbuffer mode (turns on or off zbuffering)
**	'c'	(8) = toggle color modes
*/

int			keydown_hook(int key, t_envars *env)
{
	if (key == 53)
	{
		cleanup_env(env);
		exit(0);
	}
	if (key == 35)
		env->point_mode = (env->point_mode) ? 0 : 1;
	if (key == 12)
		env->no_zbuf_mode = (env->no_zbuf_mode) ? 0 : 1;
	if (key == 8)
	{
		env->color_style = (env->color_style + 1) % 5;
		toggle_colors(env);
	}
	if (key == 15)
	{
		reset_env(env);
	}
	render(env);
	return (0);
}

/*
**	key_pressed_hook - while any of the arrow keys on the keyboard are
**	held down, shift the map in that direction.
**
**	← (123) = translate x left by 5 units
**	→ (124) = translate x right by 5 units
**	↓ (125) = translate y down by 5 units
**	↑ (126) = translate y up by 5 units
**
**	z (6) = rotates z axis counterclockwise by 1 degree
**	x (7) = rotates z axis clockwise by 1 degree
*/

int			key_pressed_hook(int key, t_envars *env)
{
	if (key == 124)
		env->x_shift += 5;
	if (key == 123)
		env->x_shift -= 5;
	if (key == 126)
		env->y_shift -= 5;
	if (key == 125)
		env->y_shift += 5;
	if (key == 6)
	{
		env->z_rot += (M_PI / 360);
		env->z_rot = fmod(env->z_rot, 360);
	}
	if (key == 7)
	{
		env->z_rot -= (M_PI / 360);
		env->z_rot = fmod(env->z_rot, 360);
	}
	render(env);
	return (0);
}
