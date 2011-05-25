// top module of HERA CPU
`timescale 1ns / 1ps
module hera(
	input wire clk,
	input wire rst,
	
//	output 
	output wire out_test,
	/**
	 * COM-port interfaces
	 **/
  output wire  tx, // data transmit 
  input  wire dtr, // data terminal ready
  output wire dsr, // data set ready
  output wire cd, // carrier detect
  input  wire rts, //request to send
  output wire cts //clear to send
);

wire taken_pc;
wire [15:0] next_pc;
wire [15:0] pc;
wire [3:0] rsa;
wire [3:0] rsb;
wire [3:0] rd;
wire load_en;
wire [15:0] q;
wire mul_en;

wire op_set_val;
wire op_set_code;

wire op_al_val;
wire [2:0] op_al_code;

wire op_shift_val;
wire [3:0] op_shift_code;

wire op_memory_val;
wire op_memory_code;

wire op_branch_val;
wire [4:0] op_branch_code;

wire op_special_branch_val;
wire [1:0] op_special_branch_code;

wire op_call_val;
wire op_swi_val;

wire [7:0] v_data;
//wire [15:0] pc;
wire [9:0] npc;

wire op_set_hi;
wire op_memory_st;

wire [15:0] rf_ra;
wire [15:0] rf_rb;
wire [2:0] flags_from_ram;
wire [15:0] temp_reg;

wire [15:0] ram_data;
wire [11:0] ram_addr;
wire ram_write;

wire call_en;
wire swi_en;
wire return_en;
wire rti_en;
wire [15:0] load;

wire return_pc; 
wire hold_pc;

	wire [15:0] R0;
	wire [15:0] R1;
	wire [15:0] R2;
	wire [15:0] R3;
	wire [15:0] R4;
	wire [15:0] R5;
	wire [15:0] R6;
	wire [15:0] R7;
	wire [15:0] R8;
	wire [15:0] R9;
	wire [15:0] R10;
	wire [15:0] R11;
	wire [15:0] R12;
	wire [15:0] R13;
	wire [15:0] R14;
	wire [15:0] R15;
	wire [4:0] flags;
	wire [5:0] vis_address_a;
	wire [5:0] vis_address_b;
	wire [31:0] vis_data_a;
	wire [31:0] vis_data_b;
	wire vis_wren_a;
	wire vis_wren_b;
	wire [31:0] vis_q_a;
	wire [31:0] vis_q_b;
	
alu_top alu_top(
	.rst(rst),
	.clk(clk),
  
	.pc(pc),
  
	.op_set_val(op_set_val),//
	.op_set_hi(op_set_hi),//
  
	.op_al_val(op_al_val),//
	.op_al_code(op_al_code),//
  
	.op_shift_val(op_shift_val),
	.op_shift_code(op_shift_code),
  
	.op_memory_val(op_memory_val),
	.op_memory_st(op_memory_st),
  
	.op_branch_val(op_branch_val),
	.op_branch_code(op_branch_code),
  
	.op_special_branch_val(op_special_branch_val),
	.op_special_branch_code(op_special_branch_code),
  
	.op_call_val(op_call_val),
	.op_swi_val(op_swi_val),
  
	.rf_ra(rf_ra),
	.rf_rb(rf_rb),
	.flags_from_ram(flags_from_ram),
  
	.v_data(v_data),

	.temp_reg(temp_reg),
  
	.ram_data(ram_data),
	.ram_addr(ram_addr),
  //output wire ram_read,
	.ram_write(ram_write),
  
  //output wire [16] flags_from_ram,
  
	.next_pc(next_pc),
	.taken_pc(taken_pc),
	
	.flags (flags[4:0])
);

decoder decoder(
	.rst(rst),
	.clk(clk),

	.q(q), // ROM
//
	.op_set_val(op_set_val),
	.op_set_code(op_set_hi),

	.op_al_val(op_al_val),
	.op_al_code(op_al_code),

	.op_shift_val(op_shift_val),
	.op_shift_code(op_shift_code),

	.op_memory_val(op_memory_val),
	.op_memory_code(op_memory_st),

	.op_branch_val(op_branch_val),
	.op_branch_code(op_branch_code),

	.op_special_branch_val(op_special_branch_val),
	.op_special_branch_code(op_special_branch_code),

	.op_call_val(op_call_val),
	.op_swi_val(op_swi_val),

	.v_data(v_data),

	.rsa(rsa), //least part of instruction
	.rsb(rsb), //second  part of instr. 
	.rd(rd), // result addr

	.load_en(load_en),
	.call_en(call_en),	
	.return_en(return_en),
	.swi_en(swi_en),	
	.rti_en(rti_en),
	.mul_en(mul_en),
	
	.hold_pc(hold_pc),
	.return_pc(return_pc)
);

pc pc_inst(
	.rst(rst),
	.clk(clk),
	.taken_pc(taken_pc),//ËÁ ¿À”
	.return_pc(return_pc),
	.hold_pc(hold_pc),
	.next_pc(next_pc),//ËÁ ¿À”
	.q(load),
	.npc(npc),
	.pc(pc)
);

hera_regf hera_regf(
	.rst(rst),
	.clk(clk),
	.rsa(rsa), //least part of instructoin
	.rsb(rsb), //second  part of instr. 
	.rd(rd), // result addr
	.load_en(load_en),
	.call_en(call_en),
	.return_en(return_en),
	.swi_en(swi_en),
	.rti_en(rti_en),
	.mul_en(mul_en),
	.load(load),  
	.rd_data(ram_data), // from  ALU to result addr
	.rd_temp(temp_reg), // temp reg used in mult, into r13
	.load_flags(flags_from_ram), // availibe cary zero
	.rsa_data(rf_ra), 
	.rsb_data(rf_rb),
	.R1 (R1[15:0]),
  .R2 (R2[15:0]),
  .R3 (R3[15:0]),
  .R4 (R4[15:0]),
  .R5 (R5[15:0]),
  .R6 (R6[15:0]),
  .R7 (R7[15:0]),
  .R8 (R8[15:0]),
  .R9 (R9[15:0]),
  .R0 (R0[15:0]),
  .R10 (R10[15:0]),
  .R11 (R11[15:0]),
  .R12 (R12[15:0]),
  .R13 (R13[15:0]),
  .R14 (R14[15:0]),
  .R15 (R15[15:0])
);

hera_rom hera_rom (
	.address(npc),
	.clock(clk),
	.q(q)
);
	
hera_ram	hera_ram_inst (
	.clock(clk),
	.data ( ram_data ),
//	.rdaddress ( ram_addr ),
//	.wraddress (ram_addr),
	.address (ram_addr[10:0]),
	.wren ( ram_write ),
	.q ( load )
	);

vis_dev vis_dev(
  .clk_ (clk),
  .rst(rst),
  .q (q[15:0]),
  .pc (pc[15:0]),
	.R1 (R1[15:0]),
  .R2 (R2[15:0]),
  .R3 (R3[15:0]),
  .R4 (R4[15:0]),
  .R5 (R5[15:0]),
  .R6 (R6[15:0]),
  .R7 (R7[15:0]),
  .R8 (R8[15:0]),
  .R9 (R9[15:0]),
  .R0 (R0[15:0]),
  .R10 (R10[15:0]),
  .R11 (R11[15:0]),
  .R12 (R12[15:0]),
  .R13 (R13[15:0]),
  .R14 (R14[15:0]),
  .R15 (R15[15:0]),
  .flags (flags[4:0]),
  .tx (tx),
  .dtr (dtr),
  .dsr (dsr),
  .cd (cd),
  .cts (cts),
  .rts (rts),
	.address_a (vis_address_a[5:0]),
	.address_b (vis_address_b[5:0]),
	.data_a (vis_data_a[31:0]),
	.data_b (vis_data_b[31:0]),
	.wren_a (vis_wren_a),
	.wren_b (vis_wren_b),
	.q_a (vis_q_a[31:0]),
	.q_b (vis_q_b[31:0]),
	.rsa (rsa[3:0]),
	.rsb (rsb[3:0]),
	.rsd (rd[3:0])
);

vis_ram vis_ram (
	.address_a (vis_address_a[5:0]),
	.address_b (vis_address_b[5:0]),
	.clock (clk),
	.data_a (vis_data_a[31:0]),
	.data_b (vis_data_b[31:0]),
	.wren_a (vis_wren_a),
	.wren_b (vis_wren_b),
	.q_a (vis_q_a[31:0]),
	.q_b (vis_q_b[31:0])
);
	
assign out_test = ram_write;
	
endmodule