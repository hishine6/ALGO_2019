CC = gcc
TARGET = mp1_20161566

$(TARGET) : algo.o
	$(CC) -o $(TARGET) algo.c

clean:
	rm *.o mp1_20161566
