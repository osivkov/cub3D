/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:10:13 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/13 10:37:26 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "mlx.h"
#include "cub3d.h"
#include <stdlib.h>

int	close_hook(void *param)
{
	(void)param;
	exit(0);
	return (0);
}

static int	key_press(int keycode, void *p)
{
	(void)p;
	if (keycode == 65307)	/* ESC */
		exit(0);
	return (0);
}

int	main(int ac, char **av)
{
	t_app	app;

	if (ac != 2)
		return (write(2, "Usage: ./cub3D scene.cub\n", 25), 1);
			/* -------- парсер ------- */
	if (parse_header(av[1], &app.cfg) || parse_map(av[1], &app.cfg))
		return (write(2, "Error\ninvalid .cub\n", 19), 1);
		/* -------- графика ------- */
	app.mlx.mlx = mlx_init();
	app.mlx.win = mlx_new_window(app.mlx.mlx, WIN_W, WIN_H, "cub3D");
	app.mlx.screen.img = mlx_new_image(app.mlx.mlx, WIN_W, WIN_H);
	app.mlx.screen.addr = (int *)mlx_get_data_addr(app.mlx.screen.img,
			&app.mlx.screen.bpp, &app.mlx.screen.line_len,
			&app.mlx.screen.endian);
				/* ---- события ---- */
	mlx_hook(app.mlx.win, 2, 1L << 0, key_press, &app); /* крестик */
	mlx_hook(app.mlx.win, 17, 0, close_hook, &app);  /* ESC */
	mlx_loop_hook(app.mlx.mlx, frame, &app);
	mlx_loop(app.mlx.mlx);
	return (0);
}