#include "common.h"

int id = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

struct Blog blog;

struct BlogOperation defining_id(struct BlogOperation operation){
    for(int i=0; i < 10; i++){
        if(id[i] == 0){
            id[i] = 1;
            operation.client_id = i;
            operation.server_response = 1;
            blog.list_clients[blog.list_clients_count] = operation.client_id;
            blog.list_clients_count++;
            return operation;
        }
    }
}

int verify_topic_existence(struct BlogOperation operation){
    struct topic subscribed_topic;
    subscribed_topic.topic = operation.topic;
    int i = 0; 
    while(blog.list_topics[i] != NULL){
        if(strcmp(blog.list_topics[i].topic, subscribed_topic.topic) == 0){
            return 1;
        }
        i++
    }
    return 0;
}

struct BlogOperation create_topic(struct BlogOperation operation){
    struct topic new_topic;
    new_topic.topic = operation.topic;
    new_topic.client[0] = operation.client_id;
    new_topic.owner = operation.client_id
    new_topic.client_count = 1;
    blog.list_topics[blog.list_topics_count] = malloc(sizeof(new_topic));
    int tam = blog.list_topics_count;
    blog.list_topics[tam] = new_topic;
    blog.list_topics_count++;
    return operation;
}

struct BlogOperation subscribe_topic(struct BlogOperation operation){
    struct topic subscribed_topic;
    subscribed_topic.topic = operation.topic;
    struct client subscribed_client = blog.list_clients[operation.client_id];
    int i = 0;
    while(blog.list_topics[i] != NULL){
        if(strcmp(blog.list_topics[i].topic, subscribed_topic.topic) == 0){
            subscribed_topic = blog.list_topics[i]
            int client_numb = subscribed_topic.client_count;
            int subscribed_client_id = operation.client_id;
            subscribed_topic.clients[subscribed_client_id] = 1;
            client_numb++;
            int num_topics = subscribed_client.subscribed_topics_count;
            subscribed_client.subscribed_topics[num_topics] = subscribed_topic;
            num_topics++;
            subscribed_client.subscribed_topics_count = num_topics;
            subscribed_topic.client_count = client_numb;
            blog.list_topics[i] = subscribed_topic;
            blog.list_clients[operation.client_id] = subscribed_client;
        }
        i++
    }
}

struct BlogOperation unsubscribe_topic(struct BlogOperation operation){
    struct topic subscribed_topic;
    subscribed_topic.topic = operation.topic;
    int i = 0;
    while(blog.list_topics[i] != NULL){
        if(strcmp(blog.list_topics[i].topic, subscribed_topic.topic) == 0){
            subscribed_topic = blog.list_topics[i]
            int client_numb = subscribed_topic.client_count;
            int subscribed_client = operation.client_id
            subscribed_topic.client[subscribed_client] = 0;
            client_numb--;
            subscribed_topic.client_count = client_numb;
            blog.list_topics[i] = subscribed_topic;
        }
        i++
    }
}

struct BlogOperation notify_participants(struct BlogOperation operation_client){
    struct BlogOperation operation;
    operation.client_id = operation_client.client_id;
    operation.operation_type = 2;
    operation.server_response = 1;
    operation.topic = operation_client.topic;
    operation.content = operation_client.content;
    return operation;
}

struct BlogOperation already_subscribed(struct BlogOperation operation_client){
    struct topic subscribed_topic;
    subscribed_topic.topic = operation.topic;
    int i = 0; 
    while(blog.list_topics[i] != NULL){
        //pega o topico da lista de topicos do blog
        if(strcmp(blog.list_topics[i].topic, subscribed_topic.topic) == 0){
            //verifica se aquele cliente ja esta inscrito
            if(subscribed_topic.clients[operation_client.client_id] == 1){
                return 1;
            }
        }
        i++
    }
    return 0;
}


int main(int argc, char *argv[]){
    initArgs(argc, argv); // initialize ip and port
    sem_init(&semaphore, 0, 1); // initialize semaphore
    initBlog(); // initialize blog
    int sockfd = initSocket(); // initialize socket

    while(1){
        // accept connection
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *) &cstorage;
        socklen_t caddrlen = sizeof(cstorage);
        int csock = accept(sockfd, caddr, &caddrlen); // complete connection between server and client
        if(csock == -1) logexit("accept");
        // add new user to the blog
        int newClientId = addNewUser(csock);
        printf("client 0%d connected\n", newClientId + 1);
        // send new connection response to client
        struct BlogOperation response = initBlogOperation(newClientId, NEW_CONNECTION, "", "", 1);
        size_t count = send(csock, &response, sizeof(struct BlogOperation), 0);
        if(count != sizeof(struct BlogOperation)) logexit("send");
        // create new thread for client
        if(pthread_create(&(mediumBlog.clients[newClientId].thread), NULL, clientFunction,(void*) &mediumBlog.clients[newClientId]) != 0) logexit("pthread_create");

    }
    for(int i = 0; i < MAX_USERS; i++){
        pthread_join(mediumBlog.clients[i].thread, NULL);
    }
        while (1){
            struct BlogOperation req;
            
            if(req.operation_type == 1){
                req = defining_id(req);
            }
            else if(req.operation_type == 2){
                if(!verify_topic_existence){
                    req = create_topic(req);
                    res2 = notify_participants(req);
                }
                else{
                    //enviar pro outro cliente
                    res2 = notify_participants(req);
                }
            }
            else if(req.operation_type == 3){

            }
            else if(req.operation_type == 4){
                if(verify_topic_existence(req)){
                    if(!already_subscribed(req)){
                        req = subscribe_topic(req);
                    }
                    else{
                        struct BlogOperation res;
                        res.client_id = 2;
                        res.operation_type = 4;
                        res.server_response = 0;
                        strcpy(res.topic, "");
                        strcpy(res.content, "error: already subscribed");
                    }
                }
                else{
                    req = create_topic(req);
                    req = subscribe_topic(req);
                
                }
            }
            else if(req.operation_type == 5){
                
            }
            else if(req.operation_type == 6){
                
            }




           
        
        close(csock);
    }
    return 0;
}