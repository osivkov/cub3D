/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:19:32 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/28 16:52:16 by pkhvorov         ###   ########.fr       */
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
	clean_up(app);
	exit(0);
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
