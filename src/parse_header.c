/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:16:42 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/26 17:00:40 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <fcntl.h>

int init_cfg(t_cfg *cfg)
{
	cfg->is_player = 0;
	cfg->floor_rgb = -1;
	cfg->ceil_rgb = -1;
	cfg->tex_north = NULL;
	cfg->tex_south = NULL;
	cfg->tex_west = NULL;
	cfg->tex_east = NULL;
	return (0);
}

static void	norm_player(t_cfg *cfg, int y, int x, char c)
{
	cfg->is_player = 1;
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
}

/*
** Преобразуем строку "R,G,B"  в 0x00RRGGBB.
*/

static int	get_rgb(char *s)
{
	int	r = -1;
	int	g = -1;
	int	b = -1;

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

static void trim_right(char *str)
{
    int len = ft_strlen(str);
    while (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r' || str[len-1] == ' '))
    {
        str[len-1] = '\0';
        len--;
    }
}

static void parse_texture(char *line, char **dst)
{
    while (*line && *line != ' ')
        ++line;
    while (*line == ' ')
        ++line;
    trim_right(line);  // Убираем символы перевода строки и пробелы справа
    if (*dst)
        free(*dst);
    *dst = ft_strdup(line);
}
/*
** Читаем только заголовочные строки (F / C),
** останавливаемся на первой строке карты ('1','0','N','S','E','W').
*/

int	has_cub_extension(const char *filename)
{
	size_t	len = ft_strlen(filename);

	if (len < 4)
		return (0); // слишком короткое имя
	return (ft_strncmp(filename + len - 4, ".cub", 4) == 0);
}

// int	parse_header(char *file, t_cfg *cfg)
// {
// 	int		fd;
// 	char	*line;
// 	int		y = 0;
// 	int		max_w = 0;

// 	if (!has_cub_extension(file))
// 		return (write(2, "Error: not .cub file \n", 22), 1);

// 	fd = open(file, O_RDONLY);
// 	if (fd < 0)
// 		return (1);
// 	line = get_next_line(fd);
// 	while (line)
// 	{
// 		if (ft_strncmp(line, "F ", 2) == 0)
// 			cfg->floor_rgb = get_rgb(line + 2);
// 		else if (ft_strncmp(line, "C ", 2) == 0)
// 			cfg->ceil_rgb = get_rgb(line + 2);
// 		else if (ft_strncmp(line, "NO ", 3) == 0)
// 			parse_texture(line + 2, &cfg->tex_north);
// 		else if (ft_strncmp(line, "SO ", 3) == 0)
// 			parse_texture(line + 2, &cfg->tex_south);
// 		else if (ft_strncmp(line, "WE ", 3) == 0)
// 			parse_texture(line + 2, &cfg->tex_west);
// 		else if (ft_strncmp(line, "EA ", 3) == 0)
// 			parse_texture(line + 2, &cfg->tex_east);
// 		else if (ft_strchr(line,'1') || ft_strchr(line,'0'))
// 		{
// 			int	x;

// 			x = 0;
// 			while (line[x] && x < MAP_MAX)
// 			{
// 				cfg->map.grid[y][x] = line[x];
// 				if (line[x] == 'N' || line[x] == 'S'
// 					|| line[x] == 'E' || line[x] == 'W')
// 					norm_player(cfg, y, x, line[x]);
// 				++x;
// 			}
// 			if (x > max_w)
// 				max_w = x;
// 			++y;
// 		}
// 		free(line);
// 		line = get_next_line(fd);
// 	}
// 	cfg->map.h = y;
// 	cfg->map.w = max_w;
// 	if (cfg->is_player == 0)
// 		return (1);
// 	close(fd);
// 	return (0);
// }

int	open_cub_file(const char *file)
{
	int	fd;

	if (!has_cub_extension(file))
	{
		write(2, "Error: not .cub file \n", 22);
		return (-1);
	}
	fd = open(file, O_RDONLY);
	if (fd < 0)
		perror("open");
	return (fd);
}


void	parse_directive(char *line, t_cfg *cfg)
{
	if (ft_strncmp(line, "F ", 2) == 0)
		cfg->floor_rgb = get_rgb(line + 2);
	else if (ft_strncmp(line, "C ", 2) == 0)
		cfg->ceil_rgb = get_rgb(line + 2);
	else if (ft_strncmp(line, "NO ", 3) == 0)
		parse_texture(line + 2, &cfg->tex_north);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		parse_texture(line + 2, &cfg->tex_south);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		parse_texture(line + 2, &cfg->tex_west);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		parse_texture(line + 2, &cfg->tex_east);
}


// int	parse_map_line(char *line, t_cfg *cfg, int y, int *max_w)
// {
// 	int	x = 0;

// 	while (line[x] && x < MAP_MAX)
// 	{
// 		cfg->map.grid[y][x] = line[x];
// 		if (line[x] == 'N' || line[x] == 'S'
// 			|| line[x] == 'E' || line[x] == 'W')
// 			norm_player(cfg, y, x, line[x]);
// 		++x;
// 	}
// 	if (x > *max_w)
// 		*max_w = x;
// 	return (0);
// }

void	parse_map_line(char *line, t_cfg *cfg, int y, int *max_w)
{
	int x = 0;

	// Обнуляем строку перед копированием
	while (x < MAP_MAX)
		cfg->map.grid[y][x++] = '\0';

	x = 0;
	while (line[x] && line[x] != '\n' && x < MAP_MAX)
	{
		cfg->map.grid[y][x] = line[x];
		if (line[x] == 'N' || line[x] == 'S' ||
			line[x] == 'E' || line[x] == 'W')
			norm_player(cfg, y, x, line[x]);
		x++;
	}

	if (x > *max_w)
		*max_w = x;
}

void	fill_map_gaps(t_map *map)
{
	int	y = 0;

	while (y < map->h)
	{
		int	x = 0;
		while (x < map->w)
		{
			if (map->grid[y][x] == '\0')
				map->grid[y][x] = ' ';
			x++;
		}
		y++;
	}
}

int	parse_header(char *file, t_cfg *cfg)
{
	int		fd, y = 0, max_w = 0;
	char	*line;

	fd = open_cub_file(file);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0 ||
			ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0 ||
			ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0)
			parse_directive(line, cfg);
		else if (ft_strchr(line, '1') || ft_strchr(line, '0'))
			parse_map_line(line, cfg, y++, &max_w);
		free(line);
		line = get_next_line(fd);
	}
	cfg->map.h = y;
	cfg->map.w = max_w;
	fill_map_gaps(&cfg->map);
	close(fd);
	return (cfg->is_player == 0);
}
