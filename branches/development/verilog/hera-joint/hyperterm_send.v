/* This module is just for testing module 'hyperterm' */
`define DIVIDER 14'd9 //should be 14'd9999 !!!!
module hyperterm_send
(
input  wire clk_48,
input  wire rst_,
output reg  tx,
input  wire dtr,
output wire dsr,
output wire cd,
input  wire rts,
output wire cts
);

assign dsr = dtr;
assign cd  = dtr;
assign cts = rts;

reg [13:0] count;

always @ ( posedge clk_48, negedge rst_ )
if ( ~rst_ )
	count <= 14'd0;
else
if ( count == `DIVIDER )
	count <= 14'd0;
else
	count <= count + 1'd1;

reg        clk;

always @ ( posedge clk_48, negedge rst_ )
if ( ~rst_ )
	clk <= 1'b0;
else
if ( count == `DIVIDER )
	clk <= ~clk;

reg  [3:0] tx_count;

reg  [7:0] msg[34:0];

always @ (posedge rst_)
begin
	msg[00] = "e";
	msg[01] = "7";
	msg[02] = "f";
	msg[03] = "9";
	msg[04] = " ";
	msg[05] = "f";
	msg[06] = "7";
	msg[07] = "7";
	msg[08] = "5";
	msg[09] = " ";
	msg[10] = "a";
	msg[11] = "4";
	msg[12] = "0";
	msg[13] = "0";
	msg[14] = " ";
	msg[15] = "a";
	msg[16] = "3";
	msg[17] = "0";
	msg[18] = "0";
	msg[19] = " ";
	msg[20] = "a";
	msg[21] = "5";
	msg[22] = "0";
	msg[23] = "0";
	msg[24] = " ";
	msg[25] = "e";
	msg[26] = "6";
	msg[27] = "0";
	msg[28] = "2";
	msg[29] = " ";
	msg[30] = "e";
	msg[31] = "e";
	msg[32] = "f";
	msg[33] = "f";
	msg[34] = 8'ha;
end

reg  [6:0] char_num;

always @ ( posedge clk, negedge rst_ )
if ( ~rst_ )
	char_num <= 7'd0;
else
if ( tx_count == 4'd9 )
	char_num <= char_num + 1'd1;

wire [7:0] char = msg[char_num];

always @ ( posedge clk, negedge rst_ )
if ( ~rst_ )
	tx_count <= 4'd0;
else
if ( tx_count == 4'd10 )
	tx_count <= 4'd0;
else
if ( ~char_num[6] | ( tx_count != 4'd00 ) )
	tx_count <= tx_count + 1'b1;

always @ ( posedge clk, negedge rst_ )
if ( ~rst_ )
	tx <= 1'b1;
else
case ( tx_count )
	4'd01:   tx <= 1'b0;
	4'd02:   tx <= char[0];
	4'd03:   tx <= char[1];
	4'd04:   tx <= char[2];
	4'd05:   tx <= char[3];
	4'd06:   tx <= char[4];
	4'd07:   tx <= char[5];
	4'd08:   tx <= char[6];
	4'd09:   tx <= char[7];
	default: tx <= 1'b1;
endcase

endmodule