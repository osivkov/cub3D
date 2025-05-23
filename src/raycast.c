#include "cub3d.h"

/* вспомогательная проверка: в пределах карты и стена */
int	hit_wall(t_cfg *cfg, int mx, int my)
{
	if (mx < 0 || my < 0 || my >= cfg->map.h || mx >= cfg->map.w)
		return (1);
	if (cfg->map.grid[my][mx] == '1')
		return (1);
	return (0);
}

/*
** raycast():
**   • col       — номер вертикальной полосы экрана (0..WIN_W-1)
**   • line_h    — возвращаемая высота столба стены
**   Алгоритм DDA идёт до первой '1'.
*/
/* возвращает расстояние и записывает высоту полосы */
double	raycast(t_cfg *cfg, int col, int *line_h, double *wall_x)
{
	double	camera_x;     // -1 .. +1 по ширине экрана
	double	ray_dir;      // абсолютный угол луча в мире
	double	delta_x;      // длина шага до следующего X-среза
	double	delta_y;      // длина шага до следующего Y-среза
	int		map_x;        // текущая клетка карты (x)
	int		map_y;        // текущая клетка карты (y)
	double	side_dist_x;  // расстояние до ближайшей вертик. грани
	double	side_dist_y;  // расстояние до ближайшей гориз. грани
	int		step_x;       // +1 или -1
	int		step_y;
	int		side;         // 0 = X-грань, 1 = Y-грань
	double	perp;         // перпендикулярная дистанция

	// 1. направление луча
	camera_x = (2.0 * col / (double)WIN_W) - 1.0;
	ray_dir = cfg->pl.dir + camera_x * 0.523598; // 60° = π/3
	delta_x = fabs(1 / cos(ray_dir));
	delta_y = fabs(1 / sin(ray_dir));

	// 2. стартовая клетка
	map_x = (int)cfg->pl.x;
	map_y = (int)cfg->pl.y;

	// 3. шаги и расстояния
	if (cos(ray_dir) < 0)
	{
		step_x = -1;
		side_dist_x = (cfg->pl.x - map_x) * delta_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = (map_x + 1.0 - cfg->pl.x) * delta_x;
	}
	if (sin(ray_dir) < 0)
	{
		step_y = -1;
		side_dist_y = (cfg->pl.y - map_y) * delta_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = (map_y + 1.0 - cfg->pl.y) * delta_y;
	}

	// 4. DDA
	while (1)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += delta_y;
			map_y += step_y;
			side = 1;
		}
		if (hit_wall(cfg, map_x, map_y))
			break ;
	}

	// 5. перпендикулярная дистанция
	double dist;
	if (side == 0)
		dist = (map_x - cfg->pl.x + (1 - step_x) / 2.0) / cos(ray_dir);
	else
		dist = (map_y - cfg->pl.y + (1 - step_y) / 2.0) / sin(ray_dir);
	perp = dist * cos(ray_dir - cfg->pl.dir);

	if (side == 0)
		*wall_x = cfg->pl.y + dist * sin(ray_dir);
	else
		*wall_x = cfg->pl.x + dist * cos(ray_dir);
	*wall_x -= floor(*wall_x);
	// 6. высота линии
	*line_h = (int)(WIN_H / perp);

	// 7. установка текстуры
	if (side == 0 && cos(ray_dir) > 0)
		cfg->tex_id = TEX_WE; // запад
	else if (side == 0 && cos(ray_dir) < 0)
		cfg->tex_id = TEX_EA; // восток
	else if (side == 1 && sin(ray_dir) > 0)
		cfg->tex_id = TEX_NO; // север
	else
		cfg->tex_id = TEX_SO; // юг

	return perp;
}