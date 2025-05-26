NAME	:= cub3D

# ──────────────────────────────────────────────────────────────────────────────
# Директории
# ──────────────────────────────────────────────────────────────────────────────
SRCDIR	:= src
INCDIR	:= include
OBJDIR	:= obj
MLXDIR	:= mlx
LFTDIR	:= libft

# ──────────────────────────────────────────────────────────────────────────────
# Файлы
# ──────────────────────────────────────────────────────────────────────────────
SRCS	:= $(wildcard $(SRCDIR)/*.c)
OBJS	:= $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

# ──────────────────────────────────────────────────────────────────────────────
# Компилятор и флаги
# ──────────────────────────────────────────────────────────────────────────────
CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror -g -I$(INCDIR) -I$(MLXDIR) -I$(LFTDIR)

LIBS	:= -L$(MLXDIR) -lmlx -lXext -lX11 -lm \
			-L$(LFTDIR) -lft

# ──────────────────────────────────────────────────────────────────────────────
# Цели
# ──────────────────────────────────────────────────────────────────────────────
all: $(NAME)

$(NAME): mlx libft $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

mlx:
	$(MAKE) -C $(MLXDIR)

libft:
	$(MAKE) -C $(LFTDIR)

clean:
	$(MAKE) clean -C $(MLXDIR)
	$(MAKE) clean -C $(LFTDIR)
	rm -rf $(OBJDIR)

fclean: clean
	$(MAKE) fclean -C $(LFTDIR)
	rm -f $(NAME)

re: fclean all

bonus:
	@echo "Bonus not implemented yet"

.PHONY: all clean fclean re bonus mlx libft