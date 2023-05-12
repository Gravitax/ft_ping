NAME=ft_ping

CC=gcc

FLAGS = -Wall -Werror -Wextra
FLAGS += -Wno-documentation
FLAGS += -Wno-documentation-unknown-command
FLAGS += -Wno-reserved-id-macro
FLAGS += -Wno-missing-noreturn
FLAGS += -Wno-incompatible-pointer-types-discards-qualifiers

# Flags variables
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    FLAGS += -g3 -fsanitize=address
endif

OPTI ?= 0
ifeq ($(OPTI), 1)
    FLAGS += -Ofast -march=native
endif

UNAME_S := $(shell uname -s)

SRC_PATH=srcs/
SRC_NAME=main.c\
	core/dns.c\
	core/request.c\
	core/socket.c\
	utils/checksum.c\
	utils/exit.c\
	utils/print.c\
	utils/singletons.c\
	utils/time.c

SRC=$(addprefix $(SRC_PATH), $(SRC_NAME))

INC_PATH=includes/
INC_NAME=main.h\
		error.h\
		icmp.h

INC=$(addprefix $(INC_PATH), $(INC_NAME))

OBJS=$(SRC:.c=.o)

########################## Librarys ######################
LIBS_PATH=libs

LIB_PATH=$(LIBS_PATH)/libft
LIB=$(LIB_PATH)/libft.a

##########################################################

all: $(NAME)

$(NAME): $(LIB) $(OBJS)
	$(CC) $(FLAGS) -o $(NAME) $(OBJS) $(LIB)

$(SRC_PATH)%.o: $(SRC_PATH)%.c $(INC)
	@tput civis
	@printf " Compiling $<"
	@printf "                                       \\r"
	@printf "$(OS)"
	@tput cnorm
	@$(CC) $(FLAGS) -I$(INC_PATH) -I$(LIB_PATH) -o $@ -c $<

########################## Library rules ##########################

$(LIB): $(LIB_PATH)
	@echo "Making Libft..."
	@make -C $(LIB_PATH) -j4

###################################################################

clean:
	@rm -rf $(OBJS)
	@make -C $(LIB_PATH) clean

fclean: clean
	@rm -rf $(NAME)
	@make -C $(LIB_PATH) fclean

re: fclean all
