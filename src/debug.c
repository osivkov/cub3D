/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:04:34 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/28 16:07:12 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	print_map(const t_map *map)
{
	int	y;
	int	x;

	if (!map)
	{
		printf("map is NULL\n");
		return ;
	}
	printf("=== MAP ==========\n");
	y = 0;
	while (y < map->h)
	{
		x = 0;
		while (x < map->w)
		{
			putchar(map->grid[y][x]);
			x++;
		}
		putchar('\n');
		y++;
	}
}

void	print_cfg(const t_cfg *cfg)
{
	if (!cfg)
	{
		printf("cfg is NULL\n");
		return ;
	}
	printf("=== CFG STRUCT ===\n");
	printf("is_player : %d\n", cfg->is_player);
	printf("ceil_rgb  : 0x%06X (R:%d G:%d B:%d)\n",
		cfg->ceil_rgb,
		(cfg->ceil_rgb >> 16) & 0xFF,
		(cfg->ceil_rgb >> 8) & 0xFF,
		cfg->ceil_rgb & 0xFF);
	printf("floor_rgb : 0x%06X (R:%d G:%d B:%d)\n",
		cfg->floor_rgb,
		(cfg->floor_rgb >> 16) & 0xFF,
		(cfg->floor_rgb >> 8) & 0xFF,
		cfg->floor_rgb & 0xFF);
}
