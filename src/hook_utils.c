/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:19:32 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/27 16:08:00 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

double	get_padding_sign(double val, double padding)
{
	if (val > 0)
		return (padding);
	else
		return (-padding);
}

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

int	key_press(int keycode, t_app *app)
{
	if (keycode == KEY_ESC)
		close_hook(app);
	move_player(&app->cfg, keycode);
	return (0);
}

int	hit_wall(t_cfg *cfg, int mx, int my)
{
	if (mx < 0 || my < 0 || my >= cfg->map.h || mx >= cfg->map.w)
		return (1);
	if (cfg->map.grid[my][mx] == '1')
		return (1);
	return (0);
}
