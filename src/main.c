/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: osivkov <osivkov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:10:13 by osivkov           #+#    #+#             */
/*   Updated: 2025/05/23 16:58:57 by osivkov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "mlx.h"
#include "cub3d.h"

int	main(int ac, char **av)
{
	t_app	app;

	// char	bad;
	
	
	if (ac != 2)
		return (write(2, "Usage: ./cub3D scene.cub\n", 25), 1);

	// bad = find_bad_symbol(av[1]);
	// if (bad)
    // {
    //     ft_printf("Error: invalid character '%c'\n", bad);
    //     return (1);
    // }
	if (init_cfg(&app.cfg)
	|| parse_scene(av[1],&app.cfg))
	/* 3) Проверяем замкнутость и корректность символов уже загруженной сетки */
   {
	   /* validate_map сама печатает в stderr причину ошибки */
	   return (1);
   }
   ft_printf(">>> Loaded map size: %d × %d\n", app.cfg.map.w, app.cfg.map.h);
	for (int yy = 0; yy < app.cfg.map.h; yy++)
	{
    // grid не нуль-терминирована, поэтому используем write
    write(1, app.cfg.map.grid[yy], app.cfg.map.w);
    write(1, "\n", 1);
	}
	if (!validate_map(&app.cfg))
		return ( 1);
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
