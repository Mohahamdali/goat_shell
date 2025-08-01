SRCS =	main.c \
		parser/toknizer.c \
		parser/syntax_error.c \
		parser/parser.c \
		parser/expander.c \
		parser/heredoc_expansion.c \
		parser/set_env.c \
		parser/set_env_utils.c \
		parser/libft_utils.c \
		parser/libft_utils_2.c \
		parser/my_split.c \
		parser/signals.c \
		parser/parser_utils.c \
		parser/expanded_size.c \
		parser/expander_utils.c \
		parser/expander_utils_2.c \
		parser/garbage_collector.c \
		execution/ft_split.c \
		execution/unset.c \
		execution/exit.c \
		execution/clean.c \
		execution/execution.c \
		execution/export.c \
		execution/export_utils.c \
		execution/export_utils2.c \
		execution/external_cmd.c \
		execution/external_cmd_u2.c \
		execution/external_cmd_u3.c \
		execution/parse_export.c \
		execution/lib_utils.c \
		execution/lib_utils_2.c \
		execution/pipe.c \
		execution/redirection.c \
		execution/redirection_heredoc.c \
		execution/builtin_cd.c \
		execution/builtin_cd_utils.c \
		execution/herdoc_utils.c \
		execution/errors.c \
		execution/env.c \
		execution/echo.c \
		execution/pipe_utils.c \
		execution/pipe_utils2.c \
		execution/sub_command_path.c \
		execution/redirection_utils.c

OBJ = $(SRCS:.c=.o)
NAME = minishell
CFLAGS = -Wall -Wextra -Werror
CC = cc
RM = rm -f

all: $(NAME)



$(NAME): $(OBJ)
	$(CC) $(CFLAGS)  $(OBJ) -o $(NAME)  -lreadline


%.o:%.c minishell.h
	$(CC) $(CFLAGS)  -c $< -o $@

clean:
	$(RM) $(OBJ)
fclean: clean
	$(RM) $(NAME)
re: fclean all

run:    $(NAME)
	make all clean
	./$(NAME)
