/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:16:42 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/13 10:32:27 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <fcntl.h>
#include <stdlib.h>

/*
** Преобразуем строку "R,G,B"  в 0x00RRGGBB.
*/
static int	get_rgb(char *s)
{
	int	r;
	int	g;
	int	b;

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
	return ((r << 16) | (g << 8) | b);
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
		/* ------------ распознаём две нужные строки ------------ */
		if (line[0] == 'F' && line[1] == ' ')
			cfg->floor_rgb = get_rgb(line + 2);
		else if (line[0] == 'C' && line[1] == ' ')
			cfg->ceil_rgb = get_rgb(line + 2);

		/* ------------ карта началась? — остановимся ------------ */
		if (line[0] == '1' || line[0] == '0'
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