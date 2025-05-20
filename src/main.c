/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:10:13 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/20 17:36:01 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "mlx.h"
#include "cub3d.h"

int	main(int ac, char **av)
{
	t_app	app;

	if (ac != 2)
		return (write(2, "Usage: ./cub3D scene.cub\n", 25), 1);

	if (init_cfg(&app.cfg))
		return (1);

	if (parse_header(av[1], &app.cfg) || parse_map(av[1], &app.cfg))
		return (write(2, "Error\ninvalid .cub\n", 19), 1);

	app.mlx.mlx = mlx_init();
	if (!app.mlx.mlx)
		return (write(2, "Failed to initialize mlx\n", 25), 1);

	if (init_textures(&app))
		return (write(2, "Failed to load textures\n", 24), 1);

	app.mlx.win = mlx_new_window(app.mlx.mlx, WIN_W, WIN_H, "cub3D");
	if (!app.mlx.win)
		return (write(2, "Failed to create window\n", 24), 1);

	app.mlx.screen.img = mlx_new_image(app.mlx.mlx, WIN_W, WIN_H);
	if (!app.mlx.screen.img)
		return (write(2, "Failed to create image\n", 23), 1);

	app.mlx.screen.addr = (int *)mlx_get_data_addr(app.mlx.screen.img,
			&app.mlx.screen.bpp, &app.mlx.screen.line_len,
			&app.mlx.screen.endian);

	mlx_hook(app.mlx.win, 2, 1L << 0, key_press, &app);  // KeyPress - для обработки клавиш
	mlx_hook(app.mlx.win, 17, 0, close_hook, &app);       // DestroyNotify - крестик окна

	mlx_loop_hook(app.mlx.mlx, frame, &app);
	mlx_loop(app.mlx.mlx);

	return (0);
}
