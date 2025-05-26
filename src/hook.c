/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:32:10 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/26 17:06:33 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	close_hook(t_app *app)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (app->cfg.textures[i].pixels)
			free(app->cfg.textures[i].pixels);
		i++;
	}
	free(app->cfg.tex_north);
	free(app->cfg.tex_south);
	free(app->cfg.tex_west);
	free(app->cfg.tex_east);
	if (app->mlx.screen.img)
		mlx_destroy_image(app->mlx.mlx, app->mlx.screen.img);
	if (app->mlx.win)
		mlx_destroy_window(app->mlx.mlx, app->mlx.win);
	if (app->mlx.mlx)
	{
		mlx_destroy_display(app->mlx.mlx);
		free(app->mlx.mlx);
	}
	exit(0);
	return (0);
}

static double	get_padding_sign(double val, double padding)
{
	if (val > 0)
		return (padding);
	else
		return (-padding);
}

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

int	key_press(int keycode, t_app *app)
{
	if (keycode == KEY_ESC)
		close_hook(app);
	move_player(&app->cfg, keycode);
	return (0);
}
