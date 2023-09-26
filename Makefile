NAME 		= webserv

CC			= c++
FLAGS 		= -Wall -Wextra -Werror
RM			= rm -rf

SRC_DIR 	= src/

OBJ_DIR		= obj/

_SRC 		=	main.cpp

OBJ_FOLDERS =	obj/

SRC 		= ${addprefix ${SRC_DIR}, ${_SRC}}
OBJ			= ${patsubst ${SRC_DIR}%.cpp, ${OBJ_DIR}%.o, ${SRC}}

INCLUDES	= -I ./include

${NAME}:	${OBJ}
			@echo "Compiling $(NAME)..."
			@$(CC) $(INCLUDES) $(OBJ) -o $(NAME) $(LIBS)
			@echo "$(NAME) compiled!"

${OBJ_DIR}%.o: ${SRC_DIR}%.cpp
			@mkdir -p ${OBJ_DIR}
			@mkdir -p ${OBJ_FOLDERS}
			@${CC} ${FLAGS} ${INCLUDES} -c $^ -o $@

all:		${NAME}

clean:
			@echo "Removing files..."
			@${RM} ${OBJ}

fclean: 	clean
			@${RM} ${NAME}

re:		fclean all

.PHONY:	all clean fclean re bonus
