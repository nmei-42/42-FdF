/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmei <nmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/12/14 14:42:17 by nmei              #+#    #+#             */
/*   Updated: 2017/12/18 13:28:45 by nmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H
# define WIN_W	1440
# define WIN_H	900

typedef struct	s_vert
{
	double		x;
	double		y;
	double		z;
	double		w;
	int			color;
}				t_vert;

typedef struct	s_map
{
	int			width;
	int			height;
	int			max_z;
	int			min_z;
	int			zbuf[WIN_H * WIN_W];
	t_vert		*verts;
	t_vert		*xverts;
}				t_map;

typedef struct	s_trmat
{
	int			rows;
	int			cols;
	double		m[16];
}				t_trmat;

typedef struct	s_image
{
	void		*image;
	char		*ptr;
	int			bpp;
	int			line_s;
	int			endian;
}				t_image;

typedef struct	s_mouse
{
	double		prev_x;
	double		prev_y;
	double		x;
	double		y;
	int			is_pressed;
}				t_mouse;

typedef struct	s_envars
{
	void		*mlx;
	void		*win;
	t_image		image;
	t_map		map;
	t_mouse		mouse;
	double		x_shift;
	double		y_shift;
	double		z_shift;
	double		x_rot;
	double		y_rot;
	double		z_rot;
	double		x_sca;
	double		y_sca;
	double		z_sca;
	int			c_min;
	int			c_max;
	int			color_style;
	int			point_mode;
	int			no_zbuf_mode;
}				t_envars;

typedef struct	s_line
{
	int			x0;
	int			y0;
	int			x1;
	int			y1;
	int			dx;
	int			sx;
	int			dy;
	int			sy;
	int			err;
	int			err2;
	int			color;
	double		dec_pcnt;
}				t_line;

/*
**	read_util.c
*/
int				read_file(int fd, t_map *map);

/*
**	matrix_utils.c
*/
int				idx(int row, int col, int dim);
t_vert			m_v_mult(t_trmat m, t_vert vec);

/*
**	prep_xform_matrices.c
*/
t_trmat			trans_mat(double x_shift, double y_shift, double z_shift);
t_trmat			x_rot_mat(double theta, int transpose);
t_trmat			y_rot_mat(double theta, int transpose);
t_trmat			z_rot_mat(double theta, int transpose);
t_trmat			scale_mat(double x_scale, double y_scale, double z_scale);

/*
**	occlusion.c
*/
int				is_occluded(t_line l, t_envars *env, t_vert p1, t_vert p2);

/*
**	image.c
*/
void			init_img(t_envars *env);
void			img_pixel_put(t_image *img, double x, double y, int color);
void			clear_img(t_image *img);
void			clear_zbuf(t_map *map);

/*
**	color.c
*/
double			ft_lint(double start, double end, double decimal_percent);
int				color_lint(int c1, int c2, double decimal_percent);
double			get_line_pcnt(double start, double end, double curr);
int				calc_vert_color(t_envars *env, t_map *map, t_vert curr_vert);
void			toggle_colors(t_envars *env);

/*
**	fdf_render.c
*/
void			render(t_envars *env);

/*
**	key_hooks.c
*/
int				keydown_hook(int key, t_envars *env);
int				key_pressed_hook(int key, t_envars *env);

/*
**	mouse_hooks.c
*/
void			init_mouse(t_envars *env);
int				mouse_pressed_hook(int button, int x, int y, t_envars *env);
int				mouse_released_hook(int button, int x, int y, t_envars *env);
int				mouse_moved_hook(int x, int y, t_envars *env);

#endif
