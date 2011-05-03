`timescale 1ns / 1ps

module decoder (
  // Clock and Reset
  input clk,
  input rst,

  // Incoming instruction code
  input [15:0] q,

  output reg op_set_val,
  output reg op_set_code,

  // Arithmetic instruction code and
  // Arithmetic flag enable
  output reg op_al_val,
  output reg [2:0] op_al_code,

  // Shift instruction code
  // Shift flag enable
  output reg op_shift_val,
  output reg [3:0] op_shift_code,

  // Memory instruction code
  // Memory flag enable
  output reg op_memory_val,
  output reg op_memory_code,

  // Branch instruction code and
  // Branch flag enable
  output reg op_branch_val,
  output reg [4:0] op_branch_code,

  // Special branch instruction code and
  // Special branch flag enable
  output reg op_special_branch_val,
  output reg [1:0] op_special_branch_code,

  // Call/Sofware interrupt flag enable
  output reg op_call_val,
  output reg op_swi_val,

  // Output data for instructions
  output reg [7:0] v_data,
  
  // Least part of instruction
  output reg [3:0] rsa,
  // Second part of instruction
  output reg [3:0] rsb,
  // Result address of instruction
  output reg [3:0] rd,
  
  // Instuctions enable:
  // Load, Software interrupt, Returns, Call, Mul
  output reg load_en,
  output reg swi_en,
  output reg rti_en,
  output reg call_en,
  output reg return_en,
  output reg mul_en,

  // Instrucions for PC
  output reg hold_pc,
  output reg return_pc
);
   
  reg cf;
  
  
  `include "opcode_decoder.v"
  
  always@(posedge clk or negedge rst)
  begin
    if (~rst)
      cf <= 1'b0;
    else if ((q == de_return) || (return_pc) || (q == de_rti))
      cf <= 1'b1;
    else
      cf <= 1'b0;
  end
  
  // SETLO & SETHI flag for ALU
  always@(*)
  begin
    if ((q[15:13] == 3'b111) && (~cf))
      op_set_val = 1'b1;
    else
      op_set_val = 1'b0;
  end

  // SETLO & SETHI operation code for ALU
  always@(*)
  begin
    case (q[15:12])
      de_setlo: op_set_code = op_set_l;
      de_sethi: op_set_code = op_set_h;
      default: op_set_code = 1'b0;
    endcase
  end

  // Three-address operations flag for ALU
  always@(*)
  begin
    if ((q[15:13] == 3'b110) && (~cf))
      op_al_val = 1'b1;
    else if ((q[15:13] == 3'b101) && (~cf))
      op_al_val = 1'b1;
    else if ((q[15:13] == 3'b100) && (~cf))
      op_al_val = 1'b1;
    else
      op_al_val = 1'b0;
  end

  // Three-address operations code for ALU
  always@(*)
  begin
    case (q[15:12])
      de_and: op_al_code = op_al_code_and;
      de_or: op_al_code = op_al_code_or;
      de_add: op_al_code = op_al_code_add;
      de_sub: op_al_code = op_al_code_sub;
      de_mult: op_al_code = op_al_code_mult;
      de_xor: op_al_code = op_al_code_xor;
      default: op_al_code = 3'b000;
    endcase
  end

  // Mult flag for ALU
  always@(*)
  begin
    if (q[15:12] == 4'b1100)
      mul_en = 1'b1;
    else
      mul_en = 1'b0;  
  end

  // Shifts flag for ALU
  always@(*)
  begin
    if ((q[15:12] == 4'b0011) && (~cf))
      op_shift_val = 1'b1;
    else
      op_shift_val = 1'b0;
  end

  //Shifts code for ALU
  /*always@(*)
  begin
    case({q[15:12],q[7:4]})
      de_lsl: op_shift_code = op_shift_code_lsl;
      de_lsr: op_shift_code = op_shift_code_lsr;
      de_lsl8: op_shift_code = op_shift_code_lsl8;
      de_lsr8: op_shift_code = op_shift_code_lsr8;
      de_asl: op_shift_code = op_shift_code_asl;
      de_asr: op_shift_code = op_shift_code_asr;
      //default: op_shift_code = 4'b0000;
    endcase
			
    case({q[15:12],q[11],q[7:4]})
      de_setf: op_shift_code = op_shift_code_setf;
      de_clrf: op_shift_code = op_shift_code_clrf;
      //default: op_shift_code = 4'b0000;
    endcase

    case({q[15:12],q[7:4],q[3]})
      de_savef: op_shift_code = op_shift_code_savef;
      de_rstrf: op_shift_code = op_shift_code_rstrf;
      //default: op_shift_code = 4'b0000;
    endcase
				         
    case({q[15:12],q[7:6]})
      de_inc: op_shift_code = op_shift_code_inc;
      de_dec: op_shift_code = op_shift_code_dec;
      default: op_shift_code = 4'b0000;
    endcase
  end
  */

  always@(*)
  begin
    if ({q[15:12], q[7:4]} == de_lsl)
      op_shift_code = op_shift_code_lsl;
    else if ({q[15:12], q[7:4]} == de_lsr)
      op_shift_code = op_shift_code_lsr;
    else if ({q[15:12], q[7:4]} == de_lsl8)
      op_shift_code = op_shift_code_lsl8;
    else if ({q[15:12], q[7:4]} == de_lsr8)
      op_shift_code = op_shift_code_lsr8; 
    else if ({q[15:12], q[7:4]} == de_asl)
      op_shift_code = op_shift_code_asl;
    else if ({q[15:12], q[7:4]} == de_asr)
      op_shift_code = op_shift_code_asr;
    else if ({q[15:12], q[11], q[7:4]} == de_setf)
      op_shift_code = op_shift_code_setf;
    else if ({q[15:12], q[11], q[7:4]} == de_clrf)
      op_shift_code = op_shift_code_clrf; 
    else if ({q[15:12], q[7:4], q[3]} == de_savef)
      op_shift_code = op_shift_code_savef;  
    else if ({q[15:12], q[7:4], q[3]} == de_rstrf)
      op_shift_code = op_shift_code_rstrf; 
    else if ({q[15:12], q[7:6]} == de_inc)
      op_shift_code = op_shift_code_inc; 
    else
      op_shift_code = op_shift_code_dec;        
  end

  // Memory instruction flag for ALU
  always@(*)
  begin
    if ((q[15:14] == 2'b01) && (~cf))
      op_memory_val = 1'b1;
    else
      op_memory_val = 1'b0;
  end

  // Memory instruction code for ALU
  always@(*)
  begin
    case (q[15:13])
      de_load: op_memory_code = op_memory_load;
      de_store: op_memory_code = op_memory_store;
      default: op_memory_code = 1'b0;
    endcase
  end


  // Branches flag for ALU
  always@(*)
  begin
    if ((q[15:13] == de_branch) &&
        (~cf) &&
        (q[11:8] != de_special_branch))
      op_branch_val = 1'b1;
    else
      op_branch_val = 1'b0;
  end

  //Branches code for ALU
  /*
  always@(*)
  begin
    case (q[12])
      de_reg_br:
      begin
        case (q[11:8])
          de_br: op_branch_code = op_br_code_br;
          de_bl: op_branch_code = op_br_code_bl;
          de_bge: op_branch_code = op_br_code_bge;
          de_ble: op_branch_code = op_br_code_ble;
          de_bg: op_branch_code = op_br_code_bg;
          de_bule: op_branch_code = op_br_code_bule;
          de_bug: op_branch_code = op_br_code_bug;
          de_bz: op_branch_code = op_br_code_bz;
          de_bnz: op_branch_code = op_br_code_bnz;
          de_bc: op_branch_code = op_br_code_bc;
          de_bnc: op_branch_code = op_br_code_bnc;
          de_bs: op_branch_code = op_br_code_bs;
          de_bns: op_branch_code = op_br_code_bns;
          de_bv: op_branch_code = op_br_code_bv;
          de_bnv: op_branch_code = op_br_code_bnv;
          default: op_branch_code = 5'b00000;
        endcase
      end
      de_rel_br:
      begin
        case (q[11:8])
          de_br: op_branch_code = op_br_code_brr;
          de_bl: op_branch_code = op_br_code_blr;
          de_bge: op_branch_code = op_br_code_bger;
          de_ble: op_branch_code = op_br_code_bler;
          de_bg: op_branch_code = op_br_code_bgr;
          de_bule: op_branch_code = op_br_code_buler;
          de_bug: op_branch_code = op_br_code_bugr;
          de_bz: op_branch_code = op_br_code_bzr;
          de_bnz: op_branch_code = op_br_code_bnzr;
          de_bc: op_branch_code = op_br_code_bcr;
          de_bnc: op_branch_code = op_br_code_bncr;
          de_bs: op_branch_code = op_br_code_bsr;
          de_bns: op_branch_code = op_br_code_bnsr;
          de_bv: op_branch_code = op_br_code_bvr;
          de_bnv: op_branch_code = op_br_code_bnvr;
          default: op_branch_code = 5'b00000;
        endcase
      end
      default: op_branch_code = 5'b00000;
    endcase
  end
  */

  // Branches code for ALU
  always@(*)
  begin
    if ((q[12] == de_reg_br) && (q[11:8] == de_br))
      op_branch_code = op_br_code_br;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bl))
      op_branch_code = op_br_code_bl;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bge))
      op_branch_code = op_br_code_bge;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_ble))
      op_branch_code = op_br_code_ble;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bg))
      op_branch_code = op_br_code_bg;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bule))
      op_branch_code = op_br_code_bule;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bug))
      op_branch_code = op_br_code_bug;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bz))
      op_branch_code = op_br_code_bz;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bnz))
      op_branch_code = op_br_code_bnz;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bc))
      op_branch_code = op_br_code_bc;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bnc))
      op_branch_code = op_br_code_bnc;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bs))
      op_branch_code = op_br_code_bs;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bns))
      op_branch_code = op_br_code_bns;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bv))
      op_branch_code = op_br_code_bv;
    else if ((q[12] == de_reg_br) && (q[11:8] == de_bnv))
      op_branch_code = op_br_code_bnv;  
    else if ((q[12] == de_rel_br) && (q[11:8] == de_br))
      op_branch_code = op_br_code_brr;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bl))
      op_branch_code = op_br_code_blr;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bge))
      op_branch_code = op_br_code_bger;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_ble))
      op_branch_code = op_br_code_bler;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bg))
      op_branch_code = op_br_code_bgr;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bule))
      op_branch_code = op_br_code_buler;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bug))
      op_branch_code = op_br_code_bugr;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bz))
      op_branch_code = op_br_code_bzr;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bnz))
      op_branch_code = op_br_code_bnzr;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bc))
      op_branch_code = op_br_code_bcr;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bnc))
      op_branch_code = op_br_code_bncr;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bs))
      op_branch_code = op_br_code_bsr;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bns))
      op_branch_code = op_br_code_bnsr;
    else if ((q[12] == de_rel_br) && (q[11:8] == de_bv))
      op_branch_code = op_br_code_bvr; 
    else
      op_branch_code = op_br_code_bnvr;    
  end

  // Special branches flag for ALU
  always@(*)
  begin
    if ((q[15:8] == de_sp_br) && (~cf))
      op_special_branch_val = 1'b1;
    else
      op_special_branch_val = 1'b0;
  end

  // Special branches code for ALU
  always@(*)
  begin
    if (q[15:4] == de_swi)
      op_special_branch_code = op_special_br_code_swi;
    else if (q == de_rti)
      op_special_branch_code = op_special_br_code_rti;
    else
      op_special_branch_code = op_special_br_code_return;
  end

  // Function call flag for ALU
  always@(*) 
  begin
    if ((q[15:12] == de_call) && (~cf))
      op_call_val = 1'b1;
    else
      op_call_val = 1'b0;
  end

  // Function swi flag for ALU
  always@(*) 
  begin
    if ((q[15:4] == de_swi) && (~cf))
      op_swi_val = 1'b1;
    else
      op_swi_val = 1'b0;
  end

  //RSA
  /*
  //op_al_val or op_shift_code or op_memory_val or op_branch_val or op_call_val
  always@(*)
	begin
    if(op_al_val == 1'b1) rsa = q[7:4];//three-address
    else if(op_set_val ==1'b1) rsa = q[11:8];
    else if(op_shift_code == op_shift_code_lsl)  rsa = q[3:0];//shift 
		
	end
	*/
  always@(*)
  begin
    if (cf)
      rsb = 4'b0000;
    else if (q[15:13] == 3'b111)
      rsa = q[11:8];
    else if ({q[15:12], q[7:6]} == de_inc)
      rsa = q[11:8];
    else if ({q[15:12], q[7:6]} == de_dec)
      rsa = q[11:8];
    else if ({q[15:12], q[7:4], q[3]} == de_rstrf)
      rsa = q[11:8];
    else if (q[15:13] == 3'b100)
      rsa = q[7:4];
    else if (q[15:13] == 3'b101)
      rsa = q[7:4];
    else if (q[15:13] == 3'b110)
      rsa = q[7:4];
    else
      rsa = q[3:0];
  end
	
  // RSB
  always@(*)
  begin
    if (cf)
      rsb = 4'b0000;
    else
      rsb = q[3:0];  // Three-address
    //else rsb = 4'b0000;
  end
  
  //RD
  /*
  always@(*) //op_set_val or op_al_val or op_shift_code or op_memory_val
	begin
    if(op_set_val == 1'b1) rd = q[11:8];  //set
    else if(op_al_val == 1'b1) rd = q[11:8]; //three-address
    else if(op_shift_code == op_shift_code_lsl) rd = q[11:8];//shift 
    else if(op_shift_code == op_shift_code_lsr) rd = q[11:8];//shift
    else if(op_shift_code == op_shift_code_lsl8) rd = q[11:8];//shift
    else if(op_shift_code == op_shift_code_lsr8) rd = q[11:8];//shift
    else if(op_shift_code == op_shift_code_asl) rd = q[11:8];//shift
    else if(op_shift_code == op_shift_code_asr) rd = q[11:8];//shift
    else if(op_shift_code == op_shift_code_savef) rd = q[11:8];//save f
    else if(op_shift_code == op_shift_code_inc) rd = q[11:8];//inc
    else if(op_shift_code == op_shift_code_dec) rd = q[11:8];//dec
    else if(op_memory_val == 1'b1) rd = q[11:8]; //memory
    else rd = 4'b0000;
	end
  */

  always@(*)
  begin
    if (({q[15:12], q[7:4], q[3]} == de_rstrf) || (cf))
      rd = 4'b0000;
    else if ((q[15:12] == 3'b000) && (q != 16'b0001000100010001))
      rd = 4'b0000;    
    else rd = q[11:8];
  end

  //v_data
  /*
  //op_set_val or op_shift_code or op_memory_val or op_branch_val or op_call_val
  always@(*) 
	begin
    if(op_set_val == 1'b1) v_data = q[7:0];  //set
    else if(op_shift_code == op_shift_code_setf) v_data = {3'b000,q[8], q[3:0]};//set flags
    else if(op_shift_code == op_shift_code_clrf) v_data = {3'b000,q[8], q[3:0]};//clear flags
    else if(op_shift_code == op_shift_code_inc) v_data = {2'b00, q[5:0]};  //inc
    else if(op_shift_code == op_shift_code_dec) v_data = {2'b00, q[5:0]};  //dec
    else if(op_memory_val == 1'b1) v_data = {3'b000, q[12], q[7:4]}; //memory
    else if (op_branch_val == 1'b1) begin 
									    case(q[12]) de_rel_br: v_data = q[7:0]; //branch
												    default: rsa = 8'b00000000;
									    endcase
							       end
    else if(op_call_val == 1'b1) v_data = q[11:4];//call
    else v_data = 8'b000000;
	end
	*/


  always@(*)
  begin
    //if (q[15:13] == 3'b111) v_data = q[7:0];//set
    if ({q[15:12], q[11], q[7:4]} == de_setf)
      v_data = {3'b000, q[8], q[3:0]};  // Set flags
    else if ({q[15:12], q[11], q[7:4]} == de_clrf)
      v_data = {3'b000, q[8], q[3:0]};  // Clear flags
    else if ({q[15:12], q[7:6]} == de_inc)
      v_data = {2'b00, q[5:0]};  // Inc
    else if ({q[15:12], q[7:6]} == de_dec)
      v_data = {2'b00, q[5:0]};  // Dec
    else if (q[15:14] == 2'b01)
      v_data = {3'b000, q[12], q[7:4]};  // Memory
    else if (q[15:12] == 4'b0010)
      v_data = q[11:4];  // Call
    else if (q[15:4] == de_swi)
      v_data = {4'b0000, q[3:0]};  // Swi
    else
      v_data = q[7:0];
  end

  // Load enable
  always@(*)
  begin
    if (q[15:13] == de_load)
      load_en = 1'b1;
    else
      load_en = 1'b0;
  end

  // Return enable
  always@(*)
  begin
    if (q == de_return)
      return_en = 1'b1;
    else
      return_en = 1'b0;
  end
  
  // Call enable
  always@(*)
  begin
    if (q[15:12] == de_call)
      call_en = 1'b1;
    else
      call_en = 1'b0;
  end

  // Swi enable
  always@(*)
  begin
    if (q[15:4] == de_swi)
      swi_en = 1'b1;
    else
      swi_en = 1'b0;
  end

  // Rti enable
  always@(*)
  begin
    if (q == de_rti)
      rti_en = 1'b1;
    else
      rti_en = 1'b0;
  end

  // Flag for pc when q is function return
  always@(*)
  begin
    if (q == de_return || q == de_rti)
      hold_pc <= 1'b1;
    else
      hold_pc <= 1'b0;
  end

  always@(posedge clk or negedge rst)
  begin
    if (~rst)
      return_pc = 1'b0;
    else if (q == de_return || q == de_rti)
      return_pc = 1'b1;
    else
      return_pc = 1'b0;
  end
  
  
endmodule

