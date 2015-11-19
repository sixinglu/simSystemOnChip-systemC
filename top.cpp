//
//  top.cpp
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#include "top.h"

Top::Top(sc_module_name nm, char* memInitFilename, unsigned period){
    
    string name ="";
    
    name ="Traffic";
    traff_obj = new Traffic(name.c_str(), period);
    
    name ="SAD";
    SAD_obj = new SAD(name.c_str());
    
    name = "Hardware";
    hw_obj = new Hardware(name.c_str());
        
    name = "Bus";
    bus_obj = new Bus(name.c_str());
    
    name= "Software";
    sw_obj = new Software(name.c_str());
    
    name = "Memory";
    mem_obj = new Memory(name.c_str(), memInitFilename);
    
    sw_obj->SW_port.bind(*bus_obj);
    hw_obj->HW_master_port.bind(*bus_obj);
    hw_obj->HW_slave_port.bind(*bus_obj);
    mem_obj->MEM_port.bind(*bus_obj);
    SAD_obj->SAD_port.bind(*bus_obj);
    traff_obj->traffic_port.bind(*bus_obj);

    hw_obj->Rst(Rst);
    
    
}
