//
//  bus.h
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#ifndef __hw02__bus__
#define __hw02__bus__

#include <stdio.h>
#include <deque>
#include "systemc.h"
#include "interface.h"
#include "common.h"

using namespace std;

/*  module      ID  */
/* software:     1  */
/* ALU:          2  */
/* Monitor:      3  */
/* Memory:       4  */

//enum Tranfer_status {
//               TRAN_READY = 0,
//               TRAN_RECEIVE,
//			   TRAN_ERROR };

enum Bus_lock_status {
    UNLOCK = 0,
    WAIT,
    LOCK,
    ERROR
};

class Bus_request
{
public:

    unsigned req_id;
    unsigned address;
    unsigned option;
    unsigned length;
    
    unsigned read_receive_count;
    unsigned write_recive_count;

    
    // default constructor
    Bus_request()
    {
        read_receive_count =0;
        write_recive_count =0;

        req_id =0;
        address =1234;
        option =0;
        length =0;
    }
    
};

class Bus:public sc_module, public bus_master_if, public bus_servant_if
{
    
public:
    
    //sc_in<sc_logic> Clk;
    
    sc_event request;
    sc_event DataReady;
    sc_event DataDone;
    sc_event ack2bus;
    sc_event ack2master;
    sc_event bus_unlock;
    sc_event req2bus;
    
    Bus_request mst_request;   // request coming
    Bus_request curr_schedule;  // current scheduled request
    Bus_lock_status bus_status;  // lock or unlock
    unsigned timer;
    
//    unsigned read_receive_count;
//    unsigned write_recive_count;
    //unsigned read_send_count;
    //unsigned write_send_count;
    
    std::deque<Bus_request> queue;
    unsigned buffer_data;
    
    Bus(sc_module_name nm);

    virtual void Request(unsigned mst_id, unsigned addr, unsigned op, unsigned len);
    virtual bool WaitForAcknowledge(unsigned mst_id);
    virtual bool ReadData(unsigned &data);
    virtual bool WriteData(unsigned data);
    
    virtual void Listen(unsigned &req_addr, unsigned &req_op, unsigned &req_len);
    virtual void Acknowledge();
    virtual bool SendReadData(unsigned data);
    virtual bool ReceiveWriteData(unsigned &data);
    
    SC_HAS_PROCESS(Bus);
    void Bus_process();
    
    //void Timeout();
    
};


#endif /* defined(__hw02__bus__) */
