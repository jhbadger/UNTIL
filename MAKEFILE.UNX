OBJECTS= search.o call.o file.o help.o io.o math.o outer.o prim.o screen.o seal.o string.o until.o user.o vectored.o vocab.o debug.o showbit.o float.o system.o quote.o sharp.o sengine.o case.o
HEADERS= until.h compiler.h functs.h untilpri.h io.h
FLAGS= -c
CC=gcc

Until: $(OBJECTS)
	$(CC) $(OBJECTS) -o Until

search.o: $(HEADERS) search.c
	$(CC) $(FLAGS) search.c

quote.o: $(HEADERS) quote.c
	$(CC) $(FLAGS) quote.c

sengine.o: $(HEADERS) sengine.c
	$(CC) $(FLAGS) sengine.c

sharp.o: $(HEADERS) sharp.c
	$(CC) $(FLAGS) sharp.c

showbit.o: $(HEADERS) showbit.c
	$(CC) $(FLAGS) showbit.c

call.o: $(HEADERS) call.c
	$(CC) $(FLAGS) call.c

file.o: $(HEADERS) file.c
	$(CC) $(FLAGS) file.c

help.o: $(HEADERS) help.c
	$(CC) $(FLAGS) help.c

io.o: $(HEADERS) io.c
	$(CC) $(FLAGS) io.c

math.o: $(HEADERS) math.c
	$(CC) $(FLAGS) math.c

outer.o: $(HEADERS) outer.c
	$(CC) $(FLAGS) outer.c

prim.o: $(HEADERS) prim.c
	$(CC) $(FLAGS) prim.c

screen.o: $(HEADERS) screen.c
	$(CC) $(FLAGS) screen.c

seal.o: $(HEADERS) seal.c
	$(CC) $(FLAGS) seal.c

string.o: $(HEADERS) string.c
	$(CC) $(FLAGS) string.c

until.o: $(HEADERS) until.c
	$(CC) $(FLAGS) until.c

user.o: $(HEADERS) user.c
	$(CC) $(FLAGS) user.c

vectored.o: $(HEADERS) vectored.c
	$(CC) $(FLAGS) vectored.c

vocab.o: $(HEADERS) vocab.c
	$(CC) $(FLAGS) vocab.c

debug.o: $(HEADERS) debug.c
	$(CC) $(FLAGS) debug.c

system.o: $(HEADERS) system.c
	$(CC) $(FLAGS) system.c

float.o: $(HEADERS) float.c
	$(CC) $(FLAGS) float.c

case.o: $(HEADERS) case.c
	$(CC) $(FLAGS) case.c


