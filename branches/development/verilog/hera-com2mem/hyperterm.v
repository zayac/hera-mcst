/* This module implements interacting of Serial Port with RAM
 * for writing command memory using Hyperterminal
 * IMPORTANT: bitrate must be 2400 bps
 *            Minitel emulation must be used
 */
`define end_tr 16'heeff //signal "end of transmission". Should be changed!!!!!!
						//is written to memory
module hyperterm
(
input  wire clk_48, //external clock (48 MHz)
input  wire rst_, //reset signal
output reg  tx,
input wire rd,
input  wire dtr,
output wire dsr,
output wire cd,
input  wire rts,
output wire cts,
output reg [15:0] data,
output reg [10:0] addr,
output reg wren,
output reg clk2, //this clock is generated just for Logic Analyzer
output reg resout, //reset signal for HERA
output reg [7:0] buffer //for test only. shouldn't be output
);
//reg [7:0] buffer; //internal buffer for one byte of transmission
assign dsr = dtr;
assign cd  = dtr;
assign cts = rts;
reg [3:0] char; //buffer for storing decoded value of a symbol
reg [2:0] part; //counter showing which part (1-4) of 16-bit command we are processing
reg clk; //internal clock for dividing frame into bits (see Null-modem connection protocol)
reg end_write;
reg inc_addr; //enable for address increment

reg [4:0] res_count;
reg end_flag; //flag showing end of transmission
reg  [3:0] tx_count; //counter showing which bit of a frame we are processing now

reg [13:0] count2; //counter for the generation of clk2

reg [13:0] count; //counter for the generation of clk


always @ ( posedge clk_48, negedge rst_ )
if ( ~rst_ )
	count <= 14'd0;
else
if ( count == 14'd9999 )
	count <= 14'd0;
else
	count <= count + 1'd1;
	
always @ ( posedge clk_48, negedge rst_ )
if ( ~rst_ )
	count2 <= 14'd0;
else
if ( count2 == 14'd999 )
	count2 <= 14'd0;
else
	count2 <= count2 + 1'd1;
	
/* generation of clk */
always @ ( posedge clk_48, negedge rst_ )
if ( ~rst_ )
	clk <= 1'b0;
else
if ( count == 14'd9999 )
	clk <= ~clk;
	
/* generation of clk2 */	
always @ ( posedge clk_48, negedge rst_ )
if ( ~rst_ )
	clk2 <= 1'b0;
else
if ( count2 == 14'd999 )
	clk2 <= ~clk2;

always @ ( posedge clk, negedge rst_ )begin
if ( ~rst_ )
	tx_count <= 4'd0;
else
if ( tx_count == 4'd9 )
	tx_count <= 4'd0;
else
if ( ~rd | ( tx_count != 4'd00 ) )
	tx_count <= tx_count + 1'b1;
end

/* filling buffer */
always @ ( posedge clk, negedge rst_ ) begin
buffer[0] <= (~rst_)? 1'b0 :
			 (tx_count == 4'd0)? 1'b0 :
			 (tx_count == 4'd1)? rd:
			 buffer[0];
buffer[1] <= (~rst_)? 1'b0 :
			 (tx_count == 4'd0)? 1'b0 :
			 (tx_count == 4'd2)? rd:
			 buffer[1];
buffer[2] <= (~rst_)? 1'b0 :
			 (tx_count == 4'd0)? 1'b0 :
			 (tx_count == 4'd3)? rd:
			 buffer[2];
buffer[3] <= (~rst_)? 1'b0 :
			 (tx_count == 4'd0)? 1'b0 :
			 (tx_count == 4'd4)? rd:
			 buffer[3];
buffer[4] <= (~rst_)? 1'b0 :
			 (tx_count == 4'd0)? 1'b0 :
			 (tx_count == 4'd5)? rd:
			 buffer[4];
buffer[5] <= (~rst_)? 1'b0 :
			 (tx_count == 4'd0)? 1'b0 :
			 (tx_count == 4'd6)? rd:
			 buffer[5];
buffer[6] <= (~rst_)? 1'b0 :
			 (tx_count == 4'd0)? 1'b0 :
			 (tx_count == 4'd7)? rd:
			 buffer[6];
buffer[7] <= (~rst_)? 1'b0 :
			 (tx_count == 4'd0)? 1'b0 :
			 (tx_count == 4'd8)? rd:
			 buffer[7];
end
/* decoding symbol */
always @(posedge clk) begin
char <= (buffer == "0" & tx_count == 4'd9)? 4'h0 :
		(buffer == "1" & tx_count == 4'd9)? 4'h1 :
		(buffer == "2" & tx_count == 4'd9)? 4'h2 :
		(buffer == "3" & tx_count == 4'd9)? 4'h3 :
		(buffer == "4" & tx_count == 4'd9)? 4'h4 :
		(buffer == "5" & tx_count == 4'd9)? 4'h5 :
		(buffer == "6" & tx_count == 4'd9)? 4'h6 :
		(buffer == "7" & tx_count == 4'd9)? 4'h7 :
		(buffer == "8" & tx_count == 4'd9)? 4'h8 :
		(buffer == "9" & tx_count == 4'd9)? 4'h9 :
		((buffer == "A" | buffer == "a") & tx_count == 4'd9)? 4'ha :
		((buffer == "B" | buffer == "b") & tx_count == 4'd9)? 4'hb :
		((buffer == "C" | buffer == "c") & tx_count == 4'd9)? 4'hc :
		((buffer == "D" | buffer == "d") & tx_count == 4'd9)? 4'hd :
		((buffer == "E" | buffer == "e") & tx_count == 4'd9)? 4'he :
		((buffer == "F" | buffer == "f") & tx_count == 4'd9)? 4'hf :
		char;
end

/* assignment of part value */
always @(posedge clk, negedge rst_) begin
part <= (~rst_)? 2'b0 :
		((buffer < "0" | ((buffer > "9") & (buffer < "A")) |
		((buffer > "F") & (buffer < "a")) | buffer > "f") & tx_count == 4'd9) ? 3'b0 :
		(part == 3'd4 & tx_count == 4'd9) ? 3'b0 : 
		(tx_count == 4'd9)? part + 1'b1 : part;
end

/*forming data */
always @(posedge clk, negedge rst_)
if(~rst_)
  data <= 16'b0;
else if(tx_count == 4'd9) begin
	if(part == 3'd1)
		data[15:12] <= char;
	else if(part == 3'd2)
		data[11:8] <= char;
	else if(part == 3'd3)
		data[7:4] <= char;
	else if(part == 3'd4)
		data[3:0] <= char;
end

/*generating address */
always @(posedge clk, negedge rst_)  //combinational logic here!
//TODO: ask how to make it sequential
addr <= (~rst_)? 11'h7FF: //very, very, very, VERY dirty hack!!!!!!!!!!!
        end_write ? 11'b0 :  
        (part == 3'd4 && inc_addr == 0)? addr + 1'b1 : addr;

/*generation of wren */
always @(posedge clk, negedge rst_)
wren <= ~rst_ ? 1'b0:
		end_write ? 1'b0:
		(part == 3'd4 & tx_count == 4'd9)? 1'b1: //tx_count - experimental!
		(tx_count == 4'd0)? 1'b0 : wren;

/*generation of resout and end_flag signals */
always @(posedge clk, negedge rst_)
resout <=(~rst_)? 1'b1:
		 end_flag ? 1'b0: 1'b1; //if reset is low level


always @(posedge clk, negedge rst_)
	res_count <=(~rst_)? 4'b0:
				end_flag ? res_count + 1'b1 : res_count;	

always @(posedge clk, negedge rst_) begin
	end_flag <= (~rst_)? 1'b0:
				(part == 3'd0 & data == `end_tr) ? 1'b1 : 
			    (res_count == 4'd3) ? 1'b0 : end_flag;
end

/*generation of a signal for stop writing in RAM */
always @(posedge clk, negedge rst_)
  end_write <= (~rst_)? 1'b0 :
               (end_flag)? 1'b1 : end_write;
               
/*generation of a signal for changing address */
always @(posedge clk, negedge rst_)
  inc_addr <= (~rst_) ? 1'h0 :
              part == 3'h3 ? 1'b0 :
              part == 3'h4 ? 1'b1 : inc_addr; //thereby addr increases once 
                                              //during part == 4             
	
endmodule