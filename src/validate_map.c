/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:17:40 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/23 18:29:46 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"
#include <stdlib.h>

#define MAX_MAP MAP_MAX
/* найти спавн и проверить символы */
t_bfs	*bfs_init(t_cfg *cfg, int sy, int sx)
{
	t_bfs	*b;
	int		total;
	int		i;

	b = malloc(sizeof(*b));
	b->H = cfg->map.h;
	b->W = cfg->map.w;
	total = b->H * b->W;
	b->queue = malloc(sizeof(*b->queue) * total);
	b->vis = malloc(sizeof(*b->vis) * b->H);
	i = 0;
	while (i < b->H)
	{
		b->vis[i] = calloc(b->W, sizeof(int));
		i++;
	}
	b->head = 0;
	b->tail = 0;
	b->vis[sy][sx] = 1;
	b->queue[b->tail++] = (t_pt){sy, sx};
	return (b);
}

/* 2) Раскрытие одной точки p: возвращает 1 при утечке */
/* 1) Обработка одного направления; возвращает 1 при утечке */
static int	bfs_process_dir(t_cfg *cfg,
				t_bfs *b,
				t_pt p,
				int dir)
{
	const int	dy[4] = {-1, 1, 0, 0};
	const int	dx[4] = {0, 0, -1, 1};
	int			ny;
	int			nx;

	ny = p.y + dy[dir];
	nx = p.x + dx[dir];
	if (ny < 0 || nx < 0 || ny >= b->H || nx >= b->W)
		return (1);
	if (cfg->map.grid[ny][nx] == ' ')
		return (1);
	if (cfg->map.grid[ny][nx] != '1' && !b->vis[ny][nx])
	{
		b->vis[ny][nx] = 1;
		b->queue[b->tail++] = (t_pt){ny, nx};
	}
	return (0);
}

/* 2) Один шаг BFS: итерируем 4 направления */
int	bfs_step(t_cfg *cfg, t_bfs *b)
{
	t_pt	p;
	int		k;
	int		leak;

	p = b->queue[b->head++];
	k = 0;
	leak = 0;
	while (k < 4 && !leak)
	{
		leak = bfs_process_dir(cfg, b, p, k);
		k++;
	}
	return (leak);
}

/* 3) Основной цикл BFS: возвращает 1 при утечке, иначе 0 */
int	bfs_run(t_cfg *cfg, t_bfs *b)
{
	int	leak;

	leak = 0;
	while (b->head < b->tail && !leak)
		leak = bfs_step(cfg, b);
	return (leak);
}

/* 4) Освобождение всех ресурсов, выделенных для BFS */
void	bfs_free(t_bfs *b)
{
	int	i;

	i = 0;
	while (i < b->H)
	{
		free(b->vis[i]);
		i++;
	}
	free(b->vis);
	free(b->queue);
	free(b);
}
