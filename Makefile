SRCS 		= free.c main.c parser.c timer.c \
			  dongler.c generator.c queue.c \
			  threader.c work_routine.c supervisor.c priority.c alternate.c
CC			= cc
RM			= rm -rf
CFLAGS		= -Wall -Wextra -Werror -pthread
NAME		= codexion
OBJS		= $(SRCS:.c=.o)

$(NAME):	$(OBJS) 
			$(CC) $(CFLAGS) $(OBJS) -o $@

all:		$(NAME)

clean:		
		$(RM) $(OBJS) $(BONUS_OBJS)

fclean:	clean
		$(RM) $(NAME)

re:		fclean $(NAME)

.PHONY:		all clean fclean re
		
