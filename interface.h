//
//  interface.h
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#include "systemc.h"

#ifndef INTERFACE_H
#define INTERFACE_H

// Bus Master Interface
class bus_master_if : virtual public sc_interface
{
public:
    virtual void Request(unsigned mst_id, unsigned addr, unsigned op, unsigned len) = 0;
    virtual bool WaitForAcknowledge(unsigned mst_id) = 0;
    virtual bool ReadData(unsigned &data) = 0;
    virtual bool WriteData(unsigned data) = 0;
    //virtual void Finish_read()=0;
};


// Bus Servant Interface
class bus_servant_if : virtual public sc_interface
{
public:
    virtual void Listen(unsigned &req_addr, unsigned &req_op, unsigned &req_len) = 0;
    virtual void Acknowledge() = 0;
    virtual bool SendReadData(unsigned data) = 0;
    virtual bool ReceiveWriteData(unsigned &data) = 0;
    //virtual void Finish_receive()=0;
};


#endif
