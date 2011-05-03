// top module of HERA CPU
`timescale 1ns / 1ps
module hera(
	input wire clk,
	input wire rst,
	
//	output 
	output wire out_test
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
wire return_en;
wire [15:0] load;

wire return_pc; 
wire hold_pc;

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
	.taken_pc(taken_pc)
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

	.v_data(v_data),

	.rsa(rsa), //least part of instruction
	.rsb(rsb), //second  part of instr. 
	.rd(rd), // result addr

	.load_en(load_en),
	.call_en(call_en),	
	.return_en(return_en),
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
	.mul_en(mul_en),
	.load(load),  
	.rd_data(ram_data), // from  ALU to result addr
	.rd_temp(temp_reg), // temp reg used in mult, into r13
	.load_flags(flags_from_ram), // availibe cary zero
	.rsa_data(rf_ra), 
	.rsb_data(rf_rb)
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
	
assign out_test = ram_write;
	
endmodule