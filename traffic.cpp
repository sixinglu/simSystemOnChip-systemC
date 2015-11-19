//
//  traffic.cpp
//  hw02
//
//  Created by sixinglu on 3/30/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#include "traffic.h"

Traffic::Traffic(sc_module_name nm, unsigned Period)
{
    period = Period;
    
    SC_THREAD(traffic_process);

}

void Traffic::traffic_process()
{
    while(1){
        
        wait(period, SC_NS);
        unsigned rand_size = rand()%33 + 32;
        unsigned temp_result;
        
        traffic_port->Request(Traff_id, MEM_start_address, READ_REQ, rand_size);
        //wait(10,SC_NS); // check if
        if(traffic_port->WaitForAcknowledge(Traff_id)){
            for(unsigned kk=0; kk<rand_size; kk++){
                traffic_port->ReadData(temp_result);
            }
        }
        
    }
}