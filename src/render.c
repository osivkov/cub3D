
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
	double 	wall_x;

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
		raycast(&app->cfg, x, &line_h, &wall_x);
		draw_start = (WIN_H - line_h) / 2;
		if (draw_start < 0)
			draw_start = 0;
		draw_end = draw_start + line_h;
		if (draw_end > WIN_H)
			draw_end = WIN_H;
		y = draw_start;
		t_tex *tex = &app->cfg.textures[app->cfg.tex_id];

		int tex_x = (int)(wall_x * (double)tex->width);
		if (tex_x < 0)
			tex_x = 0;
		if (tex_x >= tex->width)
			tex_x = tex->width - 1;

		for (int y = draw_start; y < draw_end; y++)
		{
			int d = y * 256 - WIN_H * 128 + line_h * 128;
			int tex_y = ((d * tex->height) / line_h) / 256;

			if (tex_y < 0) 
				tex_y = 0;
			if (tex_y >= tex->height)
				tex_y = tex->height - 1;

			unsigned int color = tex->pixels[tex_y * tex->width + tex_x];
			put_pixel(&app->mlx.screen, x, y, color);
		}
		++x;
	}
	mlx_put_image_to_window(app->mlx.mlx, app->mlx.win,
		app->mlx.screen.img, 0, 0);
	return (0);
}