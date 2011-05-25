`timescale 1ms / 1ms
   
module pc (
  // Clock and Reset
  input clk,
  input rst,
  input taken_pc,  // From ALU  
  input return_pc, // From decoder 
  input hold_pc,   // From decoder
  input [15:0] next_pc,  // From ALU
  input [15:0] q, // From RAM
  
  output reg [9:0] npc,  // To ROM
  output reg [15:0] pc // To ALU
);

  reg hold;

  always@(posedge clk or negedge rst)
  begin
    if (~rst)
      hold = 1'b0;
    else if (return_pc)
      hold = 1'b1;
    else
      hold = 1'b0;
  end

  always@(*) 
  begin
    if (~rst)
      npc = {10{1'b0}};
    else if (return_pc || hold)
      npc = q[9:0];
    // else if ((hold_pc) && (~return_pc)) npc = npc;   
    else if
      (taken_pc) npc = next_pc[9:0];
    else
      npc = pc[9:0]+1'b1;   
  end

  always@(posedge clk, negedge rst)
  begin
    if (~rst)
      pc <= {16{1'b0}};      
    else
      pc <= {6'b000000, npc[9:0]};
  end
endmodule
