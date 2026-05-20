all:
	gcc -Wall main.c tarefa.c -o todo

run:
	./todo

clean:
	rm -f todo