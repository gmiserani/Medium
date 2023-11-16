#include "common.h"

struct BlogOperation init_new_client(struct BlogOperation operation){
    operation.client_id = 0;
    operation.operation_type = 1;
    operation.server_response = 0;
    strcpy(operation.topic, "");
    strcpy(operation.content, "");
    return operation;
}

struct BlogOperation add_content(struct BlogOperation operation){
    struct topic subscribed_topic;
    subscribed_topic.topic = operation.topic;
    int i = 0;
    while(blog.list_topics[i] != NULL){
        if(strcmp(blog.list_topics[i].topic, subscribed_topic.topic) == 0){
            subscribed_topic = blog.list_topics[i]
            int client_numb = subscribed_topic.client_count;
            subscribed_topic.client[client_numb] = operation.client_id;
            client_numb++;
            subscribed_topic.client_count = client_numb;
            blog.list_topics[i] = subscribed_topic;
        }
        i++
    }
    blog.list_topics[i] = operation.topic;
    blog.list_topics[i+1] = operation.content;
}


int main(int argc, char **argv){
    char* protocol = argv[1];
    char* gate = argv[2];
    
   

    struct BlogOperation client_opration;
    //client_opration = init_new_client(client_opration);
    

    while(1){
        if()
        else if(client_opration.operation_type == )
        
    }

}