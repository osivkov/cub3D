/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:31:53 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/27 12:18:39 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	is_walkable(char c)
{
	return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static bool	is_space(char c)
{
	return (c == ' ' || c == '\n');
}

static bool	dfs_check(t_map *map, bool visited[MAP_MAX][MAP_MAX], int y, int x)
{
	char	c;

	if (y < 0 || y >= map->h || x < 0 || x >= map->w)
		return (false);
	if (visited[y][x])
		return (true);
	c = map->grid[y][x];
	if (c == '1')
		return (true);
	if (is_space(c) || !is_walkable(c))
		return (false);
	visited[y][x] = true;
	return (dfs_check(map, visited, y + 1, x) && \
		dfs_check(map, visited, y - 1, x) && \
		dfs_check(map, visited, y, x + 1) && \
		dfs_check(map, visited, y, x - 1)
	);
}

static void	init_visited(bool visited[MAP_MAX][MAP_MAX])
{
	int	y;
	int	x;

	y = 0;
	while (y < MAP_MAX)
	{
		x = 0;
		while (x < MAP_MAX)
		{
			visited[y][x] = false;
			x++;
		}
		y++;
	}
}

bool	validate_map_closed(t_map *map)
{
	bool	visited[MAP_MAX][MAP_MAX];
	int		y;
	int		x;
	char	c;

	init_visited(visited);
	y = 0;
	while (y < map->h)
	{
		x = 0;
		while (x < map->w)
		{
			c = map->grid[y][x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
				return (dfs_check(map, visited, y, x));
			++x;
		}
		++y;
	}
	return (false);
}
