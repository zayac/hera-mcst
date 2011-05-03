`ifndef __OPCODE__
`define __OPCODE__

parameter [2:0] op_al_code_and=3'b000,
          op_al_code_or=3'b001,
          op_al_code_add=3'b010,
          op_al_code_sub=3'b011,
          op_al_code_mult=3'b100,
          op_al_code_xor=3'b101;
parameter [3:0]       
          op_shift_code_lsl=4'b0000,
          op_shift_code_lsr=4'b0001,
          op_shift_code_lsl8=4'b0010,
          op_shift_code_lsr8=4'b0011,
          op_shift_code_asl=4'b0100,
          op_shift_code_asr=4'b0101,
          op_shift_code_setf=4'b0110,
          op_shift_code_clrf=4'b0111,
          op_shift_code_savef=4'b1000,
          op_shift_code_rstrf=4'b1001,
          //op_shift_code_lsl8=4'b1010,
          //op_shift_code_lsr8=4'b1011,
          op_shift_code_inc=4'b1010,
          op_shift_code_dec=4'b1011;
          
parameter op_memory_load = 1'b0;
parameter op_memory_store = 1'b1;
		  
          
parameter [4:0]        
          op_br_code_br=5'b00000,
          op_br_code_bl=5'b00010,
          op_br_code_bge=5'b00100,
          op_br_code_ble=5'b00110,
          op_br_code_bg=5'b01000,
          op_br_code_bule=5'b01010,
          op_br_code_bug=5'b01100,
          op_br_code_bz=5'b01110,
          op_br_code_bnz=5'b10000,
          op_br_code_bc=5'b10010,
          op_br_code_bnc=5'b10100,
          op_br_code_bs=5'b10110,
          op_br_code_bns=5'b11000,
          op_br_code_bv=5'b11010,
          op_br_code_bnv=5'b11100,
          
          op_br_code_brr=5'b00001,
          op_br_code_blr=5'b00011,
          op_br_code_bger=5'b00101,
          op_br_code_bler=5'b00111,
          op_br_code_bgr=5'b01001,
          op_br_code_buler=5'b01011,
          op_br_code_bugr=5'b01101,
          op_br_code_bzr=5'b01111,
          op_br_code_bnzr=5'b10001,
          op_br_code_bcr=5'b10011,
          op_br_code_bncr=5'b10101,
          op_br_code_bsr=5'b10111,
          op_br_code_bnsr=5'b11001,
          op_br_code_bvr=5'b11011,
          op_br_code_bnvr=5'b11101;


parameter [1:0]         
          op_special_br_code_swi=2'b00,
          op_special_br_code_rti=2'b01,
          op_special_br_code_return=2'b10;
          
parameter op_set_l=1'b0,
		  op_set_h=1'b1;
		  
		  


//internal decoder parameters



parameter [3:0] de_setlo = 4'b1110,
				de_sethi = 4'b1111,
				de_and = 4'b1000,
				de_or = 4'b1001,
				de_add = 4'b1010,
				de_sub = 4'b1011,
				de_mult = 4'b1100,
				de_xor = 4'b1101;

parameter [7:0] de_lsl = 8'b00110000,
				de_lsr = 8'b00110001,
				de_lsl8 = 8'b00110010,
				de_lsr8 = 8'b00110011,
				de_asl = 8'b00110100,
				de_asr = 8'b00110101;

parameter [8:0] de_setf = 9'b001100110,
				de_clrf = 9'b001110110,
				de_savef = 9'b001101110,
				de_rstrf = 9'b001101111;
				
parameter [5:0] de_inc = 6'b001110,
				de_dec = 6'b001111;

parameter [2:0] de_load = 3'b010,
				de_store = 3'b011;
				
parameter [2:0] de_branch = 3'b000;

parameter [3:0] de_special_branch = 4'b0001;  //this parameter shows difference between 
											  //common branch and special branch 

parameter de_reg_br = 1'b1,
		  de_rel_br = 1'b0;
		  
parameter [3:0] de_br = 4'b0000,
				de_bl = 4'b0010,
				de_bge = 4'b0011,
				de_ble = 4'b0100,
				de_bg = 4'b0101,
				de_bule = 4'b0110,
				de_bug = 4'b0111,
				de_bz = 4'b1000,
				de_bnz = 4'b1001,
				de_bc = 4'b1010,
				de_bnc = 4'b1011,
				de_bs = 4'b1100,
				de_bns = 4'b1101,
				de_bv = 4'b1110,
				de_bnv = 4'b1111;
				
				
parameter [7:0] de_sp_br = 8'b00010001; 	//this parameter is a condition for special branch
				
parameter [11:0] de_swi = 12'b000100010000;

parameter [15:0] de_rti = 16'b0001000100010000,
				 de_return = 16'b0001000100010001;
				
parameter [3:0] de_call = 4'b0010;
				
`endif