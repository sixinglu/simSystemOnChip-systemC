//
//  hardware.cpp
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#include "hardware.h"

/******************** Constructor ***********************/
Hardware::Hardware(sc_module_name name): sc_module(name)
{
    SAD_reg =0;
    blk1_mem_address =0;
    for(unsigned i=0; i<SIZE; i++){
        blk2_mem_address[i] =0;
    }
    blk3_mem_address =0;
    control_reg =0;
    Done_reg =0;
    
    SC_THREAD(Hardware_master_process);
    //sensitive << Clk.pos();
    
    SC_THREAD(Hardware_slave_process);
    //sensitive << Clk.pos();
}

/******************** master threads ***********************/
void Hardware::Hardware_master_process()  // for first inner loop
{
    while(1){
        wait(compute);
        SAD_reg = 0;
        Done_reg =0;
        unsigned A[control_length_reg];
        unsigned B[control_length_reg];
        unsigned C=0;
        
        // ReadBlk1
        HW_master_port->Request(HW_id, blk1_mem_address, READ_REQ, control_length_reg);
        if(HW_master_port->WaitForAcknowledge(HW_id)){  // if address is valid
            for(int kk=0;kk<control_length_reg;kk++){   // burstly ReadBlk1
                HW_master_port->ReadData(A[kk]);
//                cout<<"read data A["<<kk<<"] ="<<A[kk]<<endl;
            }
        }
        
        // ReadBlk2
        for(unsigned kk=0; kk<control_length_reg; kk++){
            HW_master_port->Request(HW_id, blk2_mem_address[kk], READ_REQ, 1);
            if(HW_master_port->WaitForAcknowledge(HW_id)){  // if address is valid
                HW_master_port->ReadData(B[kk]);
//                cout<<"read data B["<<kk<<"] ="<<B[kk]<<endl;
            }
        }
        
        // ReadBlk3
        HW_master_port->Request(HW_id, blk3_mem_address, READ_REQ, 1);
        if(HW_master_port->WaitForAcknowledge(HW_id)){  // if address is valid
            HW_master_port->ReadData(C);
        }
        
        // computation
        for(unsigned kk=0; kk<control_length_reg; kk++){
            C = C + A[kk]*B[kk];
        }
        SAD_reg = C;
        
        // write back
        HW_master_port->Request(HW_id, blk3_mem_address, WRITE_REQ, 1);
        if(HW_master_port->WaitForAcknowledge(HW_id)){  // if address is valid
            HW_master_port->WriteData(C);
        }
        
        Done_reg = cal_done_flag;
        
    }

}

/******************** slave threads ***********************/
void Hardware::Hardware_slave_process()  // basically it is the interface between reg and SAD
{
    while(1){
        if(Rst==1){
            SAD_reg =0;
            in_length =0;
            in_address =0;
            in_option =0;
        }
        else{
            
            // listen for request
            HW_slave_port->Listen(in_address, in_option, in_length);
            if(in_address>=hw_start_address && in_address<=hw_end_address){
                
                HW_slave_port->Acknowledge();  // lock bus
                if(in_option==READ_REQ){  // read from regs
                    //wait(); // otherwise, wait(DataReady) will be call after DataReady.notify
                    if(in_address==SAD_reg_address){  // only been read
                        HW_slave_port->SendReadData(SAD_reg);
                    }
                    else if(in_address==Done_reg_address){   // only been read
                        HW_slave_port->SendReadData(Done_reg);
                    }
                }
                else if(in_option==WRITE_REQ){ // write to regs
                    if(in_address==CONTROL_address){  // only been write
                        HW_slave_port->ReceiveWriteData(control_reg);
//                        cout<<"write control_reg"<<endl;
                        if(control_reg==exec_flag){
                            compute.notify();
                        }
                    }
                    else if(in_address==CONTROL_Length_address){
                        HW_slave_port->ReceiveWriteData(control_length_reg);
//                        cout<<"write control_length_reg"<<endl;
                    }
                    else if (in_address==block1_address){  // only been write
                        HW_slave_port->ReceiveWriteData(blk1_mem_address);
//                        cout<<"write blk1_mem_address"<<endl;
                    }
                    else if(in_address==block2_address){  // only been write
                        for(unsigned kk=0; kk< in_length; kk++){
                            HW_slave_port->ReceiveWriteData(blk2_mem_address[kk]);
                        }
//                        cout<<"write blk2_mem_address"<<endl;
                    }
                    else if(in_address==block3_address){  // only been write
                        HW_slave_port->ReceiveWriteData(blk3_mem_address);
//                        cout<<"write blk3_mem_address= "<<blk3_mem_address-c_start_address<<endl;
                    }
                    //cout<<"finish ReceiveWriteData."<<endl;
                }
                
            } // end if hw address range

        } // end else Rst!=1

    } // end while(1)

}


/*************** Custermized functions ****************/
unsigned Hardware::ABS_diff(unsigned A, unsigned B)
{
    if(A>B){
        return A-B;
    }
    else{
        return B-A;
    }
}

unsigned Hardware::Addition(unsigned A, unsigned B)
{
    return A+B;
}

unsigned Hardware::multiplication(unsigned A, unsigned B)
{
    return A*B;
}


