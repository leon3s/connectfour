EXE := connectfour.exe
CC := gcc

EXE_DIR := bin
SRCS_DIR := srcs
OBJS_DIR := objs
INCLUDES_DIR = includes

SRCS := $(wildcard $(SRCS_DIR)/*.c)
OBJS := $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

CPPFLAGS := -I $(INCLUDES_DIR)
CFLAGS   := -Wall -g -fPIC
LDLIBS   := -lncurses

.PHONY: all clean fclean re test

all: install-env $(EXE)

re: fclean all

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $(EXE_DIR)/$@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

install-env:
	@[ ! -d ${OBJS_DIR} ] && mkdir ${OBJS_DIR} || true
	@[ ! -d ${EXE_DIR} ] && mkdir ${EXE_DIR} || true

clean:
	@[ -d ${OBJS_DIR} ] && rm -r ${OBJS_DIR} || true

test:
	${EXE_DIR}/${EXE}

fclean: clean
	@[ -d ${EXE_DIR} ] && rm -r ${EXE_DIR} || true
