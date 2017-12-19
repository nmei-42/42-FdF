/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/15 20:40:26 by nmei              #+#    #+#             */
/*   Updated: 2017/12/18 14:44:56 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <libft.h>
#include "fdf.h"

/*
**	init_img - use mlx functions to initialize our image that we will be
**	drawing our maps on!
**
**	The initial bits per pixel (bpp) that the mlx_get_data_addr function returns
**	is 32 bits. Because the colors we're dealing with are RGB that max out at
**	0xFF (255) per channel we divide the bpp by 8 bits (nbr bits per channel)
**	so that our addressing will work out.
*/

void		init_img(t_envars *env)
{
	t_image	*img;

	img = &env->image;
	img->image = mlx_new_image(env->mlx, WIN_W, WIN_H);
	img->ptr = mlx_get_data_addr(img->image, &img->bpp, &img->line_s,
								&img->endian);
	img->bpp /= 8;
}

void		img_pixel_put(t_image *img, double x, double y, int color)
{
	if (x >= 0 && x < WIN_W && y >= 0 && y < WIN_H)
		*(int *)(img->ptr + (int)(idx(y, x, WIN_W) * img->bpp)) = color;
}

void		clear_img(t_image *img)
{
	ft_bzero(img->ptr, WIN_W * WIN_H * img->bpp);
}

/*
**	Initialize or re-initialize our z-buffer.
**	Most negative int is the FARTHEST in the z direction a point could possibly
**	be if we're using ints to represent our z.
*/

void		clear_zbuf(t_map *map)
{
	int len;

	len = (int)(WIN_H * WIN_W);
	while (--len > -1)
		map->zbuf[len] = -2147483648;
}
