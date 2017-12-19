/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_xform_matrices.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 23:19:12 by nmei              #+#    #+#             */
/*   Updated: 2017/12/16 12:49:42 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <libft.h>
#include "fdf.h"

/*
**	A translation matrix takes the following form:
**	1	0	0	vx
**	0	1	0	vy
**	0	0	1	vz
**	0	0	0	1
**
**	vx, vy, vz represent translations in the x, y, and z axes respectively
*/

t_trmat	trans_mat(double x_shift, double y_shift, double z_shift)
{
	t_trmat	trans_mat;

	trans_mat.rows = 4;
	trans_mat.cols = 4;
	ft_bzero(trans_mat.m, sizeof(trans_mat.m));
	trans_mat.m[idx(0, 0, 4)] = 1;
	trans_mat.m[idx(1, 1, 4)] = 1;
	trans_mat.m[idx(2, 2, 4)] = 1;
	trans_mat.m[idx(0, 3, 4)] = x_shift;
	trans_mat.m[idx(1, 3, 4)] = y_shift;
	trans_mat.m[idx(2, 3, 4)] = z_shift;
	trans_mat.m[idx(3, 3, 4)] = 1;
	return (trans_mat);
}

/*
**	Rotation about the x-axis:
**	1		0		0		0
**	0		cos		-sin	0
**	0		sin		cos		0
**	0		0		0		1
**
**	Note: for anti rotation switch the sin and -sin
**	Note 2: theta is measured in radians
*/

t_trmat	x_rot_mat(double theta, int transpose)
{
	t_trmat trans_mat;

	trans_mat.rows = 4;
	trans_mat.cols = 4;
	ft_bzero(trans_mat.m, sizeof(trans_mat.m));
	trans_mat.m[idx(0, 0, 4)] = 1;
	trans_mat.m[idx(1, 1, 4)] = cos(theta);
	trans_mat.m[idx(1, 2, 4)] = (transpose) ? -sin(theta) : sin(theta);
	trans_mat.m[idx(2, 1, 4)] = (transpose) ? sin(theta) : -sin(theta);
	trans_mat.m[idx(2, 2, 4)] = cos(theta);
	trans_mat.m[idx(3, 3, 4)] = 1;
	return (trans_mat);
}

/*
**	Rotation about the y-axis:
**	cos		0		sin		0
**	0		1		0		0
**	-sin	0		cos		0
**	0		0		0		1
**
**	Note: for anti rotation switch the sin and -sin
**	Note 2: theta is measured in radians
*/

t_trmat	y_rot_mat(double theta, int transpose)
{
	t_trmat	trans_mat;

	trans_mat.rows = 4;
	trans_mat.cols = 4;
	ft_bzero(trans_mat.m, sizeof(trans_mat.m));
	trans_mat.m[idx(0, 0, 4)] = cos(theta);
	trans_mat.m[idx(0, 2, 4)] = (transpose) ? sin(theta) : -sin(theta);
	trans_mat.m[idx(1, 1, 4)] = 1;
	trans_mat.m[idx(2, 0, 4)] = (transpose) ? -sin(theta) : sin(theta);
	trans_mat.m[idx(2, 2, 4)] = cos(theta);
	trans_mat.m[idx(3, 3, 4)] = 1;
	return (trans_mat);
}

/*
**	Rotation about the z-axis:
**	cos		-sin	0		0
**	sin		cos		0		0
**	0		0		1		0
**	0		0		0		1
**
**	Note: for anti rotation switch the sin and -sin
**	Note 2: theta is measured in radians
*/

t_trmat	z_rot_mat(double theta, int transpose)
{
	t_trmat	trans_mat;

	trans_mat.rows = 4;
	trans_mat.cols = 4;
	ft_bzero(trans_mat.m, sizeof(trans_mat.m));
	trans_mat.m[idx(0, 0, 4)] = cos(theta);
	trans_mat.m[idx(0, 1, 4)] = (transpose) ? -sin(theta) : sin(theta);
	trans_mat.m[idx(1, 0, 4)] = (transpose) ? sin(theta) : -sin(theta);
	trans_mat.m[idx(1, 1, 4)] = cos(theta);
	trans_mat.m[idx(2, 2, 4)] = 1;
	trans_mat.m[idx(3, 3, 4)] = 1;
	return (trans_mat);
}

/*
**	Scaling matrix:
**	Sx	0	0	0
**	0	Sy	0	0
**	0	0	Sz	0
**	0	0	0	1
**
**	Sx, Sy, Sz represent scalings that will be performed on x, y, and z
*/

t_trmat	scale_mat(double x_scale, double y_scale, double z_scale)
{
	t_trmat trans_mat;

	trans_mat.rows = 4;
	trans_mat.cols = 4;
	ft_bzero(trans_mat.m, sizeof(trans_mat.m));
	trans_mat.m[idx(0, 0, 4)] = x_scale;
	trans_mat.m[idx(1, 1, 4)] = y_scale;
	trans_mat.m[idx(2, 2, 4)] = z_scale;
	trans_mat.m[idx(3, 3, 4)] = 1;
	return (trans_mat);
}
