#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


struct BlogOperation {
    int client_id;
    int operation_type;
    int server_response;
    char topic[50];
    char content[2048];
};

struct client{
    int client_id;
    int socket;
    pthread_t thread;
};

struct post{
    char topic[50];
    char content[2048];
    int author_id;
};

struct topic{
    int topic_id; //posicao dele na lista de topicos do blog
    char topic[50];
    int clients[10];
    struct post posts[10];
    int owner;
    int posts_count;
    int client_count;
};

struct server{
    int server_id;
    int clients[10];
    int clients_count;
    int topics_count;
    struct topic topics[10];
};

struct Blog{
    struct topic list_topics[10];
    int list_topics_count;
    int list_clients_count;
    struct client list_clients[10];
    int id[10];
};

int addrparse(const char * addrstr, const char * portstr, struct sockaddr_storage * storage);
int server_sockaddr_init(const char * protocol_version, const char * portstr, struct sockaddr_storage * storage);
void logexit(const char * msg);
size_t receive_all(int socket, void * buffer, size_t length);