//
//  bus.cpp
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#include "bus.h"

/**************** Constructor **********************/
Bus::Bus(sc_module_name nm):sc_channel(nm)
{
    bus_status = UNLOCK;  // do not need to LOCK anymore
    timer =0;
    
    SC_THREAD(Bus_process);

}

/********************** Master interface *************************/
void Bus::Request(unsigned mst_id, unsigned addr, unsigned op, unsigned len)
{
    wait(2*CYCLE_TIME,SC_NS);  // put it in begginning, otherwise it will be scheduled before notify
    
    mst_request.req_id = mst_id;
    mst_request.address = addr;
    mst_request.option = op;
    mst_request.length =len;

    queue.push_back(mst_request);
    
    //cout<<"push request of master "<<mst_id<<" into Q"<<endl;
    
    req2bus.notify();
}

bool Bus::WaitForAcknowledge(unsigned mst_id)
{
    while(1){  // keep watching until bus ask me
        wait(ack2master);
        //if(curr_schedule!=NULL){
            if(mst_id==curr_schedule.req_id){   // if scheduled is me
                if(bus_status==ERROR){   // timeout
                    //bus_status = UNLOCK;
                    bus_unlock.notify();
                    return false;
                }
                else //if(bus_status==UNLOCK){  // acknowledged by slave
                    return true;
                //}
            }
        //}
        //wait();
    }
}

bool Bus::ReadData(unsigned &data)
{
    wait(2*CYCLE_TIME,SC_NS);
    
    data = buffer_data;
    curr_schedule.read_receive_count ++;
    
    if(curr_schedule.read_receive_count==curr_schedule.length){   // finish read
        curr_schedule.read_receive_count =0;
        bus_status=UNLOCK;
        
        DataDone.notify();   // this must be notify earier than bus_unlock, otherwise memroy cannot catch request
        
        wait(0,SC_NS);
        bus_unlock.notify();
        
        return true;
    }
    
    DataDone.notify();
    wait(0,SC_NS);
    
    return true;
}

bool Bus::WriteData(unsigned data)
{
    wait(1*CYCLE_TIME,SC_NS);
    
    buffer_data = data;
    DataReady.notify();
    
    wait(0,SC_NS);
    
    return true;
}

/********************** Slave interface *************************/
void Bus::Listen(unsigned &req_addr, unsigned &req_op, unsigned &req_len)
{
    wait(request);
    req_addr = curr_schedule.address;
    req_op = curr_schedule.option;
    req_len = curr_schedule.length;
    
}

void Bus::Acknowledge()
{
    wait(1*CYCLE_TIME,SC_NS);
    
    bus_status = LOCK;    // have issue when lock here
    timer = 0; // clean timer
    ack2bus.notify();

}

bool Bus::SendReadData(unsigned data)
{
   // read_send_count++;   // added 2015-3-30
    buffer_data = data;
    
   // if(read_send_count==curr_schedule.length){
   //     read_send_count =0;
   //     return true;
   // }
   // else{
        wait(DataDone);  // send continue
    
        return true;
   // }
}

bool Bus::ReceiveWriteData(unsigned &data)
{
    wait(DataReady);
    data = buffer_data;
    
    curr_schedule.write_recive_count ++;
    
    if(curr_schedule.write_recive_count==curr_schedule.length){
        curr_schedule.write_recive_count =0;
        bus_status=UNLOCK;
        bus_unlock.notify();
    }
    
    return true;
}

/********************** Arbitriter *************************/
void Bus::Bus_process()
{
    // at the very begging
    unsigned next_schedu_ID =1;
    if(!queue.empty()){
        curr_schedule = queue.at(0);
    }
    bool find_flag =false;
    
    while(1){
        //if(bus_status==UNLOCK){ // only schedulr new request when bus unlock
            if(!queue.empty()){
                
                while(1){  // until find a request to schedule
                    for(unsigned i=0;i<queue.size();i++){
                        if (queue.at(i).req_id==next_schedu_ID){  // find the target
                            curr_schedule = queue.at(i);
                            queue.erase(queue.begin()+i);
                            find_flag =true;
                            break;
                        }
                    }
                    next_schedu_ID =next_schedu_ID%MASTER_NUM+1;  // for Round Robin
                    if(find_flag==true){
                        find_flag = false;
                        break;
                    }
                }
            
                //cout<<"current schedule is: master "<<curr_schedule.req_id<<endl;
                bus_status=WAIT;
                
                wait(0,SC_NS); // let slave start listening
                request.notify();   // the current scheduled request is ready, search for slave
                
                sc_time AnswerTime = sc_time(10,SC_NS);
                wait(AnswerTime,ack2bus);
                if(bus_status==LOCK){    // the slave answer
                    ack2master.notify();
                    //cout<<"ack to "<<curr_schedule.req_id<<endl;
                }
                else{
                    ack2master.notify();
                    bus_status=ERROR;
                    cout<<"master "<<curr_schedule.req_id<<"cannot find "<<curr_schedule.address<<" address!"<<endl;
                }
                
                wait(bus_unlock);  // if there is transmission
            }
            else{
                //wait(0,SC_NS); // cannot use this, because it will make sw get stuck when adding wait in sw
                wait(req2bus);    // if no transmission
            }
        //}
        
    }
}
