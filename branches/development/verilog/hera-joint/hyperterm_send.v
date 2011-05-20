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

reg  [7:0] msg[278:0];

always @ (posedge rst_)
begin
msg[000] = "e";
msg[001] = "7";
msg[002] = "f";
msg[003] = "9";
msg[004] = " ";
msg[005] = "f";
msg[006] = "7";
msg[007] = "7";
msg[008] = "5";
msg[009] = " ";
msg[010] = "a";
msg[011] = "4";
msg[012] = "0";
msg[013] = "0";
msg[014] = " ";
msg[015] = "a";
msg[016] = "3";
msg[017] = "0";
msg[018] = "0";
msg[019] = " ";
msg[020] = "a";
msg[021] = "5";
msg[022] = "0";
msg[023] = "0";
msg[024] = " ";
msg[025] = "e";
msg[026] = "6";
msg[027] = "0";
msg[028] = "2";
msg[029] = " ";
msg[030] = "f";
msg[031] = "6";
msg[032] = "0";
msg[033] = "0";
msg[034] = " ";
msg[035] = "e";
msg[036] = "8";
msg[037] = "0";
msg[038] = "1";
msg[039] = " ";
msg[040] = "f";
msg[041] = "8";
msg[042] = "0";
msg[043] = "0";
msg[044] = " ";
msg[045] = "e";
msg[046] = "6";
msg[047] = "0";
msg[048] = "2";
msg[049] = " ";
msg[050] = "f";
msg[051] = "6";
msg[052] = "0";
msg[053] = "0";
msg[054] = " ";
msg[055] = "e";
msg[056] = "c";
msg[057] = "3";
msg[058] = "1";
msg[059] = " ";
msg[060] = "f";
msg[061] = "c";
msg[062] = "0";
msg[063] = "0";
msg[064] = " ";
msg[065] = "1";
msg[066] = "0";
msg[067] = "0";
msg[068] = "c";
msg[069] = " ";
msg[070] = "e";
msg[071] = "8";
msg[072] = "0";
msg[073] = "1";
msg[074] = " ";
msg[075] = "f";
msg[076] = "8";
msg[077] = "0";
msg[078] = "0";
msg[079] = " ";
msg[080] = "e";
msg[081] = "c";
msg[082] = "2";
msg[083] = "2";
msg[084] = " ";
msg[085] = "f";
msg[086] = "c";
msg[087] = "0";
msg[088] = "0";
msg[089] = " ";
msg[090] = "1";
msg[091] = "0";
msg[092] = "0";
msg[093] = "c";
msg[094] = " ";
msg[095] = "c";
msg[096] = "c";
msg[097] = "6";
msg[098] = "8";
msg[099] = " ";
msg[100] = "3";
msg[101] = "0";
msg[102] = "6";
msg[103] = "8";
msg[104] = " ";
msg[105] = "b";
msg[106] = "0";
msg[107] = "c";
msg[108] = "7";
msg[109] = " ";
msg[110] = "0";
msg[111] = "9";
msg[112] = "0";
msg[113] = "8";
msg[114] = " ";
msg[115] = "a";
msg[116] = "4";
msg[117] = "6";
msg[118] = "0";
msg[119] = " ";
msg[120] = "a";
msg[121] = "3";
msg[122] = "8";
msg[123] = "0";
msg[124] = " ";
msg[125] = "e";
msg[126] = "5";
msg[127] = "0";
msg[128] = "1";
msg[129] = " ";
msg[130] = "f";
msg[131] = "5";
msg[132] = "0";
msg[133] = "0";
msg[134] = " ";
msg[135] = "e";
msg[136] = "c";
msg[137] = "2";
msg[138] = "5";
msg[139] = " ";
msg[140] = "f";
msg[141] = "c";
msg[142] = "0";
msg[143] = "0";
msg[144] = " ";
msg[145] = "1";
msg[146] = "0";
msg[147] = "0";
msg[148] = "c";
msg[149] = " ";
msg[150] = "e";
msg[151] = "c";
msg[152] = "0";
msg[153] = "1";
msg[154] = " ";
msg[155] = "f";
msg[156] = "c";
msg[157] = "0";
msg[158] = "0";
msg[159] = " ";
msg[160] = "3";
msg[161] = "8";
msg[162] = "6";
msg[163] = "8";
msg[164] = " ";
msg[165] = "a";
msg[166] = "8";
msg[167] = "8";
msg[168] = "c";
msg[169] = " ";
msg[170] = "3";
msg[171] = "0";
msg[172] = "6";
msg[173] = "8";
msg[174] = " ";
msg[175] = "b";
msg[176] = "0";
msg[177] = "8";
msg[178] = "7";
msg[179] = " ";
msg[180] = "0";
msg[181] = "4";
msg[182] = "e";
msg[183] = "f";
msg[184] = " ";
msg[185] = "e";
msg[186] = "c";
msg[187] = "0";
msg[188] = "1";
msg[189] = " ";
msg[190] = "f";
msg[191] = "c";
msg[192] = "0";
msg[193] = "0";
msg[194] = " ";
msg[195] = "3";
msg[196] = "0";
msg[197] = "6";
msg[198] = "8";
msg[199] = " ";
msg[200] = "b";
msg[201] = "0";
msg[202] = "5";
msg[203] = "c";
msg[204] = " ";
msg[205] = "0";
msg[206] = "9";
msg[207] = "0";
msg[208] = "4";
msg[209] = " ";
msg[210] = "e";
msg[211] = "c";
msg[212] = "3";
msg[213] = "4";
msg[214] = " ";
msg[215] = "f";
msg[216] = "c";
msg[217] = "0";
msg[218] = "0";
msg[219] = " ";
msg[220] = "1";
msg[221] = "0";
msg[222] = "0";
msg[223] = "c";
msg[224] = " ";
msg[225] = "e";
msg[226] = "c";
msg[227] = "0";
msg[228] = "1";
msg[229] = " ";
msg[230] = "f";
msg[231] = "c";
msg[232] = "0";
msg[233] = "0";
msg[234] = " ";
msg[235] = "3";
msg[236] = "8";
msg[237] = "6";
msg[238] = "8";
msg[239] = " ";
msg[240] = "a";
msg[241] = "6";
msg[242] = "6";
msg[243] = "c";
msg[244] = " ";
msg[245] = "3";
msg[246] = "0";
msg[247] = "6";
msg[248] = "8";
msg[249] = " ";
msg[250] = "b";
msg[251] = "0";
msg[252] = "6";
msg[253] = "7";
msg[254] = " ";
msg[255] = "0";
msg[256] = "4";
msg[257] = "d";
msg[258] = "b";
msg[259] = " ";
msg[260] = "e";
msg[261] = "1";
msg[262] = "0";
msg[263] = "1";
msg[264] = " ";
msg[265] = "f";
msg[266] = "1";
msg[267] = "0";
msg[268] = "0";
msg[269] = " ";
msg[270] = "1";
msg[271] = "1";
msg[272] = "1";
msg[273] = "1";
msg[274] = " ";
msg[275] = "0";
msg[276] = "0";
msg[277] = "0";
msg[278] = "0";

end

reg  [11:0] char_num;

always @ ( posedge clk, negedge rst_ )
if ( ~rst_ )
	char_num <= 12'd0;
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
if ( ~char_num[11] | ( tx_count != 4'd00 ) )
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