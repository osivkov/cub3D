/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:11:30 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/26 15:49:19 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/* ────────── внешние инклюды ────────── */
# include "mlx.h"          /* MiniLibX API */
# include <stdlib.h>       /* malloc/free, exit */
# include "libft.h"
# include <unistd.h>       /* write */
# include <math.h>
# include <stdbool.h>
/* ────────── размеры окна ────────── */
# define WIN_W 1280
# define WIN_H 960

# define MOVE_SPEED 0.1
# define ROT_SPEED 0.05
# define PADDING 0.1

# define KEY_W     119
# define KEY_A     97
# define KEY_S     115
# define KEY_D     100
# define KEY_LEFT  65361
# define KEY_RIGHT 65363
# define KEY_ESC   65307

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
# define MAP_MAX 256

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

enum e_tex_dir
{
	TEX_NO,
	TEX_SO,
	TEX_WE,
	TEX_EA
};

typedef struct s_tex
{
    void            *img;    // чтобы сохранить изображение
    int             width;
    int             height;
    unsigned int    *pixels;
} t_tex;

/* ────────── конфиг сцены ────────── */
typedef struct s_cfg
{
	int			is_player;
	int			ceil_rgb;/* цвет потолка 0xRRGGBB */
	int			floor_rgb;/* цвет пола    0xRRGGBB */
	int 		tex_id;
	char		*tex_north;
	char		*tex_south;
	char		*tex_west;
	char		*tex_east;
	t_tex		textures[4];
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
int		init_cfg(t_cfg *cfg);
int		init_textures(t_app *app);
int		parse_header(char *file, t_cfg *cfg);
int		parse_map(char *file, t_cfg *cfg);
bool	validate_map_closed(t_map *map);
double	raycast(t_cfg *cfg, int col, int *line_h, double *wall_x);
int		frame(void *param);
int		close_hook(t_app *app);
int		hit_wall(t_cfg *cfg, int mx, int my);
int		key_press(int keycode, t_app *app);

#endif
