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
    struct topic subscribed_topics[10000];
    int subscribed_topics_count = 0;
    int socket;
    p_thread thread;
};

struct post{
    char topic[50];
    char content[2048];
    int author_id;
}

struct topic{
    char topic[50];
    int clients[10];
    struct post posts[1000];
    int owner;
    int posts_count = 0;
    int client_count = 0;
};

struct server{
    int server_id;
    int clients[10];
    int clients_count = 0;
    int topics_count = 0;
    struct topic topics[10000];
};

struct Blog{
    struct topic list_topics[10000];
    int list_topics_count = 0;
    int list_clients_count = 0;
    struct client list_clients[10];
}