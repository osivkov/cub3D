
#include "cub3d.h"

static void	put_pixel(t_img *img, int x, int y, int color)
{
	int	*dst;

	dst = img->addr + y * img->line_len / 4 + x;
	*dst = color;
}

int	frame(void *param)
{
	t_app	*app;
	int		x;
	int		y;
	int		line_h;
	int		draw_start;
	int		draw_end;

	app = (t_app *)param;
	y = 0;
	while (y < WIN_H / 2)
	{
		int *dst = app->mlx.screen.addr + y * WIN_W;
		x = 0;
		while (x < WIN_W)
		{
			dst[x] = app->cfg.ceil_rgb;
			++x;
		}
		++y;
	}
	while (y < WIN_H)
	{
		int *dst = app->mlx.screen.addr + y * WIN_W;
		x = 0;
		while (x < WIN_W)
		{
			dst[x] = app->cfg.floor_rgb;
			++x;
		}
		++y;
	}
	x = 0;
	while (x < WIN_W)
	{
		raycast(&app->cfg, x, &line_h);
		draw_start = (WIN_H - line_h) / 2;
		if (draw_start < 0)
			draw_start = 0;
		draw_end = draw_start + line_h;
		if (draw_end > WIN_H)
			draw_end = WIN_H;
		y = draw_start;
		while (y < draw_end)
		{
			put_pixel(&app->mlx.screen, x, y, 0x7F7F7F);
			++y;
		}
		++x;
	}
	mlx_put_image_to_window(app->mlx.mlx, app->mlx.win,
		app->mlx.screen.img, 0, 0);
	return (0);
}