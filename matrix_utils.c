/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 14:41:40 by nmei              #+#    #+#             */
/*   Updated: 2017/12/18 14:48:02 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

/*
**	idx() - Useful function to get the row, col value from a flattened
**	2D matrix. Dim the is the 'width' of the matrix...
*/

int		idx(int row, int col, int dim)
{
	return (row * dim + col);
}

void	init_vert(t_vert *new_vert)
{
	new_vert->x = 0;
	new_vert->y = 0;
	new_vert->z = 0;
	new_vert->w = 0;
	new_vert->color = 0;
}

/*
**	Compute the dot product between a 4 x 4 transformation *matrix*
**	(which we are representing as a 16 element 'double' array) and a
**	a 4 x 1 *vector* which we are representing as a t_vert struct.
**
**	Lots of loop unrolling for (hopeful) speed optimizations...
**
**	This matrix function is hyper specialized for this project so...
**	DO NOT USE FOR GENERAL PURPOSE MATRIX-VECTOR MULTIPLICATION
**	YOU'RE GONNA HAVE A BAD TIME...
*/

t_vert	m_v_mult(t_trmat m, t_vert vec)
{
	int		i;
	t_vert	new_vert;
	double	*new_vert_ind[4];

	init_vert(&new_vert);
	new_vert_ind[0] = &new_vert.x;
	new_vert_ind[1] = &new_vert.y;
	new_vert_ind[2] = &new_vert.z;
	new_vert_ind[3] = &new_vert.w;
	i = 0;
	while (i <= 2)
	{
		*new_vert_ind[i] += m.m[idx(i, 0, 4)] * vec.x;
		*new_vert_ind[i] += m.m[idx(i, 1, 4)] * vec.y;
		*new_vert_ind[i] += m.m[idx(i, 2, 4)] * vec.z;
		*new_vert_ind[i] += m.m[idx(i, 3, 4)] * vec.w;
		*new_vert_ind[i + 1] += m.m[idx(i + 1, 0, 4)] * vec.x;
		*new_vert_ind[i + 1] += m.m[idx(i + 1, 1, 4)] * vec.y;
		*new_vert_ind[i + 1] += m.m[idx(i + 1, 2, 4)] * vec.z;
		*new_vert_ind[i + 1] += m.m[idx(i + 1, 3, 4)] * vec.w;
		i += 2;
	}
	return (new_vert);
}
