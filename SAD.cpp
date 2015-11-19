//
//  SAD.cpp
//  hw01
//
//  Created by sixinglu on 2/12/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#include "SAD.h"


SAD::SAD(sc_module_name nm)
{
    SC_THREAD(SAD_process);

}

void SAD::SAD_process()
{
    int i, v;
    unsigned int block =0;
    unsigned int sad =0;
    unsigned int num1 =0;
    unsigned int num2 =0;
    
    
    wait(30,SC_NS); // loop
    for (block=0; block<NUM_BLOCKS; block++)
    {
        sc_time start = sc_core:: sc_time_stamp();
        
        sad = 0;
        wait(10,SC_NS); // sad = 0;
        
        wait(30,SC_NS); // loop
        for (i=0; i<BLOCK_SIZE; i++)
        {
            wait(30,SC_NS); // for addition
            wait(40,SC_NS); // for + and *
            
            SAD_port->Request(SAD_id, MEM_start_address+INPUT1_ADDR+(block*BLOCK_SIZE)+i, READ_REQ, 1);
            wait(10,SC_NS); // check if
            if(SAD_port->WaitForAcknowledge(SAD_id)){
                SAD_port->ReadData(num1);
//                cout<<"SAD read data num1: "<<num1<<endl;
            }
            
            wait(40,SC_NS); // for + and *
            SAD_port->Request(SAD_id, MEM_start_address+INPUT2_ADDR+(block*BLOCK_SIZE)+i, READ_REQ, 1);
            wait(10,SC_NS); // check if
            if(SAD_port->WaitForAcknowledge(SAD_id)){
                SAD_port->ReadData(num2);
//                cout<<"SAD read data num2: "<<num2<<endl;
            }
            
            v= num1 - num2;
            wait(10,SC_NS);
            
            if( v < 0 ){
                v = -v;
                wait(10,SC_NS); // v=-v
            }
            
            sad += v;
            wait(10,SC_NS); // sad +=v

            
        }
        
        wait(10,SC_NS); //addition
        SAD_port->Request(SAD_id, MEM_start_address+SAD_OUTPUT_ADDR+block, WRITE_REQ, 1);
        wait(10,SC_NS); // check if
        if(SAD_port->WaitForAcknowledge(SAD_id)){
            SAD_port->WriteData(sad);
        }
        
        sc_time end = sc_core:: sc_time_stamp();
        //cout<<endl<<"execution time of SAD block "<<block<<" is: "<<end-start<<endl<<endl;
    }
    
    //sc_time end = sc_core:: sc_time_stamp();
    //cout<<"total time is: "<<end-start<<endl;
   // exit(EXIT_SUCCESS);

}