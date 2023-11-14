#include "common.h"

int id = 0;

struct BlogOperation defining_id(struct BlogOperation operation){
    id++;
    operation.operation_type = 1;
    operation.server_response = 1;
    return operation;
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
            else if()




           
        }
        close(csock);
    }
    return 0;
}