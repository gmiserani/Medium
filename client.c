#include "common.h"

struct BlogOperation init_new_client(struct BlogOperation operation){
    operation.client_id = 0;
    operation.operation_type = 1;
    operation.server_response = 0;
    strcpy(operation.topic, "");
    strcpy(operation.content, "");
    return operation;
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