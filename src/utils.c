/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:31:53 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/30 10:48:52 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_app_params(t_app *app)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		app->cfg.textures[i].pixels = NULL;
		i++;
	}
	app->cfg.tex_north = NULL;
	app->cfg.tex_south = NULL;
	app->cfg.tex_west = NULL;
	app->cfg.tex_east = NULL;
	app->mlx.screen.img = NULL;
	app->mlx.win = NULL;
	app->mlx.mlx = NULL;
	app->cfg.is_player = 0;
	app->cfg.floor_rgb = -1;
	app->cfg.ceil_rgb = -1;
}

int	get_rgb(char *s)
{
	int	r;
	int	g;
	int	b;

	r = -1;
	g = -1;
	b = -1;
	r = ft_atoi(s);
	while (*s && *s != ',')
		++s;
	if (*s == ',')
		++s;
	g = ft_atoi(s);
	while (*s && *s != ',')
		++s;
	if (*s == ',')
		++s;
	b = ft_atoi(s);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	return ((r << 16) | (g << 8) | b);
}

int	norm_player(t_cfg *cfg, int y, int x, char c)
{
	cfg->is_player = cfg->is_player + 1;
	cfg->pl.x = x + 0.5;
	cfg->pl.y = y + 0.5;
	if (c == 'N')
		cfg->pl.dir = -M_PI / 2.0;
	else if (c == 'S')
		cfg->pl.dir = M_PI / 2.0;
	else if (c == 'W')
		cfg->pl.dir = M_PI;
	else
		cfg->pl.dir = 0.0;
	return (0);
}

static void	trim_right(char *str)
{
	int	len;

	len = ft_strlen(str);
	while (len > 0 && (str[len - 1] == '\n' || \
		str[len - 1] == '\r' || str[len - 1] == ' '))
	{
		str[len - 1] = '\0';
		len--;
	}
}

void	parse_texture(char *line, char **dst)
{
	while (*line && *line != ' ')
		++line;
	while (*line == ' ')
		++line;
	trim_right(line);
	if (*dst)
		free(*dst);
	*dst = ft_strdup(line);
}
