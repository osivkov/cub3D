/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:27:15 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/27 16:33:05 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	fill_sky(t_app *app)
{
	int	y;
	int	x;
	int	*dst;

	y = 0;
	while (y < WIN_H / 2)
	{
		dst = app->mlx.screen.addr + y * WIN_W;
		x = 0;
		while (x < WIN_W)
		{
			dst[x] = app->cfg.ceil_rgb;
			x++;
		}
		y++;
	}
}

void	fill_floor(t_app *app)
{
	int	y;
	int	x;
	int	*dst;

	y = WIN_H / 2;
	while (y < WIN_H)
	{
		dst = app->mlx.screen.addr + y * WIN_W;
		x = 0;
		while (x < WIN_W)
		{
			dst[x] = app->cfg.floor_rgb;
			x++;
		}
		y++;
	}
}
