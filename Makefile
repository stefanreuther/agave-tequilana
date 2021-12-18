PDK = ../pdk
CFLAGS = -W -Wall -O -I$(PDK) -std=c99
O = commands.o config.o language.o main.o message.o score.o sendconf.o state.o util.o

cactus: $(O)
	$(CC) -o $@ $(O) -L$(PDK) -lpdk -lm
