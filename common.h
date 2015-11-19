//
//  common.h
//  hw02
//
//  Created by sixinglu on 2/25/15.
//  Copyright (c) 2015 sixinglu. All rights reserved.
//

#ifndef __hw02__common__
#define __hw02__common__

#include <stdio.h>
#include <time.h>
#include <sys/time.h>

#define mem_size 36
#define SAD_MEM_SIZE 429499672
#define LOOPS 1000
#define SIZE 5
#define CYCLE_TIME 100.0/15

// address management
#define a_start_address 0
#define a_end_address (a_start_address+mem_size-1)

#define b_start_address (a_end_address + 1)
#define b_end_address (b_start_address+mem_size-1)

#define c_start_address (b_end_address + 1)
#define c_end_address (c_start_address+mem_size-1)

#define hw_start_address (c_end_address + 1)
#define hw_end_address (hw_start_address+3*mem_size-1)

#define SAD_reg_address hw_start_address
#define CONTROL_address (hw_start_address+1)
#define Done_reg_address (hw_start_address+2)
#define CONTROL_Length_address (hw_start_address+3)
#define block1_address (hw_start_address+4)
#define block2_address (hw_start_address+5)
#define block3_address (block2_address+SIZE)  // store mem start address

#define MEM_start_address (hw_end_address+1)
#define MEM_end_address (MEM_start_address+SAD_MEM_SIZE)

// control masters
#define MASTER_NUM 4
#define SW_id 1
#define HW_id 2
#define SAD_id 3
#define Traff_id 4

// option
#define READ_REQ 0
#define WRITE_REQ 1

//HW control signals
#define exec_flag 1
#define cal_done_flag 1

#endif /* defined(__hw02__common__) */
