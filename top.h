//
//  top.h
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#ifndef __hw02__top__
#define __hw02__top__

#include <stdio.h>
#include "systemc.h"
#include "software.h"
#include "hardware.h"
#include "bus.h"
#include "memory.h"
#include "SAD.h"
#include "traffic.h"
#include "common.h"
#include "interface.h"

class Top: public sc_module
{
    
public:
    
    //sc_event ACK2SAD;
    
    //sc_signal<sc_logic> Clk;
    sc_signal<sc_logic> Rst;

    Software* sw_obj;
    Hardware* hw_obj;
    Bus* bus_obj;
    Memory* mem_obj;
    SAD* SAD_obj;
    Traffic* traff_obj;
    
    SC_HAS_PROCESS(Top);
    
    Top(sc_module_name nm, char* memInitFilename, unsigned period);
    
    //void oscillator();
    
};

#endif /* defined(__hw02__top__) */
