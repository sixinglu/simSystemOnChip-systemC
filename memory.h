//
//  memory.h
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#ifndef __hw02__memory__
#define __hw02__memory__

#include <stdio.h>
#include "systemc.h"
#include "interface.h"
#include "common.h"

// memory domain a
class Memory: public sc_module
{
    
public:
    unsigned in_address;
    unsigned in_option;
    unsigned in_length;
    
    unsigned a[mem_size];
    unsigned b[mem_size];
    unsigned c[mem_size];
    
    unsigned int memData[SAD_MEM_SIZE]; // for SAD master (from hw01)
    
    //sc_in<sc_logic> Clk;
    sc_port<bus_servant_if> MEM_port;
    
    Memory(sc_module_name nm,char* memInitFilename);
    SC_HAS_PROCESS(Memory);
    
    void Mem_process();
    
    // other functions
    void SAD_mem_read(char* memInitFilename);
    
};

#endif /* defined(__hw02__memory__) */
