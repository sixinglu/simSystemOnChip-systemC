//
//  hardware.h
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#ifndef __hw02__hardware__
#define __hw02__hardware__

#include <stdio.h>
#include "systemc.h"
#include "common.h"
#include "interface.h"

// for another inner loop
//enum HW_state {
//    Wait = 0,
//    ReadBlk1Init,
//    ReadBlk2Init,
//    ReadBlk1,
//    ReadBlk2,
//    Comp,
//    timeout
//};

enum HW_state {
    Wait = 0,
    WriteBlkInit,
    WriteBlk,
    timeout
};


class Hardware: public sc_module
{
public:
    //sc_in<sc_logic> Clk;
    sc_in<sc_logic> Rst;
    
    sc_port<bus_master_if> HW_master_port;
    sc_port<bus_servant_if> HW_slave_port;
    sc_event compute;
    
    // for master
    HW_state State;
    unsigned control_reg; // reg0: ctrol start
    unsigned Done_reg; // reg1: tell sw cal is done
    unsigned SAD_reg;  // reg2: comp result reg
    unsigned control_length_reg; //reg3: how long the hw will deal with
    unsigned blk1_mem_address;  // reg4: A
    unsigned blk2_mem_address[SIZE];  // reg5 array: B[]
    unsigned blk3_mem_address;
    
    // for slave
    unsigned in_address;
    unsigned in_option;
    unsigned in_length;
    
    Hardware(sc_module_name name);
    
    SC_HAS_PROCESS(Hardware);
    void Hardware_master_process();
    void Hardware_slave_process();
    
    unsigned ABS_diff(unsigned A, unsigned B);
    unsigned Addition(unsigned A, unsigned B);
    unsigned multiplication(unsigned A, unsigned B);
    
};

#endif /* defined(__hw02__hardware__) */
