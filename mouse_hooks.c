/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/17 14:16:38 by nmei              #+#    #+#             */
/*   Updated: 2017/12/18 14:48:18 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

void	init_mouse(t_envars *env)
{
	t_mouse		*new_mouse;

	new_mouse = &env->mouse;
	new_mouse->is_pressed = 0;
	new_mouse->prev_x = 0;
	new_mouse->prev_y = 0;
	new_mouse->x = 0;
	new_mouse->y = 0;
}

/*
**	mouse_pressed_hook - hook for the mouse pressed event that is passed to
**	mlx_hook(). mlx_hook() will pass "int button, int x, int y" followed by
**	any params also passed to mlx_hook() to the function when the mouse is
**	pressed.
**
**	Button can take on multiple values!!!
**	1 = mouse left button click
**	2 = mouse right button click
**	3 = ????? (maybe clicking the scroll wheel???)
**	4 = mouse scroll wheel down
**	5 = mouse scroll wheel up
**	6 = mouse scroll wheel right
**	7 = mouse scroll wheel left
**
**	NOTE: all x and y coordinates reported to the mouse hook functions will
**	have (0, 0) being the BOTTOM LEFT CORNER of the window. (WIN_W, WIN_H) will
**	thus be the TOP RIGHT CORNER of the window... which is completely different
**	from the image's (0,0) which is TOP LEFT CORNER... go figure...
**
**	NOTE: DO NOT TRY TO CHANGE THE PARAMETER TYPES OF X & Y FROM INT TO DOUBLE!!
**	Doing so will corrupt the addressing of your params and give you segfaults
**	that will take you possibly hours to debug (at least for me...)
*/

int		mouse_pressed_hook(int button, int x, int y, t_envars *env)
{
	if (x >= 0 && x <= WIN_W && y >= 0 && y <= WIN_H)
	{
		if (button == 1 || button == 2)
		{
			(&env->mouse)->is_pressed = 1;
			(&env->mouse)->x = x;
			(&env->mouse)->y = y;
		}
		if (button == 4)
		{
			env->x_sca -= 1;
			env->y_sca -= 1;
			env->z_sca -= 1;
		}
		if (button == 5)
		{
			env->x_sca += 1;
			env->y_sca += 1;
			env->z_sca += 1;
		}
		if (button == 4 || button == 5)
			render(env);
	}
	return (0);
}

/*
**	NOTE: DO NOT TRY TO CHANGE THE PARAMETER TYPES OF X & Y FROM INT TO DOUBLE!!
*/

int		mouse_released_hook(int button, int x, int y, t_envars *env)
{
	t_mouse		*mouse;

	mouse = &env->mouse;
	(void)button;
	(void)x;
	(void)y;
	mouse->is_pressed = 0;
	return (0);
}

/*
**	NOTE: DO NOT TRY TO CHANGE THE PARAMETER TYPES OF X & Y FROM INT TO DOUBLE!!
*/

int		mouse_moved_hook(int x, int y, t_envars *env)
{
	t_mouse		*mouse;

	mouse = &env->mouse;
	if (x >= 0 && x <= WIN_W && y >= 0 && y <= WIN_H)
	{
		mouse->prev_x = mouse->x;
		mouse->prev_y = mouse->y;
		mouse->x = x;
		mouse->y = y;
		if (mouse->is_pressed)
		{
			env->y_rot -= (mouse->prev_x - x) / 150;
			env->x_rot += (mouse->prev_y - y) / 150;
		}
		if (mouse->is_pressed)
			render(env);
	}
	return (0);
}
