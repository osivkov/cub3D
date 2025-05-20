#include "cub3d.h"

static int	load_xpm_texture(void *mlx, t_tex *tex, char *path)
{
	void	*img;
	int		*data;
	int		bpp, size_line, endian;

    printf("Loading texture: {%s}\n", path);
	img = mlx_xpm_file_to_image(mlx, path, &tex->width, &tex->height);
    if (!img)
    {
        printf("Failed to load texture: %s\n", path);
        return (1);
    }
	data = (int *)mlx_get_data_addr(img, &bpp, &size_line, &endian);
	if (!data)
    {
        mlx_destroy_image(mlx, img);
        return (1);
    }
		
	tex->pixels = malloc(sizeof(unsigned int) * tex->width * tex->height);
	if (!tex->pixels)
		return (1);

	for (int y = 0; y < tex->height; y++)
	{
		for (int x = 0; x < tex->width; x++)
		{
			int color = data[y * (size_line / 4) + x];
			tex->pixels[y * tex->width + x] = (unsigned int)color;
		}
	}

	mlx_destroy_image(mlx, img);
    printf("Loaded %s: %dx%d\n", path, tex->width, tex->height);
	return (0);
}

int init_textures_def(t_cfg *cfg)
{
    for (int i = 0; i < 4; i++)
    {
        cfg->textures[i].img = NULL;
        cfg->textures[i].width = 0;
        cfg->textures[i].height = 0;
        cfg->textures[i].pixels = NULL;
    }
    return 0;
}


int	init_textures(t_app *app)
{
    init_textures_def(&app->cfg);
	if (load_xpm_texture(app->mlx.mlx, &app->cfg.textures[TEX_NO], app->cfg.tex_north))
		return (1);
	if (load_xpm_texture(app->mlx.mlx, &app->cfg.textures[TEX_SO], app->cfg.tex_south))
		return (1);
	if (load_xpm_texture(app->mlx.mlx, &app->cfg.textures[TEX_WE], app->cfg.tex_west))
		return (1);
	if (load_xpm_texture(app->mlx.mlx, &app->cfg.textures[TEX_EA], app->cfg.tex_east))
		return (1);
	return (0);
}