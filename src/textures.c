/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:28:05 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/28 16:10:36 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	copy_texture_pixels(t_tex *tex, int *data, int size_line)
{
	int	y;
	int	x;
	int	color;

	y = 0;
	while (y < tex->height)
	{
		x = 0;
		while (x < tex->width)
		{
			color = data[y * (size_line / 4) + x];
			tex->pixels[y * tex->width + x] = (unsigned int)color;
			x++;
		}
		y++;
	}
}

static int	load_xpm_texture(void *mlx, t_tex *tex, char *path)
{
	void	*img;
	int		*data;
	int		bpp;
	int		size_line;
	int		endian;

	img = mlx_xpm_file_to_image(mlx, path, &tex->width, &tex->height);
	if (!img)
		return (1);
	data = (int *)mlx_get_data_addr(img, &bpp, &size_line, &endian);
	if (!data)
	{
		mlx_destroy_image(mlx, img);
		return (1);
	}
	tex->pixels = malloc(sizeof(unsigned int) * tex->width * tex->height);
	if (!tex->pixels)
	{
		mlx_destroy_image(mlx, img);
		return (1);
	}
	copy_texture_pixels(tex, data, size_line);
	mlx_destroy_image(mlx, img);
	return (0);
}

static int	init_textures_def(t_cfg *cfg)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		cfg->textures[i].img = NULL;
		cfg->textures[i].width = 0;
		cfg->textures[i].height = 0;
		cfg->textures[i].pixels = NULL;
		i++;
	}
	return (0);
}

static int	has_xpm_extension(const char *filename)
{
	size_t	len;

	len = ft_strlen(filename);
	if (len < 4)
		return (0);
	return (ft_strncmp(filename + len - 4, ".xpm", 4) == 0);
}

int	init_textures(t_app *app)
{
	init_textures_def(&app->cfg);
	if (!has_xpm_extension(app->cfg.tex_north) || \
		load_xpm_texture(app->mlx.mlx, &app->cfg.textures[TEX_NO], \
		app->cfg.tex_north))
		return (1);
	if (!has_xpm_extension(app->cfg.tex_south) || \
		load_xpm_texture(app->mlx.mlx, &app->cfg.textures[TEX_SO], \
		app->cfg.tex_south))
		return (1);
	if (!has_xpm_extension(app->cfg.tex_west) || \
		load_xpm_texture(app->mlx.mlx, &app->cfg.textures[TEX_WE], \
		app->cfg.tex_west))
		return (1);
	if (!has_xpm_extension(app->cfg.tex_east) || \
		load_xpm_texture(app->mlx.mlx, &app->cfg.textures[TEX_EA], \
		app->cfg.tex_east))
		return (1);
	return (0);
}
