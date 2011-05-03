
`timescale 1ns / 1ps

module alu_top
(
  input wire rst,
  input wire clk,
  //
  //from decoder
  //
  input wire [15:0] pc,
  
  input wire op_set_val,//
  input wire op_set_hi,//
  
  input wire op_al_val,//
  input wire [2:0] op_al_code,//
  
  input wire op_shift_val,
  input wire [3:0] op_shift_code,
  
  input wire op_memory_val,
  input wire op_memory_st,
  
  input wire op_branch_val,
  input wire [4:0] op_branch_code,
  
  input wire op_special_branch_val,
  input wire [1:0] op_special_branch_code,
  
  input wire op_call_val,
  
  input wire [7:0]  v_data,
  //
  //from RF
  //
  input wire [15:0] rf_ra,
  input wire [15:0] rf_rb,
  input wire [2:0] flags_from_ram,
  
  
  //
  //to RF
  //
  output wire [15:0] temp_reg,
  
  output wire [15:0] ram_data,
  //
  //to RAM
  //
  output wire [11:0] ram_addr,
  //output wire ram_read,
  output wire ram_write,
  
  //output wire [16] flags_from_ram,
  //
  //to PC
  //
  output wire [15:0] next_pc,
  output wire taken_pc
);
`include "opcode_alu.v"
wire c15;
wire [15:0] data_t;
wire [15:0] data_trap;

reg sign;
reg zero;
reg overflow;
reg carry;
reg carry_block;

wire [15:0] flag_data ;

assign {c15,data_t[14:0]} = ((op_al_val)&(op_al_code==op_al_code_add)         )? {1'b0,rf_ra[14:0]}+{1'b0,rf_rb[14:0]}+{15'b0,(carry&(!carry_block))}:
                            ((op_al_val)&(op_al_code==op_al_code_sub)         )? {1'b0,rf_ra[14:0]}-{1'b0,rf_rb[14:0]}-{15'b0,((!carry)&(!carry_block))}:
                            ((op_shift_val)&(op_shift_code==op_shift_code_inc))? {1'b0,rf_ra[14:0]}+{8'b0,v_data}+{15'b0,1'b1}:
                            ((op_shift_val)&(op_shift_code==op_shift_code_dec))? {1'b0,rf_ra[14:0]}-{8'b0,(v_data+{7'b0,1'b1})}://(~{7'b0,(v_data+1)}):
                            16'b0;

assign {data_trap,data_t[15]} = ((op_al_val)&(op_al_code==op_al_code_add ))? rf_ra[15]+rf_rb[15]+c15:
                                ((op_al_val)&(op_al_code==op_al_code_sub ))? rf_ra[15]-rf_rb[15]+c15:
                                ((op_shift_val)&(op_shift_code==op_shift_code_inc))? rf_ra[15]+c15:
                                ((op_shift_val)&(op_shift_code==op_shift_code_dec))? rf_ra[15]+c15:
                                16'b0;
//
//operations? which work with ram_data
//
assign {flag_data,ram_data} = (op_set_val&(~op_set_hi))                 ? {16'b0,{8{v_data[7]}},v_data}:
                              (op_set_val&(op_set_hi)                  )? {16'b0,v_data, rf_ra[7:0]}:
                              ((op_al_val)&(op_al_code==op_al_code_and))? {16'b0, rf_ra & rf_rb}:
                              ((op_al_val)&(op_al_code==op_al_code_or) )? {16'b0,rf_ra | rf_rb}:
                              ((op_al_val)&(op_al_code==op_al_code_add)|(op_al_code==op_al_code_sub ))? {data_trap,data_t}:
                              ((op_al_val)&(op_al_code==op_al_code_mult)&(~rf_ra[15])&(~rf_rb[15]))? rf_ra*rf_rb:/*(rf_ra[15]^rf_rb[15])*(rf_ra[14:0]*rf_rb[14:0])+
                                                                           //(~(rf_ra[15]^rf_rb[15]))*(~{2'b0,(rf_ra[14:0]*rf_rb[14:0])})://rf_ra*rf_rb://{(rf_ra[15]^rf_rb[15]),(rf_ra[15]^rf_rb[15]),((rf_ra[14:0])*(rf_rb[14:0]))}:*/
                              ((op_al_val)&(op_al_code==op_al_code_mult)&(rf_ra[15])&(rf_rb[15]))? ({1'b1,{16{1'b0}}}-rf_ra)*({1'b1,{16{1'b0}}}-rf_rb)://*((~rf_rb)+1):
                              ((op_al_val)&(op_al_code==op_al_code_mult)&(~rf_ra[15])&(rf_rb[15]))? {1'b1,{32{1'b0}}}-((rf_ra*(({1'b1,{16{1'b0}}}-rf_rb)))):
                              ((op_al_val)&(op_al_code==op_al_code_mult)&(rf_ra[15])&(~rf_rb[15]))? {1'b1,{32{1'b0}}}-((rf_rb*(({1'b1,{16{1'b0}}}-rf_ra))))://(~((rf_rb*((~rf_ra)+1))))+1:
                              ((op_al_val)&(op_al_code==op_al_code_xor ))? {16'b0,rf_ra ^ rf_rb}:
                              (op_memory_val&op_memory_st               )? {16'b0,rf_ra}:
                              (op_call_val                              )? {16'b0,pc+1}:
                              (~op_shift_val                            )?  32'b0:
                              (op_shift_code==op_shift_code_lsl         )? {16'b0,rf_ra[14:0],(carry&(~carry_block))}:
                              (op_shift_code==op_shift_code_lsr         )? {16'b0,(carry&(~carry_block)),rf_ra[15:1]}:
                              (op_shift_code==op_shift_code_lsl8        )? {16'b0,rf_ra[7:0],8'b0}:
                              (op_shift_code==op_shift_code_lsr8        )? {16'b0,8'b0,rf_ra[15:8]}:
                              (op_shift_code==op_shift_code_asl         )? {16'b0,rf_ra[14:0],(carry&(~carry_block))}:
                              (op_shift_code==op_shift_code_asr         )? {17'b0,rf_ra[15:1]}:
                              (op_shift_code==op_shift_code_savef       )? {16'b0,11'b0, carry_block, carry, overflow, carry, zero, sign}:
                              (op_shift_code==op_shift_code_inc         )? {data_trap,data_t}:
                              (op_shift_code==op_shift_code_dec         )? {data_trap,data_t}:
                              32'b0;                         
//
//modify temporary register? if used multiplication operation
//
assign temp_reg = (op_al_val)? flag_data:
                  {8'b0,v_data};
//
//set sign flag 
//
always@(posedge clk)
begin
  if(~rst)
    sign <= 1'b0;
  else
    sign <= (op_al_val&((op_al_code==op_al_code_mult))        )? temp_reg[15] :
            (op_al_val                                        )? ram_data[15] :
            (op_shift_val&(op_shift_code==op_shift_code_setf) )? sign|v_data[0]:
            (op_shift_val&(op_shift_code==op_shift_code_clrf) )? sign&(~v_data[0]):
            (op_shift_val&(op_shift_code==op_shift_code_rstrf))? rf_ra[0]:
            (op_shift_val&((op_shift_code==op_shift_code_inc)|
            (op_shift_code==op_shift_code_dec))               )? ram_data[15]:
            (flags_from_ram[2]                                )? flags_from_ram[1]:
            sign;
end
//
//set real_sign for branchs
//
wire real_sign;
assign real_sign =(flags_from_ram[2])? flags_from_ram[1]: sign;
//
//set flag zero
//
always@(posedge clk)
begin
  if(~rst)
    zero <= 1'b0;
  else 
    zero<=(op_al_val&(op_al_code!=op_al_code_mult)&(ram_data==16'b0)            )? 1'b1 :
          (op_al_val&(op_al_code!=op_al_code_mult)&(ram_data!=16'b0)            )? 1'b0 :
          (op_al_val&(op_al_code==op_al_code_mult)&({flag_data,ram_data}==32'b0))? 1'b1 :
          (op_al_val&(op_al_code==op_al_code_mult)&({flag_data,ram_data}!=32'b0))? 1'b0 :
          (op_shift_val&(op_shift_code==op_shift_code_setf)                     )? zero|v_data[1]:
          (op_shift_val&(op_shift_code==op_shift_code_clrf)                     )? zero&(~v_data[1]):
          (op_shift_val&(op_shift_code==op_shift_code_rstrf)                    )? rf_ra[1]:
          (op_shift_val&((op_shift_code==op_shift_code_inc)|
          (op_shift_code==op_shift_code_dec))&(ram_data==16'b0)                 )? 1'b1:
          (op_shift_val&((op_shift_code==op_shift_code_inc)|
          (op_shift_code==op_shift_code_dec))&(ram_data!=16'b0)                 )? 1'b0:
          (flags_from_ram[2]                                                    )? flags_from_ram[0]:
          zero;
end
//
//set real_zero for branchs
//
wire real_zero;
assign real_zero =(flags_from_ram[2])? flags_from_ram[0]: zero;
//
//set flag overflow
//
always@(posedge clk)
begin
  if(~rst)
    overflow <= 1'b0;
  else
    if(op_al_val&((op_al_code==op_al_code_add )|(op_al_code==op_al_code_sub ))) 
      overflow <= (c15 ^ flag_data[0]);
  else if(op_al_val&(op_al_code==op_al_code_mult))
      overflow <= ((~rf_ra)&(~rf_rb)&((rf_ra*rf_rb)>32'h0000ffff))? 1'b1:
                  ((~rf_ra)&(rf_rb)&((({1'b1,{16{1'b0}}}-rf_ra)*rf_rb)>32'h0000ffff))? 1'b1:
                  ((~rf_rb)&(rf_rb)&((({1'b1,{16{1'b0}}}-rf_rb)*rf_ra)>32'h0000ffff))? 1'b1:
                  ((~rf_ra)&(~rf_rb)&((({1'b1,{16{1'b0}}}-rf_ra)*({1'b1,{16{1'b0}}}-rf_rb))>32'h0000ffff))? 1'b1:


                   1'b0;
    /*begin 
      if(flag_data==16'b0)
        overflow <=1'b0;
      else
        overflow <=1'b1;
    end  */  
    else if(op_shift_val&((op_shift_code==op_shift_code_inc)|(op_shift_code==op_shift_code_dec))) 
      overflow <= (c15 ^ flag_data[0]);
    else overflow <= (~op_shift_val)? overflow:
                     (op_shift_code==op_shift_code_setf )? overflow|v_data[2]:
                     (op_shift_code==op_shift_code_clrf )? overflow&(~v_data[2]):
                     (op_shift_code==op_shift_code_rstrf)? rf_ra[2]:
                     overflow;
end
//
//set flag carry
//
always@(posedge clk)
begin
  if(~rst)
    carry <= 1'b0;
  else if(op_al_val&((op_al_code==op_al_code_add )|(op_al_code==op_al_code_sub ))) 
      carry <= flag_data[0]; 
  else if(op_shift_val&((op_shift_code==op_shift_code_inc)|(op_shift_code==op_shift_code_dec))) 
      carry <= flag_data[0];     
  else carry <= (~op_shift_val)? carry:
                  (op_shift_code==op_shift_code_lsl  )? rf_ra[15]:
                  (op_shift_code==op_shift_code_lsr  )? rf_ra[0]:
                  (op_shift_code==op_shift_code_asl  )? rf_ra[15]:
                  (op_shift_code==op_shift_code_asr  )? rf_ra[0]:
                  (op_shift_code==op_shift_code_setf )? carry|v_data[3]:
                  (op_shift_code==op_shift_code_clrf )? carry&(~v_data[3]):
                  (op_shift_code==op_shift_code_rstrf)? rf_ra[3]:
                  carry;
end
//
//set flag carry_block
//
always@(posedge clk)
begin
  if(~rst)
    carry_block <= 1'b0;
  else
    carry_block <= (~op_shift_val)? carry_block:
                   (op_shift_code==op_shift_code_setf )? carry_block|v_data[4]:
                   (op_shift_code==op_shift_code_clrf )? carry_block&(~v_data[4]):
                   (op_shift_code==op_shift_code_rstrf)? rf_ra[4]:
                   carry_block;
end
//
//memory instructions
//
assign ram_addr = (op_memory_val)? rf_ra[11:0]+v_data:
                  (op_special_branch_val&(op_special_branch_code==op_special_br_code_return))? rf_ra[11:0]:
                  12'b0;

//assign ram_data = (op_memory_val&op_memory_st)? rf_rb: 16'b0;
/*
assign ram_read = ((op_memory_val&&(~op_memory_st))|
                  (op_special_branch_val&(op_special_branch_code==op_special_br_code_return)))? 1'b1 : 1'b0;
*/
assign ram_write = ((op_memory_val&&(op_memory_st))|(op_call_val))? 1'b1 : 1'b0;
//
//instructions? which work whith pc
//
assign taken_pc = (op_branch_val&&((op_branch_code==op_br_code_br)||
                                   (op_branch_code==op_br_code_brr)||               
                                   ((op_branch_code==op_br_code_bl)&(real_sign+overflow))||
                  ((op_branch_code==op_br_code_blr)&(real_sign+overflow))||                  
                  ((op_branch_code==op_br_code_bge)&(~(real_sign+overflow)))||
                  ((op_branch_code==op_br_code_bger)&(~(real_sign+overflow)))||                 
                  ((op_branch_code==op_br_code_ble)&((real_sign+overflow)|real_zero))||
                  ((op_branch_code==op_br_code_bler)&((real_sign+overflow)|real_zero))||                 
                  ((op_branch_code==op_br_code_bg)&(~((real_sign+overflow)|real_zero)))||
                  ((op_branch_code==op_br_code_bgr)&(~((real_sign+overflow)|real_zero)))||                 
                  ((op_branch_code==op_br_code_bule)&(~carry|real_zero))||
                  ((op_branch_code==op_br_code_buler)&(~carry|real_zero))||                
                  ((op_branch_code==op_br_code_bug)&(~(~carry|real_zero)))|| 
                  ((op_branch_code==op_br_code_bugr)&(~(~carry|real_zero)))||                  
                  ((op_branch_code==op_br_code_bz)&(real_zero))||
                  ((op_branch_code==op_br_code_bzr)&(real_zero))||               
                  ((op_branch_code==op_br_code_bnz)&(~real_zero))||
                  ((op_branch_code==op_br_code_bnzr)&(~real_zero))||                  
                  ((op_branch_code==op_br_code_bc)&(carry))||
                  ((op_branch_code==op_br_code_bcr)&(carry))||                  
                  ((op_branch_code==op_br_code_bnc)&(~carry))||
                  ((op_branch_code==op_br_code_bncr)&(~carry))||                  
                  ((op_branch_code==op_br_code_bs)&(real_sign))||
                  ((op_branch_code==op_br_code_bsr)&(real_sign))||                 
                  ((op_branch_code==op_br_code_bns)&(~real_sign))|| 
                  ((op_branch_code==op_br_code_bnsr)&(~real_sign))||                 
                  ((op_branch_code==op_br_code_bv)&(overflow))||
                  ((op_branch_code==op_br_code_bvr)&(overflow))||                  
                  ((op_branch_code==op_br_code_bnv)&(~overflow))|| 
                  ((op_branch_code==op_br_code_bnvr)&(~overflow))))? 1'b1 :
                  (op_call_val)? 1'b1: 1'b0;
 
assign next_pc = ((op_branch_val)&(~op_branch_code[0]))? rf_ra: 
                 ((op_branch_val)&(op_branch_code[0]))? pc+{{8{v_data[7]}},8'b0}+v_data:
                 (op_call_val)? rf_ra: 16'b0;


endmodule
