# *************************	sr	src/lexer/expand_helpers.c \
	src/lexer/expand_merge.c \
	src/lexer/expand_merge_utils.c \
	src/lexer/expand_var_utils.c \
	src/lexer/expand_replace.c \xer/expand_helpers.c \
	src/lexer/expand_merge.c \
	src/lexer/expand_merge_utils.c \
	src/lexer/expand_var_utils.c \************************************************ #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mobullad <mobullad@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/19 21:21:25 by mobullad          #+#    #+#              #
#    Updated: 2025/08/16 19:25:20 by mobullad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRC = main.c \
	src/lexer/lexer.c \
	src/lexer/lexer_quotes.c \
	src/lexer/lexer_single_quote.c \
	src/lexer/token_utils.c \
	src/lexer/token_utils_helpers.c \
	src/lexer/token_quotes.c \
	src/lexer/token_quotes_utils.c \
	src/lexer/token_quotes_utils_2.c \
	src/utils/utils.c \
	src/utils/input_handler.c \
	src/utils/main_execution.c \
	src/env/env_utils.c \
	src/env/env_utils_basic.c \
	src/env/env_export.c \
	src/env/env_cpy.c \
	src/lexer/expand.c \
	src/lexer/expand_utils.c \
	src/lexer/expand_core.c \
	src/lexer/expand_core_utils.c \
	src/lexer/expand_helpers.c \
	src/lexer/expand_helpers_2.c \
	src/lexer/expand_merge.c \
	src/lexer/expand_merge_2.c \
	src/lexer/expand_merge_utils.c \
	src/lexer/expand_var_utils.c \
	src/lexer/expand_replace.c \
	src/lexer/token_operators.c \
	src/lexer/token_operators_utils.c \
	src/parser/cmd_utils.c \
	src/parser/parser_utils.c \
	src/parser/parser_redirect_utils.c \
	src/parser/parser_cmd_utils.c \
	src/parser/parser.c  \
	src/executor/executor.c	\
	src/executor/executor_helpers.c \
	src/executor/executor_utils.c \
	src/executor/executor_utils_2.c \
	src/executor/executor_utils_3.c \
	src/executor/executor_utils_4.c \
	src/executor/executor_child.c \
	src/executor/exec_utils_4.c \
	src/executor/handle_redirection.c \
	src/executor/handle_heredoc.c \
	src/executor/handle_heredoc_utils.c \
	src/executor/handle_heredoc_expand.c \
	src/executor/open_redirection.c \
	src/builtins/builtin_utils.c \
	src/builtins/cd.c \
	src/builtins/pwd.c \
	src/builtins/export.c \
	src/builtins/unset.c \
	src/builtins/env.c \
	src/builtins/exit.c \
	src/builtins/echo.c \
	src/signal/signals.c

BUILD = build/

OBJS = $(SRC:%.c=$(BUILD)%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude -g3

COUNTER = 0
TOTAL = $(words $(SRC))

all: $(NAME)

$(NAME): $(OBJS)
	make -C ./lib/libft/
	cp ./lib/libft/libft.a .
	$(CC) $(CFLAGS) $(OBJS) -lreadline libft.a -o $(NAME)

$(BUILD)%.o: %.c 
	@mkdir -p ${@D}
	$(eval COUNTER := $(shell echo $$(($(COUNTER)+1))))
	@printf "${GREEN}libft ${MAGENTA}[${COUNTER}/${TOTAL}]${RESET}\r"
	@${CC} ${CFLAGS} -c $< -o $@
 
clean:
	@rm -rf $(OBJS) $(BUILD)
  
fclean: clean
	@rm -f $(NAME) libft.a
  
re: fclean all
  
.PHONY: clean fclean re all

# ==================== COLORS ==================== #

GREEN = \033[1;32m
CYAN = \033[1;36m
YELLOW = \033[1;33m
MAGENTA = \033[1;35m
RED = \033[1;31m
RESET = \033[0m