/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:17:29 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/23 18:32:18 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"
#include "libft.h"
#include <fcntl.h>
#include <string.h>

void	norm_player(t_cfg *cfg, int y, int x, char c)
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
/* Инициализация cfg: сбрасываем header и цвета */
int	init_cfg(t_cfg *cfg)
{
	cfg->floor_rgb = -1;
	cfg->ceil_rgb  = -1;
	cfg->tex_north = NULL;
	cfg->tex_south = NULL;
	cfg->tex_west  = NULL;
	cfg->tex_east  = NULL;
	return (0);
}

/* get_rgb: преобразует "R,G,B" в 0x00RRGGBB */
static int	get_rgb(char *s)
{
	int r;
	int g;
	int b;

	r = ft_atoi(s);
	while (*s && *s != ',')
		s++;
	if (*s == ',')
		s++;
	g = ft_atoi(s);
	while (*s && *s != ',')
		s++;
	if (*s == ',')
		s++;
	b = ft_atoi(s);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	return ((r << 16) | (g << 8) | b);
}

static void	trim_right(char *s)
{
	int len;

	len = ft_strlen(s);
	while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r' || s[len - 1] == ' '))
	{
		s[--len] = '\0';
	}
}
/* parse_texture: извлекает путь XPM в dst */
static void	parse_texture(char *line, char **dst)
{
	while (*line && *line != ' ')
		line++;
	while (*line == ' ')
		line++;
	trim_right(line);
	if (*dst)
		free(*dst);
	*dst = ft_strdup(line);
}


/* skip_spaces: возвращает указатель на первый ненулевой символ */
static char	*skip_spaces(char *s)
{
	while (*s == ' ' || *s == '\t')
		s++;
	return (s);
}

/* Проверка, начинается ли строка с одним из токенов и пробел */
/**
 * match_token:
 *   Смотрит первые два символа s и пробел после них.
 *   Возвращает 1 и записывает индекс директивы в *idx, если совпали.
 */
static int match_token(const char *s, int *idx)
{
	if (s[0] == 'N' && s[1] == 'O' && s[2] == ' ')
	{
		*idx = 0;
		return 1;
	}
	if (s[0] == 'S' && s[1] == 'O' && s[2] == ' ')
	{
		*idx = 1;
		return 1;
	}
	if (s[0] == 'W' && s[1] == 'E' && s[2] == ' ')
	{
		*idx = 2;
		return 1;
	}
	if (s[0] == 'E' && s[1] == 'A' && s[2] == ' ')
	{
		*idx = 3;
		return 1;
	}
	if (s[0] == 'F' && s[1] == ' ')
	{
		*idx = 4;
		return 1;
	}
	if (s[0] == 'C' && s[1] == ' ')
	{
		*idx = 5;
		return 1;
	}
	return 0;
}

/* parse_scene: объединяет parse_header + parse_map */
int	parse_scene(const char *file, t_cfg *cfg)
{
	int		fd;
	char	*orig;
	char	*p;
	int		seen[6];
	int		i;
	int		header_cnt;
	int		y;
	int		max_w;
	int		j;
	int		tok;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (1);

	/* сброс заголовков */
	i = 0;
	while (i < 6)
		seen[i++] = 0;
	header_cnt = 0;

	/* подготовка карты */
	y     = 0;
	max_w = 0;
	i     = 0;
	while (i < MAP_MAX)
	{
		j = 0;
		while (j < MAP_MAX)
		{
			cfg->map.grid[i][j] = ' ';
			j++;
		}
		i++;
	}

	/* читаем файл построчно */
	while ((orig = get_next_line(fd)))
	{
		/* trim и пропуск табов/пробелов */
		trim_right(orig);
		p = skip_spaces(orig);

		/* парсим заголовки */
		if (header_cnt < 6 && match_token(p, &tok))
		{
			if (seen[tok])
			{
				free(orig);
				close(fd);
				return (1);
			}
			seen[tok] = 1;
			header_cnt++;
			if (tok == 0)
				parse_texture(p + 2, &cfg->tex_north);
			else if (tok == 1)
				parse_texture(p + 2, &cfg->tex_south);
			else if (tok == 2)
				parse_texture(p + 2, &cfg->tex_west);
			else if (tok == 3)
				parse_texture(p + 2, &cfg->tex_east);
			else if (tok == 4)
				cfg->floor_rgb = get_rgb(p + 2);
			else /* tok == 5 */
				cfg->ceil_rgb  = get_rgb(p + 2);
			free(orig);
			continue;
		}

		/* если заголовки ещё не все, пропускаем строку */
		if (header_cnt < 6)
		{
			free(orig);
			continue;
		}

		/* копируем строку карты */
		j = 0;
		while (p[j] && j < MAP_MAX)
		{
			cfg->map.grid[y][j] = p[j];
			if (p[j] == 'N' || p[j] == 'S'
			 || p[j] == 'E' || p[j] == 'W')
				norm_player(cfg, y, j, p[j]);
			j++;
		}
		if (j > max_w)
			max_w = j;
		y++;

		free(orig);
	}

	close(fd);

	/* проверяем, что было ровно 6 заголовков */
	if (header_cnt != 6)
		return (1);

	cfg->map.w = max_w;
	cfg->map.h = y;
	return (0);
}
