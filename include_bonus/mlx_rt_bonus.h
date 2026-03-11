/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_rt_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlavared <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 01:15:42 by tlavared          #+#    #+#             */
/*   Updated: 2026/03/11 13:50:20 by tlavared         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_RT_BONUS_H
# define MLX_RT_BONUS_H

# include "miniRt_bonus.h"

int		draw(t_minirt *minirt);
t_color	ray_color(t_ray *r, t_scene *scene);
t_color	sky_color(t_ray *r);
t_color	pixel_color_aa(t_minirt *minirt, int x, int y);
void	put(t_minirt *minirt, int x, int y, uint32_t color);
int		ft_errorinit(mlx_t *mlx);
int		ft_errorimg(mlx_t *mlx, mlx_image_t *img);
void	ft_clearimg(t_minirt *minirt);

#endif
