# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: radahman <radahman@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/19 21:21:25 by mobullad          #+#    #+#              #
#    Updated: 2025/09/18 17:16:08 by radahman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

PRINT = 	src/print/printf/ft_print_hex.c \
			src/print/printf/ft_printf.c \
			src/print/printf/ft_unsigned_putnbr.c \
			src/print/printf/ft_putnbr.c \
			src/print/printf/ft_putstr.c \
			src/print/printf/ft_putchar.c \
			src/print/ft_putchar_fd.c  \
			src/print/ft_putstr_fd.c \
			src/print/ft_putendl_fd.c \
			src/print/ft_putnbr_fd.c \

GNL = 		src/get_next_line/get_next_line.c

CHECK =		src/check/ft_isalpha.c \
			src/check/ft_isdigit.c \
			src/check/ft_isalnum.c \
			src/check/ft_isascii.c \
			src/check/ft_isprint.c \

STRING =	src/string/ft_strlen.c \
			src/string/ft_strlcpy.c \
			src/string/ft_strlcat.c \
			src/string/ft_strchr.c \
			src/string/ft_strrchr.c \
			src/string/ft_strncmp.c \
			src/string/ft_strnstr.c \
			src/string/ft_strdup.c \
			src/string/ft_substr.c \
			src/string/ft_strmapi.c \
			src/string/ft_striteri.c \
			src/string/ft_strjoin.c \
			src/string/ft_strjoin_f1.c \
			src/string/ft_strtrim.c \
			src/string/ft_split.c \

CONVERT =	src/convert/ft_atoi.c \
			src/convert/ft_toupper.c \
			src/convert/ft_tolower.c \
			src/convert/ft_itoa.c \

LIST =		src/lst/ft_lstnew.c \
			src/lst/ft_lstadd_front.c \
			src/lst/ft_lstsize.c \
			src/lst/ft_lstlast.c \
			src/lst/ft_lstadd_back.c \
			src/lst/ft_lstdelone.c \
			src/lst/ft_lstclear.c \
			src/lst/ft_lstiter.c \
			src/lst/ft_lstmap.c \
			
MEMORY =	src/memory/ft_memset.c \
			src/memory/ft_bzero.c \
			src/memory/ft_memcpy.c \
			src/memory/ft_memmove.c \
			src/memory/ft_memchr.c \
			src/memory/ft_memcmp.c \
			src/memory/ft_calloc.c \
			src/garbage_collector.c

FREE =		src/free/ft_free_tab.c

SRC =	$(PRINT) $(GNL) \
		$(CHECK) $(STRING) $(CONVERT) $(LIST) $(MEMORY) $(FREE)

BUILD = build/

OBJS = $(SRC:%.c=$(BUILD)%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I include -g3

COUNTER = 0
TOTAL = $(words $(SRC))

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "\nArchived $(NAME)"

$(BUILD)%.o: %.c 
	@mkdir -p ${@D}
	$(eval COUNTER := $(shell echo $$(($(COUNTER)+1))))
	@printf "${GREEN}libft ${MAGENTA}[${COUNTER}/${TOTAL}]${RESET}\r"
	@${CC} ${CFLAGS} -c $< -o $@
 
clean:
	@rm -rf $(OBJS) $(BUILD) libft.a
  
fclean: clean
	@rm -f $(NAME)
  
re: fclean all
  
.PHONY: clean fclean re all

# ==================== COLORS ==================== #

GREEN = \033[1;32m
CYAN = \033[1;36m
YELLOW = \033[1;33m
MAGENTA = \033[1;35m
RED = \033[1;31m
RESET = \033[0m