//
//  software.h
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#ifndef __hw02__software__
#define __hw02__software__

#include <stdio.h>
#include "systemc.h"
#include "interface.h"
#include "common.h"

class Software: public sc_module
{
public:
    unsigned a[SIZE][SIZE];
    unsigned b[SIZE][SIZE];
    unsigned c[SIZE][SIZE];
    
    sc_port<bus_master_if> SW_port;
    unsigned result;
    
    Software(sc_module_name nm);
    
    SC_HAS_PROCESS(Software);
    void Software_process();
    
    sc_time pure_sw();
};



#endif /* defined(__hw02__software__) */
