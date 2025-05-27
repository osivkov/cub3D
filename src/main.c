/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:10:13 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/27 14:11:22 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx.h"
#include "cub3d.h"

int	print_error(const char *msg)
{
	write(2, msg, ft_strlen(msg));
	return (1);
}

int	init_mlx(t_app *app)
{
	app->mlx.mlx = mlx_init();
	if (!app->mlx.mlx)
		return (print_error("Failed to initialize mlx\n"));
	app->mlx.win = mlx_new_window(app->mlx.mlx, WIN_W, WIN_H, "cub3D");
	if (!app->mlx.win)
		return (print_error("Failed to create window\n"));
	app->mlx.screen.img = mlx_new_image(app->mlx.mlx, WIN_W, WIN_H);
	if (!app->mlx.screen.img)
		return (print_error("Failed to create image\n"));
	app->mlx.screen.addr = (int *)mlx_get_data_addr(app->mlx.screen.img, \
		&app->mlx.screen.bpp, &app->mlx.screen.line_len, \
		&app->mlx.screen.endian);
	return (0);
}

int	main(int ac, char **av)
{
	t_app	app;

	if (ac != 2)
		return (print_error("Usage: ./cub3D scene.cub\n"));
	if (init_cfg(&app.cfg))
		return (1);
	if (parse_header(av[1], &app.cfg))
		return (print_error("Error: invalid .cub\n"));
	if (!validate_map_closed(&app.cfg.map))
		return (print_error("Error: invalid map\n"));
	if (init_mlx(&app))
		return (1);
	if (init_textures(&app))
		return (print_error("Failed to load textures\n"));
	mlx_hook(app.mlx.win, 2, 1L << 0, key_press, &app);
	mlx_hook(app.mlx.win, 17, 0, close_hook, &app);
	mlx_loop_hook(app.mlx.mlx, frame, &app);
	mlx_loop(app.mlx.mlx);
	return (0);
}
