/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 17:52:44 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/23 18:30:41 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
/* scan_bad_char: проверяет, что все символы в map.grid валидны */
static int	scan_bad_char(t_cfg *cfg)
{
	int	i;
	int	j;

	i = 0;
	while (i < cfg->map.h)
	{
		j = 0;
		while (j < cfg->map.w)
		{
			char c = cfg->map.grid[i][j];
			if (c != ' ' && c != '\t'
			 && c != '0' && c != '1'
			 && c != 'N' && c != 'S'
			 && c != 'E' && c != 'W')
			{
				ft_printf("Error: invalid character '%c' at (%d,%d)\n",
					  c, i, j);
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

/* scan_spawn_pos: находит ровно один спавн и возвращает его координаты */
static int	scan_spawn_pos(t_cfg *cfg, int *spy, int *spx)
{
	int	count;
	int	i;
	int	j;

	count = 0;
	i = 0;
	while (i < cfg->map.h)
	{
		j = 0;
		while (j < cfg->map.w)
		{
			char c = cfg->map.grid[i][j];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				*spy = i;
				*spx = j;
				count++;
			}
			j++;
		}
		i++;
	}
	if (count == 0)
		ft_putendl_fd("Error: no player start found", 2);
	else if (count > 1)
		ft_putendl_fd("Error: multiple player starts found", 2);
	return (count == 1);
}

/* find_spawn: объединяет обе проверки */
int	find_spawn(t_cfg *cfg, int *spy, int *spx)
{
	if (!scan_bad_char(cfg))
		return (0);
	return (scan_spawn_pos(cfg, spy, spx));
}

/* 5) Обёртка: возвращает 1, если карта замкнута, иначе 0 */
int	check_leak(t_cfg *cfg, int sy, int sx)
{
	t_bfs	*b;
	int		leak;

	b = bfs_init(cfg, sy, sx);
	leak = bfs_run(cfg, b);
	bfs_free(b);
	return (!leak);
}
