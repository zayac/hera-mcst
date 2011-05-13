`timescale 100ns / 1ps

module mem_ctrl
(
  input wire clk, //clock
    
  input wire WE, //write enable  
  input wire [15:0] addr, //address
  input wire [15:0] dat_in, //data input
  input wire [15:0] dat_fr_mem, //data from memory
  output reg [15:0] mem_addr, //address in memory
  output wire [15:0] dat_to_mem, //data to memory
  output reg [15:0] out_dat, //data to processor
  output wire CE_, 
  output  OE_, 
  output  WE_, 
  output  LB_, 
  output  UB_ //chip en, output en, write en, lower byte, upper byte
);
  
  reg [15:0]dat_to_mem_r;
  
/*---------------------for enabling writing or reading----------------------*/
  assign WE_ = ~WE;
  assign CE_ = 1'b0;
  assign LB_ = 1'b0;
  assign UB_ = 1'b0;
  assign OE_ = WE; 
/*--------------------------------------------------------------------------*/  
 
  //assign mem_addr = addr; //redirecting address to memory 
always @ (posedge clk) 
begin
  
  if (WE == 1) 
    begin
      mem_addr <= addr; //redirect address
      dat_to_mem_r <= dat_in; //write
    end
  else 
    begin
      mem_addr <= addr; //redirect address
      out_dat <= dat_fr_mem; //read
    end
end
  assign dat_to_mem = (WE) ? dat_to_mem_r : 16'bz;
  
endmodule
