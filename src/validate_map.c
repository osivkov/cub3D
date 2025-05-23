/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:17:40 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/23 17:05:02 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "cub3d.h"
#include "libft.h"
#include <stdlib.h>

#define MAX_MAP MAP_MAX

/* найти спавн и проверить символы */
static int	find_spawn(t_cfg *cfg, int *spy, int *spx)
{
	int i = 0;
	int hg = cfg->map.h;
	int wg = cfg->map.w;
	int spawn = 0;
	char bad = '\0';
	int by = -1, bx = -1;
	while (i < hg)
	{
		int j = 0;
		while (j < wg)
		{
			char c = cfg->map.grid[i][j];
			if (c != ' ' && c != '\t')
			{
				if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
				{
					spawn++;
					*spy = i;
					*spx = j;
				}
				else if (c != '0' && c != '1')
				{
					bad = c;
					by = i;
					bx = j;
				}
			}
			j++;
		}
		i++;
	}
	if (bad)
	{
		ft_printf("Error: invalid character '%c' at (%d,%d)\n", bad, by, bx);
		return 0;
	}
	if (spawn == 0)
	{
		ft_putendl_fd("Error: no player start found", 2);
		return 0;
	}
	if (spawn > 1)
	{
		ft_putendl_fd("Error: multiple player starts found", 2);
		return 0;
	}
	return 1;
}

/* BFS выявляет утечку */
static int	check_leak(t_cfg *cfg, int sy, int sx)
{
	int H = cfg->map.h;
	int W = cfg->map.w;
	int total = H * W;
	int head = 0, tail = 0;
	t_pt *queue = malloc(sizeof(*queue) * total);
	int **vis = malloc(sizeof(*vis) * H);
	int i = 0;
	while (i < H)
	{
		vis[i] = calloc(W, sizeof(int));
		i++;
	}
	vis[sy][sx] = 1;
	queue[tail++] = (t_pt){sy, sx};
	const int dy[4] = {-1,1,0,0};
	const int dx[4] = {0,0,-1,1};
	int leak = 0;
	while (head < tail && !leak)
	{
		t_pt p = queue[head++];
		int k = 0;
		while (k < 4)
		{
			int ny = p.y + dy[k];
			int nx = p.x + dx[k];
			if (ny < 0 || nx < 0 || ny >= H || nx >= W || cfg->map.grid[ny][nx] == ' ')
			{
				leak = 1;
				break;
			}
			if (!vis[ny][nx])
			{
				char c = cfg->map.grid[ny][nx];
				if (c == '0' || c=='N' || c=='S' || c=='E' || c=='W')
				{
					vis[ny][nx] = 1;
					queue[tail++] = (t_pt){ny, nx};
				}
			}
			k++;
		}
	}
	i = 0;
	while (i < H)
	{
		free(vis[i]);
		i++;
	}
	free(vis);
	free(queue);
	return !leak;
}


/* validate_map: главная */
int	validate_map(t_cfg *cfg)
{
	int	sy;
	int	sx;

	if (!find_spawn(cfg, &sy, &sx))
		return (0);
	if (!check_leak(cfg, sy, sx))
	{
		ft_putendl_fd("Error: map is not closed by walls", 2);
		return (0);
	}
	return (1);
}

