# Nazwa kompilatora
CC = gcc

# Flagi kompilatora
CFLAGS = -Wall -pthread

# Nazwy plików wykonywalnych
SERVER = server
CLIENT = client

# Pliki źródłowe
SERVER_SRC = server.c
CLIENT_SRC = client.c

# Domyślna reguła, która kompiluje wszystko
all: $(SERVER) $(CLIENT)

# Reguła kompilacji serwera
$(SERVER): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Reguła kompilacji klienta
$(CLIENT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $^

# Reguła czyszczenia plików binarnych
clean:
	rm -f $(SERVER) $(CLIENT)

.PHONY: all clean
