/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <student@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 00:00:00 by student           #+#    #+#             */
/*   Updated: 2026/02/02 17:04:17 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

/* ========================================================================== */
/*                               BIBLIOTECAS                                  */
/* ========================================================================== */

# include <math.h>       // Funções matemáticas: sqrt, pow, sin, cos, etc
# include <stdlib.h>     // malloc, free, exit
# include <fcntl.h>      // open, close
# include <unistd.h>     // read, write
# include <stdbool.h>    // tipo bool (true/false)

# include "vectors.h"   // Operações com vetores 3D

# include "../MLX42/include/MLX42/MLX42.h"	// Lib da mlx
# include "color.h"
# include "../libft/include/libft.h"			// lib da libft

# define WIDTH 1280
# define HEIGHT 800
# define EPSILON 1e-6

# define PI 3.14159265358979323846

/*
** Limites para intersecções de raios
** T_MIN evita "shadow acne" (quando um objeto faz sombra em si mesmo)
** T_MAX define a distância máxima que um raio pode viajar
*/

# define T_MIN 0.001         // Distância mínima para considerar uma interseção
# define T_MAX 1e30          // Distância máxima (essencialmente infinito)

typedef t_vec3	t_point3;

typedef struct s_ray
{
	t_point3	origin;
	t_vec3		direction;
}	t_ray;

typedef struct s_camera
{
	t_point3	position;
	t_vec3		orientation;
	double		fov;
	t_vec3		forward;
	t_vec3		right;
	t_vec3		up;
	double		viewport_width;
	double		viewport_height;
	t_point3	lower_left_corner;
	t_vec3		horizontal;
	t_vec3		vertical;
}	t_camera;

typedef struct s_ambient
{
	double		intensity;
	t_color		color;
}	t_ambient;

typedef struct s_light
{
	t_point3		position;
	double			brightness;
	t_color			color;
	struct s_light	*next;
}	t_light;

typedef struct s_sphere
{
	t_point3	center;
	double		diameter;
	double		radius;
	t_color		color;
}	t_sphere;

typedef struct s_plane
{
	t_point3	point;
	t_vec3		normal;
	t_color		color;
}	t_plane;

typedef struct s_cylinder
{
	t_point3	center;
	t_vec3		axis;
	double		diameter;
	double		radius;
	double		height;
	t_color		color;
}	t_cylinder;

typedef enum e_cylinder_surface
{
	CYL_BODY,
	CYL_BOTTOM_CAP,
	CYL_TOP_CAP
}	t_cylinder_surface;

typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER
}	t_object_type;

typedef struct s_object
{
	t_object_type	type;
	union u_shape
	{
		t_sphere	sphere;
		t_plane		plane;
		t_cylinder	cylinder;
	}				shape;
	struct s_object	*next;
}	t_object;

typedef struct s_hit_record
{
	t_point3			point;
	t_vec3				normal;
	double				t;
	t_color				color;
	bool				front_face;
	t_object			*object;
	t_cylinder_surface	cylinder_surface;
}	t_hit_record;

typedef struct s_quadratic_equation
{
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t;
}	t_quadratic_equation;

typedef struct s_scene
{
	t_camera	camera;
	t_ambient	ambient;
	t_light		*lights;
	t_object	*objects;
	int			width;
	int			height;
}	t_scene;

typedef struct s_mlx_data
{
	mlx_t		*mlx_ptr;
	void		*win_ptr;
	mlx_image_t	*img_ptr;
	uint8_t		*pixels;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}	t_mlx_data;
typedef struct s_minirt
{
	t_scene		scene;
	t_mlx_data	mlx;
}	t_minirt;

t_ray		ray_create(t_point3 origin, t_vec3 direction);
void		render_scene(t_minirt *rt);
t_color		calculate_lighting(t_scene *scene, t_hit_record *rec);
void		ft_free_split(char **split);
void		ft_print_error(const char *msg);
void		error_exit(const char *message);
void		ft_on_close(void *param);
void		ft_on_keypress(mlx_key_data_t keydata, void *param);
void		ft_on_resize(int32_t width, int32_t height, void *param);
#endif
