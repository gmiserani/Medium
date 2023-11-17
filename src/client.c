#include "common.h"

pthread_t wait_t;

struct BlogOperation init_new_client(){
    struct BlogOperation operation;
    operation.client_id = 0;
    operation.operation_type = 1;
    operation.server_response = 0;
    strcpy(operation.topic, "");
    strcpy(operation.content, "");
    return operation;
}


void* waitForResponse(void* sock){
    int* sockfd = (int*) sock;
    while(true){
        struct BlogOperation resp;
        receive_all(*sockfd, &resp, sizeof(struct BlogOperation));
        if(resp.operation_type == 3){
            printf("%s\n", resp.content);
        }
        else if(resp.operation_type == 2){
            printf("new post added in %s by 0%d\n%s", resp.topic, resp.client_id, resp.content);
        }
        else if(resp.operation_type == 5){
            printf("exit\n");
        }
        else if(resp.operation_type == -1){
            printf("%s\n", resp.content);
        }
        if(resp.operation_type == 5){
            close(*sockfd);
            break;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv){
    char* ip = argv[1];
    char* port = argv[2];
    struct sockaddr_storage storage;
    if(addrparse(ip, port, &storage) != 0) logexit("addrparse");
    // inicialize socket
    int sockfd = socket(storage.ss_family, SOCK_STREAM, 0);
    if(sockfd == -1) logexit("socket");
    // inicialize connection
    struct sockaddr *addr = (struct sockaddr *)(&storage); 
    if(connect(sockfd, addr, sizeof(storage)) != 0) logexit("connect");
   

    struct BlogOperation client_operation;
    client_operation = init_new_client();
    
    int count = send(sockfd, &client_operation, sizeof(client_operation), 0); // send client_operation to server
    if(count != sizeof(client_operation)) logexit("send");

    struct BlogOperation res;
    receive_all(sockfd, &res, sizeof(res)); // receive server response
    if(res.server_response != 1){
        printf("Client %02d could not connect to server\n", res.client_id);
        exit(EXIT_FAILURE);
    }
    pthread_create(&wait_t, NULL, &waitForResponse, (void*) &sockfd);
    
    char posted[2048];
    while(1){
        fgets(posted, 2048, stdin);
        posted[strlen(posted) - 1] = '\0';
        char *temp = malloc(sizeof(char) * 2048);
        strcpy(temp, posted);
        char * cmd = strtok(temp, " ");
        char *content = " ";

        if(strcmp(posted, "exit") == 0){
            res.operation_type = 5;
            count = send(sockfd, &res, sizeof(res), 0); // send res to server
            if(count != sizeof(res)) logexit("send");
            break;
        }
        else if(strcmp(cmd, "subscribe") == 0){
            strcpy(temp, cmd);
            content = temp + 10;
            strcpy(res.topic, content);
            res.operation_type = 4;
            count = send(sockfd, &res, sizeof(res), 0); // send res to server
            if(count != sizeof(res)) logexit("send");
        }
        else if(strcmp(cmd, "unsubscribe") == 0){
            strcpy(temp, cmd);
            content = temp + 15;
            strcpy(res.topic, content);
            res.operation_type = 6;
            count = send(sockfd, &res, sizeof(res), 0); // send res to server
            if(count != sizeof(res)) logexit("send");
        }
        else if(strcmp(cmd, "publish") == 0){
            strcpy(temp, cmd);
            content = temp + 11;
            fgets(posted, 2048, stdin);
            char publish[2048];
            strcpy(publish, posted);
            strcpy(res.content, publish);
            strcpy(res.topic, content);
            res.operation_type = 2;            
            count = send(sockfd, &res, sizeof(res), 0); // send res to server
            if(count != sizeof(res)) logexit("send");
        }
        else if(strcmp(posted, "list topics") == 0){
            res.operation_type = 3;
            count = send(sockfd, &res, sizeof(res), 0); // send res to server
            if(count != sizeof(res)) logexit("send");
        }
        else{
            printf("Invalid command\n");
        }
        
    }

}