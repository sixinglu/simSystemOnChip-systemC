//
//  main.cpp
//  hw02
//
//  Created by sixinglu on 2/24/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#include <iostream>
#include <sstream>
#include "systemc.h"
#include "string"
#include "top.h"

using namespace std;

int sc_main (int argc, char * argv[]) {
			
    string name= "TopModule";
    stringstream strValue;
    unsigned period;
   //const char* value = argv[2];
    strValue << argv[2];
    strValue >> period;
    Top *top_obj = new Top(name.c_str(), argv[1], period);
        
    sc_start();
    std::cout << "simulation done!\n";
    
    return 0;
}

