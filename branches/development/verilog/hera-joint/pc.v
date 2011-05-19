`timescale 1ms / 1ms
   
module pc (
    clk, rst_s,
    taken_pc, next_pc,
    return_pc, hold_pc, q,
    npc, pc);
    
  input wire clk;
  input wire rst_s;
  input wire taken_pc;  //from ALU  
  input wire return_pc; // from decoder 
  input wire hold_pc;  // from decoder
  input wire [15:0] next_pc;  //from ALU
  input wire [15:0] q;//from RAM
  
  output reg [9:0] npc;  // to ROM
  output reg [15:0] pc; //to ALU
  
  reg hold;
  

always@(posedge clk)
begin
  if(~rst_s) hold = 1'b0;
  else if(return_pc) hold = 1'b1;
  else hold = 1'b0;
  end

always@(*) 
begin

   if(~rst_s)  npc = {10{1'b0}};
   else if (return_pc || hold) npc = q[9:0];
  // else if ((hold_pc) && (~return_pc)) npc = npc;   
   else if (taken_pc) npc = next_pc[9:0];
   else npc = pc[9:0]+1'b1;
 
end

always@(posedge clk)
begin
  if(~rst_s) pc <= {16{1'b0}};
    
  else pc <= {6'b000000, npc[9:0]};
end
endmodule