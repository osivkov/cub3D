/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkhvorov <pkhvorov@student.codam.nl>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 16:52:04 by pkhvorov          #+#    #+#             */
/*   Updated: 2025/05/28 16:52:27 by pkhvorov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	safe_free(char **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

static void	clean_mlx(t_app *app)
{
	if (app->mlx.screen.img)
	{
		mlx_destroy_image(app->mlx.mlx, app->mlx.screen.img);
		app->mlx.screen.img = NULL;
	}
	if (app->mlx.win)
	{
		mlx_destroy_window(app->mlx.mlx, app->mlx.win);
		app->mlx.win = NULL;
	}
	if (app->mlx.mlx)
	{
		mlx_destroy_display(app->mlx.mlx);
		free(app->mlx.mlx);
		app->mlx.mlx = NULL;
	}
}

void	clean_up(t_app *app)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (app->cfg.textures[i].pixels)
		{
			free(app->cfg.textures[i].pixels);
			app->cfg.textures[i].pixels = NULL;
		}
		i++;
	}
	safe_free(&app->cfg.tex_north);
	safe_free(&app->cfg.tex_south);
	safe_free(&app->cfg.tex_west);
	safe_free(&app->cfg.tex_east);
	clean_mlx(app);
}
