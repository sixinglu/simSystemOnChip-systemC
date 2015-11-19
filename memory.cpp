//
//  memory.cpp
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#include "memory.h"

Memory::Memory(sc_module_name nm, char* memInitFilename){

    unsigned int temp1[mem_size]={
        0,0,0,0,0,
        0,0,0,9,4,
        7,9,0,12,14,
        15,16,11,0,2,
        3,4,5,6,0,
        4,3,2,1,2,
        0,2,7,6,4,
        9 };
    
    unsigned int temp2[mem_size]={
        0,0,0,0,0,
        0,0,0,9,4,
        7,9,0,12,14,
        15,16,11,0,2,
        3,4,5,6,0,
        4,3,2,1,2,
        0,2,7,6,4,
        9 };
    
    for (int i=0;i<mem_size;i++){
        a[i] = temp1[i];
        b[i] = temp2[i];
        c[i] =0;
    }
    
    SAD_mem_read(memInitFilename);
    
    SC_THREAD(Mem_process);
    //sensitive << Clk.pos();
}

void Memory::SAD_mem_read(char* memInitFilename)
{
    ifstream fp_in;
    fp_in.open(memInitFilename);
    if (!fp_in.is_open()){
        cout<<"cannot open the file! \n"<<endl;
        return;
    }
    std::string temp;
    unsigned int i =0;
    while(fp_in>>temp){
        unsigned value;
        std::stringstream str_value;
        str_value<<temp;
        str_value >> value;
        memData[i] =value;
        i++;
    }
    
    fp_in.close();
    
    // if initial file is not large enough
    if(i<SAD_MEM_SIZE){
        for(;i<SAD_MEM_SIZE;i++){
            memData[i] =0;
        }
    }
}

void Memory::Mem_process(){
    while(1){
        
        // listen the request
        MEM_port->Listen(in_address, in_option, in_length);
        
        // verify the address range
        if(in_address>=a_start_address && in_address<=a_end_address){
            MEM_port->Acknowledge();  // tell bus find the address, can start transfer
            if(in_option==READ_REQ){
               // wait();   // otherwise, wait(DataReady) will be call after DataReady.notify
                for(int i=0;i<in_length;i++){  // send burst data to master
                    MEM_port->SendReadData(a[in_address+i]);
                }
            }
            else if(in_option==WRITE_REQ){  // write request
                for(int i=0;i<in_length;i++){  // send burst data to master
                    MEM_port->ReceiveWriteData(a[in_address+i]);
//                    cout<<"receive write data a["<<in_address+i<<"] is: "<<a[in_address+i]<<endl;
                }
            }
        }
        else if(in_address>=b_start_address && in_address<=b_end_address){
            MEM_port->Acknowledge();
            if(in_option==READ_REQ){
              //  wait(); // otherwise, wait(DataReady) will be call after DataReady.notify
                for(int i=0;i<in_length;i++){  // send burst data to master
                    MEM_port->SendReadData(b[in_address-b_start_address+i]);
                }
            }
            else if(in_option==WRITE_REQ){  // write request
                for(int i=0;i<in_length;i++){  // send burst data to master
                    MEM_port->ReceiveWriteData(b[in_address-b_start_address+i]);
//                    cout<<"receive write data b["<<in_address-b_start_address+i<<"] is: "<<b[in_address-b_start_address+i]<<endl;
                }
            }
        }
        else if(in_address>=c_start_address && in_address<=c_end_address){
            MEM_port->Acknowledge();
            if(in_option==READ_REQ){
                for(int i=0;i<in_length;i++){  // send burst data to master
                    MEM_port->SendReadData(c[in_address-c_start_address+i]);
                }
            }
            else if(in_option==WRITE_REQ){  // write request
                for(int i=0;i<in_length;i++){  // send burst data to master
                    MEM_port->ReceiveWriteData(c[in_address-c_start_address+i]);
//                    cout<<"receive write data c["<<in_address-c_start_address+i<<"] is: "<<c[in_address-c_start_address+i]<<endl;
                }
            }
        }
        else if(in_address>=MEM_start_address && in_address<=MEM_end_address){
            MEM_port->Acknowledge();
            if(in_option==READ_REQ){
                for(int i=0;i<in_length;i++){  // send burst data to master
                    MEM_port->SendReadData(memData[in_address-MEM_start_address+i]);
                }
            }
            else if(in_option==WRITE_REQ){  // write request
                for(int i=0;i<in_length;i++){  // send burst data to master
                    MEM_port->ReceiveWriteData(memData[in_address-MEM_start_address+i]);
//                    cout<<"receive write data memData["<<in_address-MEM_start_address+i<<"] is: "<<memData[in_address-MEM_start_address]<<endl;
                }
            }
        }
        
    }
}
