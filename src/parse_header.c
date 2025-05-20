/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:16:42 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/20 17:35:00 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <fcntl.h>

int init_cfg(t_cfg *cfg)
{
	cfg->floor_rgb = -1;
	cfg->ceil_rgb = -1;
	cfg->tex_north = NULL;
	cfg->tex_south = NULL;
	cfg->tex_west = NULL;
	cfg->tex_east = NULL;
	return (0);
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

int	parse_header(char *file, t_cfg *cfg)
{
	int		fd;
	char	*line;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
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
		else if (line[0] == '1' || line[0] == '0'
			|| line[0] == 'N' || line[0] == 'S'
			|| line[0] == 'E' || line[0] == 'W')
		{
			free(line);
			break ;
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}

