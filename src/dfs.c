/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:31:53 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/26 16:47:53 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool is_walkable(char c)
{
	return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

bool is_space(char c)
{
	return (c == ' ' || c == '\n');
}

bool	dfs_check(t_map *map, bool visited[MAP_MAX][MAP_MAX], int y, int x)
{
	if (y < 0 || y >= map->h || x < 0 || x >= map->w)
		return (false); // Выход за пределы — дырка!

	if (visited[y][x])
		return (true); // Уже были — всё ок

	char c = map->grid[y][x];

	if (c == '1') // Стена — остановка
		return (true);

	if (is_space(c) || !is_walkable(c))
		return (false);

	visited[y][x] = true;

	// Рекурсивный обход в 4 стороны
	return (
		dfs_check(map, visited, y + 1, x) &&
		dfs_check(map, visited, y - 1, x) &&
		dfs_check(map, visited, y, x + 1) &&
		dfs_check(map, visited, y, x - 1)
	);
}

bool	validate_map_closed(t_map *map)
{
	bool	visited[MAP_MAX][MAP_MAX] = {{0}};
	int		y, x;

	// найти игрока
	y = 0;
	while (y < map->h)
	{
		x = 0;
		while (x < map->w)
		{
			char c = map->grid[y][x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
				return dfs_check(map, visited, y, x);
			++x;
		}
		++y;
	}

	return (false); // нет игрока — ошибка
}