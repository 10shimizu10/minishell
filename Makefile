NAME     = minishell
CC       = cc
RLDIR    = $(shell brew --prefix readline)
INCLUDES = -I include -I$(RLDIR)/include
CFLAGS   = -Wall -Wextra -Werror $(INCLUDES)
LIBS     = -lreadline -L$(RLDIR)/lib
SRCS     = src/main.c\
		   src/error/error0.c\
           src/error/error1.c\
           src/tokenize/tokenize.c\
           src/destructor.c\
		   src/expand/expand.c\
		   src/parse/parse.c\
		   src/redirect/redirect0.c\
		   src/redirect/redirect1.c\
		   src/pipe.c\
		   src/exec/exec0.c\
		   src/exec/exec1.c\
		   src/signal.c\
		   src/builtin/builtin.c\
		   src/builtin/builtin_exit.c\
		   src/builtin/builtin_export.c\
		   src/builtin/builtin_unset.c\
		   src/builtin/builtin_env.c\
		   src/builtin/builtin_cd0.c\
		   src/builtin/builtin_cd1.c\
		   src/builtin/builtin_echo.c\
		   src/builtin/builtin_pwd.c\
		   src/map/map0.c\
		   src/map/map1.c\
		   src/map/map2.c\
		   src/env.c\
		   src/ft_utils/ft_utils0.c\
		   src/ft_utils/ft_utils1.c\
		   src/ft_utils/ft_utils2.c\
		   src/ft_utils/ft_utils3.c\
		   src/ft_utils/ft_utils4.c\
		   src/ft_utils/ft_utils5.c\

OBJS     = $(SRCS:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS) history.txt

fclean: clean
	$(RM) $(NAME) history.txt

re: fclean all

test: all
	./test.sh
.PHONY: all clean fclean re test

# Linux | Darwin
OS := $(shell uname -s)

ifeq ($(OS),Linux)
	# commands for Linux
endif

ifeq ($(OS),Darwin)
	# commands for macos
	RLDIR = $(shell brew --prefix readline)
	INCLUDES += -I$(RLDIR)/include
	LIBS     += -L$(RLDIR)/lib
endif