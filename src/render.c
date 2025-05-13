/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:48:36 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/13 13:00:27 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"

static void	put_pixel(t_img *img, int x, int y, int color)
{
	int	*dst;
	/* переходим к адресу пикселя: (y строк) × (байт в строке / 4 байта) + x */
	dst = img->addr + y * img->line_len / 4 + x;
	*dst = color; /* пишем 32-битный цвет в буфер */
}

int	frame(void *param)
{
	t_app	*app;
	int		x;
	int		y;

	app = (t_app *)param;
	/* двойной цикл заполняет каждую строку окна */
	y = 0;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			/* верхняя половина ⇒ цвет потолка, нижняя ⇒ цвет пола */
			if (y < WIN_H / 2)
				put_pixel(&app->mlx.screen, x, y, app->cfg.ceil_rgb);
			else
				put_pixel(&app->mlx.screen, x, y, app->cfg.floor_rgb);
			++x;
		}
		++y;
	}
	/* выводим готовый буфер в окно одним вызовом (быстрее, чем пиксель-за-пикселем) */
	mlx_put_image_to_window(app->mlx.mlx, app->mlx.win,
		app->mlx.screen.img, 0, 0);
	return (0); /* MLX игнорирует, но должен быть int-return */
}