//
//  SAD.h
//  hw01
//
//  Created by sixinglu on 2/12/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#include "systemc.h"
#include <stdlib.h>
#include "interface.h"
#include "common.h"

#ifndef SAD_H
#define SAD_H

#define NUM_BLOCKS 64
#define BLOCK_SIZE 64
#define INPUT1_ADDR 0
#define INPUT2_ADDR 16384
#define SAD_OUTPUT_ADDR 32768


using namespace std;
using namespace sc_core;


class SAD: public sc_module{
    
public:
    
    //unsigned SAD_buffer1;
    //unsigned SAD_buffer2;

    sc_port<bus_master_if> SAD_port;
    
    SAD(sc_module_name nm);
    
    SC_HAS_PROCESS(SAD);
    void SAD_process();
    
};


#endif