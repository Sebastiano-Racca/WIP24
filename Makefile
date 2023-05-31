src = $(wildcard *.c)
obj = $(src: .c = o)
bin = dama

$(bin): $(obj)
	$(CC) -o $@ $^

%.o: %.c 
	gcc -c -o $@ $<