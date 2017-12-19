/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 09:52:48 by nmei              #+#    #+#             */
/*   Updated: 2017/12/18 18:59:57 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mlx.h>
#include <libft.h>
#include "fdf.h"

static void		setup_env(t_envars *env)
{
	env->mlx = mlx_init();
	env->win = mlx_new_window(env->mlx, WIN_W, WIN_H, "FdF 42");
	init_img(env);
	init_mouse(env);
	env->x_shift = 0;
	env->y_shift = 0;
	env->z_shift = 0;
	env->x_rot = 0;
	env->y_rot = 0;
	env->z_rot = 0;
	env->x_sca = 25;
	env->y_sca = 25;
	env->z_sca = 25;
	env->color_style = 0;
	env->c_max = 0xFFFFFF;
	env->c_min = 0xFFFFFF;
	env->point_mode = 0;
	env->no_zbuf_mode = 0;
}

static void		disp_usage(void)
{
	ft_putstr("\n						FdF 42!!\n\
				Mouse click and drag: Rotate x/y axes\n\
				Mouse scroll up/down: Zoom in/out\n\
				Arrow keys: Translate x/y axes\n\
				'z/x' keys: Rotates z axis\n\
				c: Change color modes\n\
				p: Turn on/off point only mode\n\
				q: Turn on/off Z-buffering\n\
				r: Reset map translations, rotations, and scalings\n\
				esc: Quit :(\n\n");
}

/*
**	main - function where we start our mlx session
**	There are additional hooks that can be found!
**	See: https://github.com/qst0/ft_libgfx
**	Of particular interest are the mouse press, release, and movement hooks
**	which are mapped to 4, 5, and 6 respectively.
*/

int				main(int argc, char *argv[])
{
	t_envars	env;
	int			fd;

	if (argc != 2)
	{
		write(2, "usage: ./fdf [target_file.fdf]\n", 31);
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0 || read_file(fd, &(env.map)) != 0)
	{
		write(2, "An error occurred reading the file... Is it valid?\n", 51);
		return (1);
	}
	close(fd);
	setup_env(&env);
	disp_usage();
	render(&env);
	mlx_key_hook(env.win, keydown_hook, &env);
	mlx_hook(env.win, 2, 0, key_pressed_hook, &env);
	mlx_hook(env.win, 4, 0, mouse_pressed_hook, &env);
	mlx_hook(env.win, 5, 0, mouse_released_hook, &env);
	mlx_hook(env.win, 6, 0, mouse_moved_hook, &env);
	mlx_loop(env.mlx);
	return (0);
}
