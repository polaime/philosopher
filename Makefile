# /* ************************************************************************** */
# /*                                                     ..,,;;;,'..            */
# /*                                                 .'cx0XNNWWWNXKOdl;.        */
# /*   MAKEFILE.c	                               .;xXWNKOkkOKNMMMMMMW0l.      */
# /*                                              ,xNMXd,.   .cONMMMMMMMK;      */
# /*   Par: polaime <pthibaud>                   .:0WMX:      :XMMNNMMMMM0,     */
# /*                                             :KMMMk.      .lxdckMMMMWd.     */
# /*   Cree: 2025/01/19 13:50:28                ,OMMMMK,          ,0MMMWO'      */
# /*   Ameliore: 2025/01/19 14:42:13           .oWMMMMW0c.      .c0WMMWk,       */
# /*                                           'OMMWXNWMWKkddodkKWMMWKo.        */
# /*                                           'OMWk,,lx0XWWMMMMMWXOo'.         */
# /*   https://github.com/polaime              .oKx'    .';:clllc;'.            */
# /*                                                                            */
# /* ************************************************************************** */



#################################
## ARGUMENTS

NAME		= philosopher
CFLAGS		= -g3 -Wall -Werror -Wextra 
LDFLAGS		= 
CC			= cc

#################################
## SOURCES

SRC_DIRS	=	src
BUILD_DIR	=	build
SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))
NAME = philosopher

#################################
## RULES
all: ${NAME}

${NAME}: ${OBJS} 
	@echo "\033[32mCompilation en cours...\033[0m"
	@${CC} ${CFLAGS} ${OBJS} -o $@ $(LDFLAGS)
	@echo "\033[32mCompilation terminee\033[0m"

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@${CC} ${CFLAGS} -c $< -o $@

clean:
	@rm -rf ${BUILD_DIR}
	@$(MAKE) -C clean
	@echo "\033[31mFichiers et dossier objets supprimés.\033[0m"

fclean: clean
	@rm -f ${NAME}
	@echo "\033[31mExecutable supprimé.\033[0m"

re: fclean all

.PHONY: all clean fclean re 