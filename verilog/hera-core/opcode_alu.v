`ifndef __OPCODE_ALU__
`define __OPCODE_ALU__

parameter [2:0]
  op_al_code_and = 3'b000,
  op_al_code_or = 3'b001,
  op_al_code_add = 3'b010,
  op_al_code_sub = 3'b011,
  op_al_code_mult = 3'b100,
  op_al_code_xor = 3'b101;

parameter [3:0]       
  op_shift_code_lsl = 4'b0000,
  op_shift_code_lsr = 4'b0001,
  op_shift_code_lsl8 = 4'b0010,
  op_shift_code_lsr8 = 4'b0011,
  op_shift_code_asl = 4'b0100,
  op_shift_code_asr = 4'b0101,
  op_shift_code_setf = 4'b0110,
  op_shift_code_clrf = 4'b0111,
  op_shift_code_savef = 4'b1000,
  op_shift_code_rstrf = 4'b1001,
  op_shift_code_inc = 4'b1010,
  op_shift_code_dec = 4'b1011;

parameter [4:0]        
  op_br_code_br = 5'b00000,
  op_br_code_bl = 5'b00010,
  op_br_code_bge = 5'b00100,
  op_br_code_ble = 5'b00110,
  op_br_code_bg = 5'b01000,
  op_br_code_bule = 5'b01010,
  op_br_code_bug = 5'b01100,
  op_br_code_bz = 5'b01110,
  op_br_code_bnz = 5'b10000,
  op_br_code_bc = 5'b10010,
  op_br_code_bnc = 5'b10100,
  op_br_code_bs = 5'b10110,
  op_br_code_bns = 5'b11000,
  op_br_code_bv = 5'b11010,
  op_br_code_bnv = 5'b11100,
  
  op_br_code_brr = 5'b00001,
  op_br_code_blr = 5'b00011,
  op_br_code_bger = 5'b00101,
  op_br_code_bler = 5'b00111,
  op_br_code_bgr = 5'b01001,
  op_br_code_buler = 5'b01011,
  op_br_code_bugr = 5'b01101,
  op_br_code_bzr = 5'b01111,
  op_br_code_bnzr = 5'b10001,
  op_br_code_bcr = 5'b10011,
  op_br_code_bncr = 5'b10101,
  op_br_code_bsr = 5'b10111,
  op_br_code_bnsr = 5'b11001,
  op_br_code_bvr = 5'b11011,
  op_br_code_bnvr = 5'b11101;

parameter [1:0]         
  op_special_br_code_swi = 2'b00,
  op_special_br_code_rti = 2'b01,
  op_special_br_code_return = 2'b10;

`endif  // __OPCODE_ALU__
