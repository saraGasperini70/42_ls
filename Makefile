NAME = ft_ls

SRC_ROOT = srcs
LIB_ROOT = libs
OBJ_ROOT = objs
LIBFT_DIR = $(LIB_ROOT)/42_libft
PRINTF_DIR = $(LIB_ROOT)/42_ft_printf
LIBFT = $(LIBFT_DIR)/libft.a
PRINTF = $(PRINTF_DIR)/libftprintf.a

SRCS_DIR = $(shell find ./$(SRC_ROOT) -name '*.c')
LIB_SRCS_DIR = $(shell find ./$(LIB_ROOT) -name '*.c')

OBJ_DIR = $(patsubst $(SRC_ROOT)/%, $(OBJ_ROOT)/%, $(SRC_ROOT))
LIB_OBJS = $(patsubst $(LIB_ROOT)/%, $(OBJ_ROOT)/%, $(LIB_SRCS))

# All .c files in the srcs directory and its subdirectories
SRCS = $(SRCS_DIR)
LIBFT_SRCS = $(wildcard $(addsuffix *.c , $(LIBFT_DIR)))
LIBPRINTF_SRCS = $(wildcard $(addsuffix *.c , $(PRINTF_DIR)))
OBJ = $(patsubst $(SRC_ROOT)/%, $(OBJ_ROOT)/%, $(SRCS:.c=.o))
LIB_OBJFT = $(patsubst $(LIBFT_DIR)/%, $(OBJ_ROOT)/42_libft%, $(LIBFT_SRCS:.c=.o))
LIB_OBJPRINTF = $(patsubst $(PRINTF_DIR)/%, $(OBJ_ROOT)/42_printf%, $(LIBPRINTF_SRCS:.c=.o))

FLAGS = gcc -Wall -Wextra -Werror -g
LINKS = -L${LIBFT_DIR} -lft -L${PRINTF_DIR} -lftprintf
INCLUDE = -I${LIBFT_DIR} -I${PRINTF_DIR}

$(OBJ_DIR)/%.o: %.c
	@$(FLAGS) -c $< -o $@ $(INCLUDE) $(LINKS)
	@echo "Compiling $@..."

all: $(LIBFT) $(PRINTF) $(NAME)
	@echo "Compiling $(NAME)..."

$(LIBFT):
	@echo "Compiling libft with bonus functions (make bonus)..."
	@make -C $(LIBFT_DIR) bonus

$(PRINTF):
	@make -C $(PRINTF_DIR)

$(NAME): $(OBJ)
	@echo "Compiling $(SRCS)..."
	@echo "Linking $(NAME)..."
	@mkdir -p $(OBJ_DIR)
	@$(FLAGS) $(LIBFT) $(PRINTF) $(OBJ) -o $(NAME) $(LINKS)

clean:
	@echo "Cleaning..."
	@rm -f $(OBJ) $(LIB_OBJ)
	@make clean -C $(LIBFT_DIR)
	@make clean -C $(PRINTF_DIR)
	@echo "Done."

fclean: clean
	@echo "Removing executable..."
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)
	@make fclean -C $(PRINTF_DIR)
	@echo "Done."

re: fclean all

re: fclean all

.PHONY : all clean fclean re
