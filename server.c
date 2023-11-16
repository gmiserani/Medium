#include "common.h"

int id[10];
struct Blog blog;
void init_all(){
    for(int i =0; i< 10; i++){
    id[i] = 0;
    }
    blog.list_clients_count = 0;
    blog.list_topics_count = 0;
}



void* clientFunction(void* clientThread)
{
    struct Client *clientThreadPtr = (struct Client*) clientThread;
    int sockfd = clientThreadPtr->sock;
    struct BlogOperation req;
    while(true){
        int count = receive_all(sockfd, &req, sizeof(struct BlogOperation));
        if(count == 0){
            req.operation_type = 5;
        }
        // handle client req
        struct BlogOperation res;
        res.client_id = req.client_id;
        res.operation_type = 0;
        res.server_response = 0;
        strcpy(res.topic, "");
        strcpy(res.content, "");
        





        if(req.client_id != 0){
            //REFAZER ESSEEEEEE
            if(req.operation_type == 2){
                if(!verify_topic_existence){
                    reate_topic(req);
                    res = notify_participants(req);
                }
                else{
                    //enviar pro outro cliente
                    res2 = notify_participants(req);
                }
            }








            else if(req.operation_type == 3){
                char * topics = malloc(sizeof(char) * 2048);
                res.operation_type = 3;
                res.server_response = 1;
                if(blog.list_topics_count == 0){
                    strcpy(topics, "no topics available");
                }else{
                    for(int i = 0; i < blog.list_topics_count; i++){
                        strcat(topics, blog.list_topics[i].topic);
                        strcat(topics, "\n");
                    }
                }
                strcpy(res.content, topics);
            }
            else if(req.operation_type == 4){
                if(verify_topic_existence(req)){
                    if(!already_subscribed(req)){
                        subscribe_topic(req);
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
                    create_topic(req);
                    subscribe_topic(req);
                }
            }
            else if(req.operation_type == 5){
                userExit(req);
                printf("client 0%d was disconnected\n", req.client_id);
            }
            else if(req.operation_type == 6){
                if(already_subscribed(req)){
                    unsubscribe_topic(req);
                }
                else{
                    struct BlogOperation res;
                    res.client_id = 2;
                    res.operation_type = 6;
                    res.server_response = 1;
                    strcpy(res.topic, "");
                    strcpy(res.content, "error: not subscribed");
                }   
                

            }
        }
        else printf("error: client not connected");
    
        // send resp to client
        if(res.server_response != -1){
            size_t count_bytes_sent = send(sockfd, &res, sizeof(struct BlogOperation), 0);
            if(count_bytes_sent != sizeof(struct BlogOperation)) logexit("send");
        }
        // if client wants to exit, close connection
        if(req.operation_type == 5){
            close(sockfd);
            break;
        }
    }
    pthread_exit(EXIT_SUCCESS);
}

void userExit(struct BlogOperation operation){
    sem_wait(&semaphore);
    struct client client;
    client = blog.list_clients[operation.client_id];
    for(int i = 0; i < blog.list_topics_count; i++){
        if(blog.list_topics[i].clients[operation.client_id] == 1){
            blog.list_topics[i].clients[operation.client_id] = 0;
            blog.list_topics[i].client_count--;
        }
    }
    id[operation.client_id] = 0;
    blog.list_clients_count--;
    sem_post(&semaphore);
}


int defining_id(){
    for(int i=0; i < 10; i++){
        if(id[i] == 0){
            id[i] = 1;
            int client_id = i+1;
            return client_id;
        }
    }
}

int verify_topic_existence(struct BlogOperation operation){
    struct topic subscribed_topic;
    strcpy(subscribed_topic.topic, operation.topic);
    int i = 0; 
    for(int i=0; i<blog.list_topics_count;i++){
        if(strcmp(blog.list_topics[i].topic, subscribed_topic.topic) == 0){
            return 1;
        }
    }
    return 0;
}


void create_topic(struct BlogOperation operation){
    struct topic new_topic;
    strcpy(new_topic.topic, operation.topic);
    //inicializa lista de clientes do novo topico
    for(int i =0; i <10; i++){
        new_topic.clients[i] = 0;
    }
    //inscreve o cliente que chamou o topico
    new_topic.clients[operation.client_id] = 1;
    //APAGAR
    new_topic.owner = operation.client_id;
    //inicializa numero de clientes inscritos
    new_topic.client_count = 1;
    //adiciona topico na lista de topicos do blog
    int tam = blog.list_topics_count;
    new_topic.topic_id = tam;
    blog.list_topics[tam] = new_topic;
    blog.list_topics_count++;
}

void subscribe_topic(struct BlogOperation operation){
    struct topic subscribed_topic;
    strcpy(subscribed_topic.topic, operation.topic);
    int i = 0;
    for(int i =0; i < blog.list_topics_count; i++){
        if(strcmp(blog.list_topics[i].topic, subscribed_topic.topic) == 0){
            subscribed_topic = blog.list_topics[i];
            int subscribed_client_id = operation.client_id;
            //coloca o cliente como inscrito no topico
            subscribed_topic.clients[subscribed_client_id] = 1;
            //aumenta o numero de pessoas inscritas no topico
            subscribed_topic.client_count++;
            //atualiza tudo no blog
            blog.list_topics[i] = subscribed_topic;
        }
    }
}

void unsubscribe_topic(struct BlogOperation operation){
    struct topic subscribed_topic;
    strcpy(subscribed_topic.topic, operation.topic);
    int i = 0;
    for(int i =0; i < blog.list_topics_count; i++){
        if(strcmp(blog.list_topics[i].topic, subscribed_topic.topic) == 0){
            subscribed_topic = blog.list_topics[i];
            int subscribed_client_id = operation.client_id;
            subscribed_topic.clients[subscribed_client_id] = 0;
            subscribed_topic.client_count--;
            blog.list_topics[i] = subscribed_topic;
        }
    }
}

struct BlogOperation notify_participants(struct BlogOperation operation_client){
    struct BlogOperation operation;
    operation.client_id = operation_client.client_id;
    operation.operation_type = 2;
    operation.server_response = 1;
    strcpy(operation.topic, operation_client.topic);
    strcpy(operation.content, operation_client.content);
    return operation;
}

int already_subscribed(struct BlogOperation operation_client){
    struct topic subscribed_topic;
    strcpy(subscribed_topic.topic, operation_client.topic);
    int i = 0; 
    for(int i =0; i < blog.list_topics_count; i++){
        //pega o topico da lista de topicos do blog
        if(strcmp(blog.list_topics[i].topic, subscribed_topic.topic) == 0){
            //verifica se aquele cliente ja esta inscrito
            if(subscribed_topic.clients[operation_client.client_id] == 1){
                return 1;
            }
        }
    }
    return 0;
}

struct client createClient(int sock){
    sem_wait(&semaphore);
    struct client connected_client;
    int connected_client_id = defining_id();
    connected_client.client_id = connected_client_id;
    connected_client.socket = sock;
    blog.list_clients[connected_client_id] = connected_client;
    blog.list_clients_count++;
    sem_post(&semaphore);
    return connected_client;
}

int main(int argc, char *argv[]){
    char * ip = argv[1];
    char * port = argv[2];
    sem_init(&semaphore, 0, 1); // initialize semaphore
    init_all(); 
    struct sockaddr_storage storage;
    if(server_sockaddr_init(ip, port, &storage)) logexit("serverSockaddrInitt");
    // initialize socket
    int sockfd = socket(storage.ss_family, SOCK_STREAM, 0);
    if(sockfd == -1) logexit("socket");
    // reuse address
    int enable = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) != 0) logexit("setsockopt");
    // bind to address
    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if(bind(sockfd, addr, sizeof(storage)) != 0) logexit("bind");
    // listen for connections
    if(listen(sockfd, 10) != 0) logexit("listen");

    while(1){
        // accept connection
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *) &cstorage;
        socklen_t caddrlen = sizeof(cstorage);
        int csock = accept(sockfd, caddr, &caddrlen); // complete connection between server and client
        if(csock == -1) logexit("accept");

        // add new user to the blog
        struct client newClient = createClient(csock);

        printf("client 0%d connected\n", newClient.client_id);
        // send new connection response to client

        struct BlogOperation resp;
        resp.client_id = newClient.client_id;
        resp.operation_type = 1;
        resp.server_response = 1;
        strcpy(resp.topic, "");
        strcpy(resp.content, "");
        
        size_t count = send(csock, &resp, sizeof(struct BlogOperation), 0);
        if(count != sizeof(struct BlogOperation)) logexit("send");
        // create new thread for client
        if(pthread_create(&(blog.list_clients[newClient.client_id].thread), NULL, clientFunction,(void*) &blog.list_clients[newClient.client_id])!=0){
            logexit("pthread_create");
        }

    }
    for(int i = 0; i < 10; i++){
        pthread_join(blog.list_clients[i].thread, NULL);
    }
    return 0;
}