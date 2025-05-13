/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:11:30 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/13 13:04:31 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/* ────────── внешние инклюды ────────── */
# include "mlx.h"          /* MiniLibX API */
# include <stdlib.h>       /* malloc/free, exit */
#include "libft.h"
# include <unistd.h>       /* write */

/* ────────── размеры окна ────────── */
# define WIN_W 640
# define WIN_H 480

/* ────────── базовый off-screen буфер ────────── */
typedef struct s_img
{
	void	*img;	/* указатель MLX на картинку */
	int		*addr;	/* raw-массив пикселей (int per pixel) */
	int		bpp;	/* bits per pixel (32)   */
	int		line_len;	/* байт в строке         */
	int		endian;	/* порядок байтов mac/pc */
	int		w;
	int		h;
}	t_img;

/* ────────── miniLibX связка ────────── */
typedef struct s_mlx
{
	void	*mlx;/* контекст MLX */
	void	*win;/* окно */
	t_img	screen;/* off-screen буфер */
}	t_mlx;

/* ────────── 2-D карта ────────── */
# define MAP_MAX 128

typedef struct s_map
{
	char	grid[MAP_MAX][MAP_MAX]; /* символы карты */
	int		w;/* фактическая ширина */
	int		h;/* фактическая высота */
}	t_map;

/* ────────── положение игрока ────────── */
typedef struct s_player
{
	double	x;/* координата по X (float) */
	double	y;/* координата по Y */
	double	dir;/* угол направления (радианы, 0 = восток) */
}	t_player;

/* ────────── конфиг сцены ────────── */
typedef struct s_cfg
{
	int			ceil_rgb;/* цвет потолка 0xRRGGBB */
	int			floor_rgb;/* цвет пола    0xRRGGBB */
	t_map		map;/* лабиринт*/
	t_player	pl;/* игрок*/
}	t_cfg;

/* ────────── единая «app» структура ────────── */
typedef struct s_app
{
	t_mlx	mlx; /* графика */
	t_cfg	cfg; /* данные  */
}	t_app;

/* ────────── прототипы ────────── */
int		parse_header(char *file, t_cfg *cfg);
int		parse_map(char *file, t_cfg *cfg);
int		frame(void *param);
int		close_hook(void *param);

#endif
