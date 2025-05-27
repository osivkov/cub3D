/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_move.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:32:10 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/27 12:21:39 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	move_forward(t_cfg *cfg)
{
	double	new_x;
	double	new_y;
	double	pad_x;
	double	pad_y;

	new_x = cfg->pl.x + cos(cfg->pl.dir) * MOVE_SPEED;
	new_y = cfg->pl.y + sin(cfg->pl.dir) * MOVE_SPEED;
	pad_x = get_padding_sign(cos(cfg->pl.dir), PADDING);
	pad_y = get_padding_sign(sin(cfg->pl.dir), PADDING);
	if (!hit_wall(cfg, (int)(new_x + pad_x), (int)cfg->pl.y))
		cfg->pl.x = new_x;
	if (!hit_wall(cfg, (int)cfg->pl.x, (int)(new_y + pad_y)))
		cfg->pl.y = new_y;
}

static void	move_backward(t_cfg *cfg)
{
	double	new_x;
	double	new_y;
	double	pad_x;
	double	pad_y;

	new_x = cfg->pl.x - cos(cfg->pl.dir) * MOVE_SPEED;
	new_y = cfg->pl.y - sin(cfg->pl.dir) * MOVE_SPEED;
	pad_x = get_padding_sign(-cos(cfg->pl.dir), PADDING);
	pad_y = get_padding_sign(-sin(cfg->pl.dir), PADDING);
	if (!hit_wall(cfg, (int)(new_x + pad_x), (int)cfg->pl.y))
		cfg->pl.x = new_x;
	if (!hit_wall(cfg, (int)cfg->pl.x, (int)(new_y + pad_y)))
		cfg->pl.y = new_y;
}

static void	move_right(t_cfg *cfg)
{
	double	new_x;
	double	new_y;
	double	pad_x;
	double	pad_y;

	new_x = cfg->pl.x - sin(cfg->pl.dir) * MOVE_SPEED;
	new_y = cfg->pl.y + cos(cfg->pl.dir) * MOVE_SPEED;
	pad_x = get_padding_sign(-sin(cfg->pl.dir), PADDING);
	pad_y = get_padding_sign(cos(cfg->pl.dir), PADDING);
	if (!hit_wall(cfg, (int)(new_x + pad_x), (int)cfg->pl.y))
		cfg->pl.x = new_x;
	if (!hit_wall(cfg, (int)cfg->pl.x, (int)(new_y + pad_y)))
		cfg->pl.y = new_y;
}

static void	move_left(t_cfg *cfg)
{
	double	new_x;
	double	new_y;
	double	pad_x;
	double	pad_y;

	new_x = cfg->pl.x + sin(cfg->pl.dir) * MOVE_SPEED;
	new_y = cfg->pl.y - cos(cfg->pl.dir) * MOVE_SPEED;
	pad_x = get_padding_sign(sin(cfg->pl.dir), PADDING);
	pad_y = get_padding_sign(-cos(cfg->pl.dir), PADDING);
	if (!hit_wall(cfg, (int)(new_x + pad_x), (int)cfg->pl.y))
		cfg->pl.x = new_x;
	if (!hit_wall(cfg, (int)cfg->pl.x, (int)(new_y + pad_y)))
		cfg->pl.y = new_y;
}

void	move_player(t_cfg *cfg, int keycode)
{
	if (keycode == KEY_W)
		move_forward(cfg);
	else if (keycode == KEY_S)
		move_backward(cfg);
	else if (keycode == KEY_D)
		move_right(cfg);
	else if (keycode == KEY_A)
		move_left(cfg);
	else if (keycode == KEY_LEFT)
	{
		cfg->pl.dir -= ROT_SPEED;
		if (cfg->pl.dir < 0)
			cfg->pl.dir += 2 * M_PI;
	}
	else if (keycode == KEY_RIGHT)
	{
		cfg->pl.dir += ROT_SPEED;
		if (cfg->pl.dir >= 2 * M_PI)
			cfg->pl.dir -= 2 * M_PI;
	}
}
