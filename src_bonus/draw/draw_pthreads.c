/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_pthreads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 01:44:49 by tlavared          #+#    #+#             */
/*   Updated: 2026/03/04 17:06:32 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include_bonus/miniRt.h"
#include "../include_bonus/mlx_rt.h"
#include "../include_bonus/color.h"
#include "../include_bonus/intersect.h"
#include <stdlib.h>

#define NUM_THREADS 10

/*
 *
 * Uma alternativa para o rand().
 *
 * Static e inline para o compilador otimizar a operação
 *
 * É static por que o valor precisa mudar a cada chamada
*/
/*
static inline	int fast_rand(void)
{
	static unsigned int seed = 12345;

	seed = 214013 * seed + 2531011;
	return ((seed >> 16) &  0x7FFF);
}

static double	random_double(void)
{
	return (fast_rand() / (double ) 0x7FFF);
}
*/
// NOVA FUNÇÃO: anti-aliasing

t_color	sky_color(t_ray *r)
{
	t_vec3	unit_dir;
	double	t;
	t_color	white;
	t_color	blue;

	unit_dir = r->direction;
	white = init_color(0xFFFFFFFF);
	blue = init_color(0x0066FFFF);
	t = 0.5 * (unit_dir.y + 1.0);
	return (color_add(
			color_scale(white, 1.0 - t),
			color_scale(blue, t)));
}

t_color	ray_color(t_ray *r, t_scene *scene)
{
	t_hit_record	rec;
	t_color			norm_color;

	if (hit_scene(r, scene->objects, &rec, T_MIN))
	{
		norm_color = calculate_lighting(scene, &rec, r,
				REFLECTION_DEPTH);
		return (norm_color);
	}
	return (sky_color(r));
}

static void	*render_rows(void *arg)
{
	t_thread_data	*data;
	int		x;
	int		y;
	t_color	color;

	data = (t_thread_data *) arg;
	y = data->y_start;
	while (y < data->y_end)
	{
		x = 0;
		while (x < data->minirt->scene.width)
		{
			color = pixel_color_aa(data->minirt, x, y);
			put(data->minirt, x, y, color_to_int32(color));
			x++;
		}
		y++;
	}
	return (NULL);
}

static void	drawing(t_minirt *minirt)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	data[NUM_THREADS];
	int				rows_per_thread;
	int				i;

	rows_per_thread = minirt->scene.height / NUM_THREADS;
	i = 0;
	while (i < NUM_THREADS)
	{
		data[i].minirt = minirt;
		data[i].y_start = i * rows_per_thread;
		if (i == NUM_THREADS - 1)
			data[i].y_end = minirt->scene.height;
		else
			data[i].y_end = data[i].y_start + rows_per_thread;
		data[i].seed = 12345 * (i + 1);
		pthread_create(&threads[i], NULL, render_rows, &data[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
		pthread_join(threads[i++], NULL);
}

int	draw(t_minirt *minirt)
{
	ft_clearimg(minirt);
	drawing(minirt);
	return (0);
}
