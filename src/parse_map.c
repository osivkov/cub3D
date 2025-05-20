/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:24:45 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/20 17:35:15 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libft.h"
#include <fcntl.h>

/* кладём игрока в cfg->pl и сразу задаём угол dir */
static void	norm_player(t_cfg *cfg, int y, int x, char c)
{
	cfg->pl.x = x + 0.5;
	cfg->pl.y = y + 0.5;
	if (c == 'N')
		cfg->pl.dir = 3.1415926535 / 2.0;  /* π/2   */
	else if (c == 'S')
		cfg->pl.dir = -3.1415926535 / 2.0;
	else if (c == 'E')
		cfg->pl.dir = 0.0;
	else
		cfg->pl.dir = 3.1415926535;   /* π */
}

/*
** parse_map():
**   После заголовков остаётся только карта.
**   Переносим в grid[y][x]; заполняем размеры w/h.
*/

int	parse_map(char *file, t_cfg *cfg)
{
	int		fd;
	char	*line;
	int		y;
	int		max_w;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (1);
	y = 0;
	max_w = 0;
	line = get_next_line(fd);
	while (line)
	{
		/* карта начинается, если первый символ — 1/0/N/S/E/W */
		if (line[0] == '1' || line[0] == '0'
			|| line[0] == 'N' || line[0] == 'S'
			|| line[0] == 'E' || line[0] == 'W')
		{
			int	x;

			x = 0;
			while (line[x] && x < MAP_MAX)
			{
				cfg->map.grid[y][x] = line[x];
				if (line[x] == 'N' || line[x] == 'S'
					|| line[x] == 'E' || line[x] == 'W')
					norm_player(cfg, y, x, line[x]);
				++x;
			}
			if (x > max_w)
				max_w = x;
			++y;
		}
		free(line);
		line = get_next_line(fd);
	}
	cfg->map.h = y;
	cfg->map.w = max_w;
	close(fd);
	return (0);
}
