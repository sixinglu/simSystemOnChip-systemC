//
//  traffic.h
//  hw02
//
//  Created by sixinglu on 3/30/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#ifndef __hw02__traffic__
#define __hw02__traffic__

#include <stdio.h>
#include "systemc.h"
#include "interface.h"
#include "common.h"

class Traffic: public sc_module{
    
public:
    
    unsigned period;
    sc_port<bus_master_if> traffic_port;
    
    Traffic(sc_module_name nm, unsigned Period);
    SC_HAS_PROCESS(Traffic);
    
    void traffic_process();
    
};


#endif /* defined(__hw02__traffic__) */
