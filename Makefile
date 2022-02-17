CC = g++
CFLAGS = -Wall -O
CLIBS = -lstdc++ -pthread
INC = -I src/

all: prodcon

prodcon: main.o producer.o common.o consumer.o tands.o log.o
	${CC} ${CFLAGS} -o prodcon main.o producer.o common.o consumer.o tands.o log.o ${CLIBS}

main.o: main.cpp
	${CC} ${CFLAGS} ${INC} -c main.cpp

producer.o: src/producer.h src/producer.cpp
	${CC} ${CFLAGS} ${INC} -c src/producer.h src/producer.cpp

common.o: src/common.cpp src/common.h
	${CC} ${CFLAGS} ${INC} -c src/common.h src/common.cpp

consumer.o: src/consumer.cpp src/consumer.h
	${CC} ${CFLAGS} ${INC} -c src/consumer.h src/consumer.cpp

tands.o: src/tands.c src/tands.h
	${CC} ${CFLAGS} ${INC} -c src/tands.h src/tands.c

log.o: src/log.cpp src/log.h
	${CC} ${CFLAGS} ${INC} -c src/log.h src/log.cpp

clean:
	rm *.o
	rm -rf src/*.gch
	rm *.log
	rm prodcon