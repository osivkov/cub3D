/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:00:49 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/27 16:07:20 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_step_data(t_cfg *cfg, double ray_dir, t_step_dist *sd)
{
	sd->map_x = (int)cfg->pl.x;
	sd->map_y = (int)cfg->pl.y;
	sd->delta_x = fabs(1 / cos(ray_dir));
	sd->delta_y = fabs(1 / sin(ray_dir));
}

static t_step_dist	calc_steps_side_dist(t_cfg *cfg, double ray_dir)
{
	t_step_dist	sd;

	init_step_data(cfg, ray_dir, &sd);
	if (cos(ray_dir) < 0)
	{
		sd.step_x = -1;
		sd.side_dist_x = (cfg->pl.x - sd.map_x) * sd.delta_x;
	}
	else
	{
		sd.step_x = 1;
		sd.side_dist_x = (sd.map_x + 1.0 - cfg->pl.x) * sd.delta_x;
	}
	if (sin(ray_dir) < 0)
	{
		sd.step_y = -1;
		sd.side_dist_y = (cfg->pl.y - sd.map_y) * sd.delta_y;
	}
	else
	{
		sd.step_y = 1;
		sd.side_dist_y = (sd.map_y + 1.0 - cfg->pl.y) * sd.delta_y;
	}
	return (sd);
}

static void	raycast_dda_step(t_step_dist *sd)
{
	if (sd->side_dist_x < sd->side_dist_y)
	{
		sd->side_dist_x += sd->delta_x;
		sd->map_x += sd->step_x;
		sd->side = 0;
	}
	else
	{
		sd->side_dist_y += sd->delta_y;
		sd->map_y += sd->step_y;
		sd->side = 1;
	}
}

static double	raycast_calc_result(t_cfg *cfg, double ray_dir, \
	t_step_dist *sd, int *line_h)
{
	double	dist;
	double	perp;

	if (sd->side == 0)
		dist = sd->side_dist_x - sd->delta_x;
	else
		dist = sd->side_dist_y - sd->delta_y;
	perp = dist * cos(ray_dir - cfg->pl.dir);
	if (sd->side == 0)
		sd->wall_x = cfg->pl.y + dist * sin(ray_dir);
	else
		sd->wall_x = cfg->pl.x + dist * cos(ray_dir);
	sd->wall_x -= floor(sd->wall_x);
	*line_h = (int)(WIN_H / perp);
	if (sd->side == 0 && cos(ray_dir) > 0)
		cfg->tex_id = TEX_EA;
	else if (sd->side == 0 && cos(ray_dir) < 0)
		cfg->tex_id = TEX_WE;
	else if (sd->side == 1 && sin(ray_dir) > 0)
		cfg->tex_id = TEX_SO;
	else
		cfg->tex_id = TEX_NO;
	return (perp);
}

double	raycast(t_cfg *cfg, int col, int *line_h, double *wall_x)
{
	double		ray_dir;
	t_step_dist	sd;
	double		camera_x;
	double		perp;

	camera_x = (2.0 * col / (double)WIN_W) - 1.0;
	ray_dir = cfg->pl.dir + camera_x * M_PI / 6.0;
	sd = calc_steps_side_dist(cfg, ray_dir);
	while (1)
	{
		raycast_dda_step(&sd);
		if (hit_wall(cfg, sd.map_x, sd.map_y))
			break ;
	}
	perp = raycast_calc_result(cfg, ray_dir, &sd, line_h);
	*wall_x = sd.wall_x;
	return (perp);
}
