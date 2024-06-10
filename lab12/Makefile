# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -pthread

# Targets
SERVER = udp_server
CLIENT = udp_client

# Source files
SERVER_SRC = udp_server.c
CLIENT_SRC = udp_client.c

# Default target
all: $(SERVER) $(CLIENT)

# Server target
$(SERVER): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_SRC)

# Client target
$(CLIENT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT_SRC)

# Clean up
clean:
	rm -f $(SERVER) $(CLIENT)

# PHONY targets
.PHONY: all clean
