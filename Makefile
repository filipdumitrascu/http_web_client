# 323CA Dumitrascu Filip-Teodor
SOURCES=client.cpp lib/*
CFLAGS=-Wall -Werror -Wno-error=unused-variable
INCPATHS=include
CC=g++

# Automatic generation of some important lists
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))

client: $(SOURCES)
	$(CC) $(INCFLAGS) $(CFLAGS) $(SOURCES) -o client

run: client
	./client

check: client
	python3 checker.py --script ALL ./client

pack:
	zip -FSr 323CA_Dumitrascu_FilipTeodor_Tema4PC.zip include/ lib/ client.cpp Makefile README.md

clean:
	rm -f *.o client
