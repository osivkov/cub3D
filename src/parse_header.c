/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:16:42 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/27 15:00:50 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <fcntl.h>

static int	has_cub_extension(const char *filename)
{
	size_t	len;

	len = ft_strlen(filename);
	if (len < 4)
		return (0);
	return (ft_strncmp(filename + len - 4, ".cub", 4) == 0);
}

static int	open_cub_file(const char *file)
{
	int	fd;

	if (!has_cub_extension(file))
	{
		write(2, "Error: not .cub file \n", 22);
		return (-1);
	}
	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (-1);
	return (fd);
}

static void	fill_map_gaps(t_map *map)
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

static int	read_map_lines(int fd, t_cfg *cfg, int *max_w)
{
	char	*line;
	int		y;

	y = 0;
	line = get_next_line(fd);
	while (line)
	{
		if ((ft_strncmp(line, "F ", 2) == 0 || \
			ft_strncmp(line, "C ", 2) == 0 || \
			ft_strncmp(line, "NO ", 3) == 0 || \
			ft_strncmp(line, "SO ", 3) == 0 || \
			ft_strncmp(line, "WE ", 3) == 0 || \
			ft_strncmp(line, "EA ", 3) == 0) && \
			parse_directive(line, cfg))
		{
			free(line);
			return (1);
		}
		if (ft_strchr(line, '1') || ft_strchr(line, '0'))
			parse_map_line(line, cfg, y++, max_w);
		free(line);
		line = get_next_line(fd);
	}
	cfg->map.h = y;
	return (0);
}

int	parse_header(char *file, t_cfg *cfg)
{
	int	fd;
	int	max_w;
	int	err;

	fd = 0;
	max_w = 0;
	err = 0;
	fd = open_cub_file(file);
	if (fd < 0)
		return (1);
	err = read_map_lines(fd, cfg, &max_w);
	close(fd);
	if (err)
		return (1);
	cfg->map.w = max_w;
	fill_map_gaps(&cfg->map);
	return (cfg->is_player == 0);
}
