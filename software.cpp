//
//  software.cpp
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#include "software.h"
#include <chrono>

Software::Software(sc_module_name nm){
    
    for(unsigned i=0; i<SIZE; i++){
        for(unsigned j=0; j<SIZE; j++){
            a[i][j] =0;
            b[i][j] =0;
            c[i][j] =0;
        }
    }
    SC_THREAD(Software_process);
    result =0;
}

void Software::Software_process() // Total Cycles: 8193437
{
//    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    int n;
    int i,j;
    
    sc_time start = sc_core:: sc_time_stamp();
    double consume_time;
    
    
    consume_time = (8193437-8186006)*CYCLE_TIME;
    
    for (n = 0 ; n < LOOPS ; n++) // Total Cycles: 8186006, Execs: 1, Iters: 1000
    {
        consume_time =(8186006-579000-7579000)/1000*CYCLE_TIME;
        wait(consume_time,SC_NS);
        
        //---------------------------  done by sw  --------------------------------------//

        for(i=0;i<SIZE;i++){ // Total Cycles: 579000, Execs: 1000, Iters: 5
            
            consume_time =(579000-520000)/5000*CYCLE_TIME;
            wait(consume_time,SC_NS);
            
            //for(j=0;j<SIZE;j++) // Total Cycles: 520000, Execs: 5000, Iters: 5
            //c[i][j] = 0;
            
            unsigned c_address = i*SIZE+0  +c_start_address;
            unsigned control_length =SIZE;
            
            SW_port->Request(SW_id, c_address, WRITE_REQ, control_length);
            //wait(2*15,SC_NS);
            if(SW_port->WaitForAcknowledge(SW_id)){
                for(int kk=0;kk<control_length;kk++){
                    SW_port->WriteData(0);
                }
            }
        }
        
        
 //       cout<<"--------------- sw part done -----------"<<endl;
        
        
        //---------------------------  done by hw  --------------------------------------//
        
        for(i=0;i<SIZE;i++){ // Total Cycles: 7579000, Execs: 1000, Iters: 5
            
            consume_time = (7579000-7520000)/5000*CYCLE_TIME;
            wait(consume_time,SC_NS);
            
            for(j=0;j<SIZE;j++){ // Total Cycles: 7520000, Execs: 5000, Iters: 5
                
                consume_time = (7520000-7225000)/25000*CYCLE_TIME;
                wait(consume_time,SC_NS);
                
                //for(k=0;k<SIZE;k++){ // Total Cycles: 7225000, Execs: 25000, Iters: 5
                    
                    // c[i][j] += a[i][k] * b[k][j];
                
                // give blk1 address
                unsigned a_address;
                unsigned len_data=1;     // start address is enough, because it can burst
                
                SW_port->Request(SW_id,block1_address, WRITE_REQ, len_data); // write to blk1 reg = A
                if( SW_port->WaitForAcknowledge(SW_id) ){
                    for(int kk=0;kk<len_data;kk++){
                        a_address =i*SIZE+kk +a_start_address;
                        SW_port->WriteData(a_address);  // write the target address as content to hw
                    }
                }
                
                // give blk2 address
                unsigned b_address;
                len_data=SIZE;     // all address should be fed into hw, because address in mem not continous
                SW_port->Request(SW_id,block2_address, WRITE_REQ, len_data);   // write to blk2 reg = B[]
                if( SW_port->WaitForAcknowledge(SW_id) ){
                    for(int kk=0;kk<len_data;kk++){
                        b_address =kk*SIZE+j + b_start_address;
                        SW_port->WriteData(b_address);
                    }
                }
                
                // give blk3 address
                unsigned start_c =i*SIZE+j +c_start_address;
                SW_port->Request(SW_id,block3_address, WRITE_REQ, 1);
                if( SW_port->WaitForAcknowledge(SW_id) ){
                    SW_port->WriteData(start_c);
                }
                
                SW_port->Request(SW_id,CONTROL_Length_address, WRITE_REQ, 1);
                if( SW_port->WaitForAcknowledge(SW_id) ){
                    SW_port->WriteData(len_data);
                }
                
                // let hardware start execute
                SW_port->Request(SW_id, CONTROL_address, WRITE_REQ, 1);
                if(SW_port->WaitForAcknowledge(SW_id)){
                    SW_port->WriteData(exec_flag);
                }
                
                // wait for hw calculation
                unsigned flag =0;
                while(flag !=cal_done_flag){ // if not finish
                    SW_port->Request(SW_id,Done_reg_address, READ_REQ, 1);
                    if(SW_port->WaitForAcknowledge(SW_id)){
                        SW_port->ReadData(flag);  // slave will unlock bus
//                        cout<<"read done_flag is: "<<flag<<endl;
                    }
                    wait(0,SC_NS);
                }
 //               cout<<"------- hw calculation done! -------"<<endl<<endl;
                    
                //}
            }
        }
    }
    
    sc_time end = sc_core:: sc_time_stamp();
    cout<<endl<<"the time consume by sw-hw is:"<<end-start<<endl<<endl;
   
//    sc_time PureSwTime = pure_sw();
//    cout<<"the time consume by pure sw is:"<<PureSwTime<<endl;
//    cout<<"the speedup is: "<<PureSwTime/(end-start)<<endl;
    
//    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
//    cout<<"consum CPU time: "<<duration<<"m"<<endl;
    
    exit(EXIT_SUCCESS);
    return;
}

sc_time Software::pure_sw() // Total Cycles: 8193437
{
    int n;
    int i,j,k;
    sc_time start = sc_core:: sc_time_stamp();
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    for (n = 0 ; n < LOOPS ; n++) // Total Cycles: 8186006, Execs: 1, Iters: 1000
    {
        for(i=1;i<=SIZE;i++) // Total Cycles: 579000, Execs: 1000, Iters: 5
            for(j=1;j<=SIZE;j++) // Total Cycles: 520000, Execs: 5000, Iters: 5
                c[i][j] = 0;
        
        for(i=1;i<=SIZE;i++) // Total Cycles: 7579000, Execs: 1000, Iters: 5
            for(j=1;j<=SIZE;j++) // Total Cycles: 7520000, Execs: 5000, Iters: 5
                for(k=1;k<=SIZE;k++) // Total Cycles: 7225000, Execs: 25000, Iters: 5
                    c[i][j] += a[i][k] * b[k][j];
    }
    
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    cout<<"consum CPU time: "<<duration<<"m"<<endl;
    
    wait(CYCLE_TIME*8193437,SC_NS);   // 150Hz
    
    sc_time end = sc_core:: sc_time_stamp();
    
    return end-start;
}