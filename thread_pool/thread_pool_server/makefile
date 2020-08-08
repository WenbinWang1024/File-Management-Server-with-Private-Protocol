RM := rm -f
CC := gcc
headpath := ./head/
srcpath := ./src/
binpath := ./bin/
srcs := $(wildcard $(srcpath)*.c)
objs := $(patsubst $(srcpath)%.c,$(binpath)%.o,$(srcs))
out := ftpserver
$(out):$(objs)
	$(CC) -o $@ $^ -pthread -Wall
$(binpath)%.o:$(srcpath)%.c
	$(CC) -c $^ -o $@ -I $(headpath) -pthread -Wall
.PHONY:clean rebuild
rebuild:clean $(out)
clean:
	$(RM) $(binpath)* $(out)

