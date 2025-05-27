/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 16:24:37 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/27 16:27:37 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	put_pixel(t_img *img, int x, int y, int color)
{
	int	*dst;

	dst = img->addr + y * img->line_len / 4 + x;
	*dst = color;
}

static unsigned int	apply_intensity(unsigned int color, double intensity)
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;

	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;
	r = (int)(r * intensity);
	g = (int)(g * intensity);
	b = (int)(b * intensity);
	return ((r << 16) | (g << 8) | b);
}

static void	draw_column(t_app *app, t_frame_vars *vars)
{
	t_tex	*tex;

	tex = &app->cfg.textures[app->cfg.tex_id];
	vars->tex_x = (int)(vars->wall_x * (double)tex->width);
	if ((app->cfg.tex_id == TEX_WE) || (app->cfg.tex_id == TEX_SO))
		vars->tex_x = tex->width - vars->tex_x - 1;
	if (vars->tex_x < 0)
		vars->tex_x = 0;
	if (vars->tex_x >= tex->width)
		vars->tex_x = tex->width - 1;
	vars->y = vars->draw_start;
	while (vars->y < vars->draw_end)
	{
		vars->d = vars->y * 256 - WIN_H * 128 + vars->line_h * 128;
		vars->tex_y = ((vars->d * tex->height) / vars->line_h) / 256;
		if (vars->tex_y < 0)
			vars->tex_y = 0;
		if (vars->tex_y >= tex->height)
			vars->tex_y = tex->height - 1;
		vars->color = tex->pixels[vars->tex_y * tex->width + vars->tex_x];
		vars->color = apply_intensity(vars->color, vars->intensity);
		put_pixel(&app->mlx.screen, vars->x, vars->y, vars->color);
		vars->y++;
	}
}

static void	render_frame(t_app *app)
{
	t_frame_vars	vars;

	vars.x = 0;
	while (vars.x < WIN_W)
	{
		vars.dist = raycast(&app->cfg, vars.x, &vars.line_h, &vars.wall_x);
		vars.intensity = 1.0 / (1.0 + vars.dist * vars.dist * 0.1);
		vars.draw_start = (WIN_H - vars.line_h) / 2;
		if (vars.draw_start < 0)
			vars.draw_start = 0;
		vars.draw_end = vars.draw_start + vars.line_h;
		if (vars.draw_end > WIN_H)
			vars.draw_end = WIN_H;
		draw_column(app, &vars);
		vars.x++;
	}
}

int	frame(void *param)
{
	t_app	*app;

	app = (t_app *)param;
	fill_sky(app);
	fill_floor(app);
	render_frame(app);
	mlx_put_image_to_window(app->mlx.mlx, app->mlx.win, \
		app->mlx.screen.img, 0, 0);
	return (0);
}
