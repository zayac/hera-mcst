// top module of HERA CPU
`timescale 1ns / 1ps
module hera (
  // Clock and Reset
  input clk,
  input hard_rst,

  // Output 
  output out_test,
  inout [15:0] IO,
  output wire [15:0] mem_addr,
  output wire CE_,
  output wire OE_,
  output wire WE_,
  output wire LB_,
  output wire UB_,
  
 // output reg  tx,
  input wire rd_com,
  input  wire dtr,
  output wire dsr,
  output wire cd,
  input  wire rts,
  output wire cts

);
wire rst_s;
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
wire op_write; //write_enable for commands memory
reg [9:0] op_addr; //address for commands memory
wire [9:0] hyperterm_address; //address from hyperterm module
wire [15:0] hyperterm_data; //data from hyperterm module
reg mode; //this signal switches command memory from being written from COM interface to reading
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

alu_top alu_top(
	.rst_s(rst_s),
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
	.ram_addr(mem_addr),
  //output wire ram_read,
	.ram_write(ram_write),
  
  //output wire [16] flags_from_ram,
  
	.next_pc(next_pc),
	.taken_pc(taken_pc)
);

decoder decoder(
	.rst_s(rst_s),
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
	.rst_s(rst_s),
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
	.rst_s(rst_s),
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
	.rsb_data(rf_rb)
);
/*
hera_rom hera_rom (
	.address(npc),
	.clock(clk),
	.q(q)
);
*/
hera_ram	hera_ram_inst (
	.clock(clk),
	.data ( hyperterm_data ),
//	.rdaddress ( ram_addr ),
//	.wraddress (ram_addr),
	.address (op_addr),
	.wren ( op_write ),
	.q ( q )	
	);

	
mem_ctrl mem_ctrl (

  .clk (clk),
  .WE (ram_write),
  .addr (ram_addr),
  .dat_in (ram_data),
  .dat_fr_mem (IO),
  .mem_addr (mem_addr),
  .out_dat (load),
  .CE_ (CE_),
  .OE_ (OE_),
  .WE_ (WE_),
  .LB_ (LB_),
  .UB_ (UB_)
);

hyperterm com_interface (
  .clk_48(clk),
  .rst_(hard_rst),
  //.tx(tx),
  .rd(rd_com),
  .dtr(dtr),
  .dsr(dsr),
  .cd(cd),
  .rts(rts),
  .cts(cts),
  .data(hyperterm_data),
  .addr(hyperterm_address),
  .wren(op_write),
  .resout(rst_s)
);
always @(posedge clk, negedge hard_rst)
  mode <= (~hard_rst)? 1'b0 :
		  (~rst_s)? 1'b1 : mode;
  
always @*
  op_addr <= mode ? npc : hyperterm_address;
  
assign out_test = ram_write;
	
endmodule
