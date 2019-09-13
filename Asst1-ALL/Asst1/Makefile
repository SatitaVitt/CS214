TARGET= memgrind
OBJECTS= mymalloc.o memgrind.o
FLAGS= -Wall -g -Werror -fsanitize=address

$(TARGET): $(OBJECTS)
	gcc $(FLAGS) -o $@ $^
clean:
	rm -f $(TARGET) $(OBJECTS)
%.o: %.c
	gcc $(FLAGS) -c $<

mymalloc.o: mymalloc.h
memgrind.o: mymalloc.h
