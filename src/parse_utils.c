/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:55:30 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/30 10:49:27 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_directive(char *line, t_cfg *cfg)
{
	if (ft_strncmp(line, "F ", 2) == 0)
	{
		cfg->floor_rgb = get_rgb(line + 2);
		if (cfg->floor_rgb == -1)
			return (1);
	}
	else if (ft_strncmp(line, "C ", 2) == 0)
	{
		cfg->ceil_rgb = get_rgb(line + 2);
		if (cfg->ceil_rgb == -1)
			return (1);
	}
	else if (ft_strncmp(line, "NO ", 3) == 0)
		parse_texture(line + 2, &cfg->tex_north);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		parse_texture(line + 2, &cfg->tex_south);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		parse_texture(line + 2, &cfg->tex_west);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		parse_texture(line + 2, &cfg->tex_east);
	return (0);
}

int	parse_map_line(char *line, t_cfg *cfg, int y, int *max_w)
{
	int	x;

	x = 0;
	while (x < MAP_MAX)
		cfg->map.grid[y][x++] = '\0';
	x = 0;
	while (line[x] && line[x] != '\n' && x < MAP_MAX)
	{
		cfg->map.grid[y][x] = line[x];
		if (line[x] == 'N' || line[x] == 'S' || \
			line[x] == 'E' || line[x] == 'W')
				norm_player(cfg, y, x, line[x]);
		x++;
	}
	if (x > *max_w)
		*max_w = x;
	return (0);
}

void	fill_map_gaps(t_map *map)
{
	int	y;
	int	x;

	y = 0;
	while (y < map->h)
	{
		x = 0;
		while (x < map->w)
		{
			if (map->grid[y][x] == '\0')
				map->grid[y][x] = ' ';
			x++;
		}
		y++;
	}
}
