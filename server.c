#include "common.h"

int id = 0;



struct BlogOperation defining_id(struct BlogOperation operation){
    id++;
    operation.client_id = 1;
    operation.server_response = 1;
    return operation;
}

struct BlogOperation create_new_topic(struct BlogOperation operation){
    
}

int main(int argc, char *argv[]){
 

    // Accept
    while(1){
        while (1){
            struct BlogOperation req;
            
            if(req.operation_type == 1){
                req = defining_id(req);
            }
            else if(req.operation_type == 2){

            }
            else if(req.operation_type == 3){

            }
            else if(req.operation_type == 4){
                int existe = check_new_topic(req)
            }
            else if(req.operation_type == 5){
                
            }
            else if(req.operation_type == 6){
                
            }




           
        }
        close(csock);
    }
    return 0;
}