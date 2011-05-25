module vis_dev(
  input rst, //only for testing
  
  input wire [15:0] q, //the command to be analysed if it's HALT
  input wire [15:0] pc, 
  input wire [15:0] R0,
	input wire [15:0] R1,
	input wire [15:0] R2,
	input wire [15:0] R3,
	input wire [15:0] R4,
	input wire [15:0] R5,
	input wire [15:0] R6,
	input wire [15:0] R7,
	input wire [15:0] R8,
	input wire [15:0] R9,
	input wire [15:0] R10,
	input wire [15:0] R11,
	input wire [15:0] R12,
	input wire [15:0] R13,
	input wire [15:0] R14,
	input wire [15:0] R15,
	input wire [4:0] flags,
	
	input  wire clk_, //clock
	/**
	 * next wires are connected to appropriate pins of COM-port
	 **/
  output reg  tx, // data transmit 
  input  wire dtr, // data terminal ready
  output wire dsr, // data set ready
  output wire cd, // carrier detect
  input  wire rts, //request to send
  output wire cts, //clear to send
  
  /**
   * Memory connections
   **/
 	output	reg [5:0] address_a,
	output	reg [5:0] address_b,
	output	reg [31:0] data_a,
	output	reg [31:0] data_b,
	output	reg wren_a,
	output	reg wren_b,
	input	wire [31:0] q_a,
	input	wire [31:0] q_b,
	
  input wire [3:0] rsa, //least part of instruction
  input wire [3:0] rsb, //second  part of instruction 
  input wire [3:0] rsd // result address
  );

parameter DIVIDER = 14'd9999; //14'd9999 when testing on the maket plate.
parameter WHOLE_NUMBER_OF_16BIT_DATA = 16'd88; // how much information in 16bit we have to transmit
parameter NUMBER_OF_16BWORDS_INFO_OF_1COMMAND = 16'd10;

/**
 * TO-DO: parce "q" bus, then compare to HALT code,
 * so the initial state of "start" is 1'b1, when HALT is recieved,
 * start should be set as 1'b0 for a few time and then to 1'b1 again
 **/
 
/**
 * Before start algorithms "start" should be in 1'b0 state at least for 1 clk to initialize registers
 * To start working "start" goes into 1'b1
 **/
reg start; 

reg [3:0] char_num; // the number of sent bytes. 
reg [15:0] index;  // number of element in the array of registers
reg [15:0] R[WHOLE_NUMBER_OF_16BIT_DATA:0]; // array of registers
reg [15:0] D[NUMBER_OF_16BWORDS_INFO_OF_1COMMAND - 1:0]; // data of 1 command from buffer to be displayed
reg [7:0] digit; // hexical digit - 4 bits. each register is divided into 4 digits
reg mode; // conventor mode. description is with "module convent" one
wire [0:7] char; // transmitted data
reg  [3:0] tx_count; //number of transmited bits
reg eot_f;
reg [5:0] d_index; // number of command 
/**
 * establishing connection due COM-port
 **/ 
assign dsr = dtr;
assign cd  = dtr;
assign cts = rts;

reg [13:0] count; // number of counted clocks
reg clk; //internal clock, data are sent at the posedge of clk
/**
 * makes external clock "clk_" slower. New clock "clk" have frequency,
 * divided into parameter "DIVIDER" being set upper
 **/
always @ ( posedge clk_, negedge rst )
  if ( ~rst )
  	count <= 14'd0;
  else
    if ( count == DIVIDER ) 
     	count <= 14'd0;
    else
    	 count <= count + 1'd1;


always @ ( posedge clk_, negedge rst )
  if ( ~rst )
  	clk <= 1'b0;
  else
    if ( count == DIVIDER )
    	clk <= ~clk;
    	
/**
 * ONLY FOR TESTING
 * start goes into 1'b1 at counts number READY , 
 * goes 1'b0 at STEADY - initialization of regs, 
 * and 1'b1 when GO - next posedge clk_ will be the beginning of the algorythm;
 **/  	
parameter READY = 16'd10, STEADY = 16'd20, GO = DIVIDER + DIVIDER;
reg [15:0] counter; //sounts clocks to set "start" wire
reg flag_q;

wire flag_STEADY = (counter == STEADY) ? 1'b1 : 1'b0;
wire flag_GO = (counter == GO) ? 1'b1 : 1'b0;

always @(posedge clk_)
  if ( ( q == 16'd0) && ( ~flag_q))
    counter <= counter + 16'd1;
  else counter <= 1'b0;


always @ (posedge clk_, negedge rst)
begin
	if (~rst) 
		flag_q <= 1'b0;
	else if ( flag_GO )
		flag_q <= 1'b1;
	
end

always @ (posedge clk_, negedge rst)
begin
	if (~rst) 
		start <= 1'b1;
	else if ( flag_STEADY )
		start <= 1'b0;
	else if ( flag_GO )
		start <= 1'b1;
end

/**
 * Set operands as their values
 **/
reg [2:0] data_not_computed;
reg [3:0] rsa_1clk_b4;
reg [3:0] rsb_1clk_b4;
reg [3:0] rsd_1clk_b4;
reg [15:0] operand1;
reg [15:0] operand2;
reg [15:0] operand3;
reg [15:0] q_1clk_b4;
reg [15:0] q_2clk_b4;
reg [15:0] q_3clk_b4;

always @ ( posedge clk_, negedge rst)
  if (~rst)
    begin
      rsa_1clk_b4 <= 4'd0;
      rsb_1clk_b4 <= 4'd0;
      rsd_1clk_b4 <= 4'd0;
    end
  else
    begin
      rsa_1clk_b4 <= rsa;
      rsb_1clk_b4 <= rsb;
      rsd_1clk_b4 <= rsd;
    end


    
always @ ( posedge clk_)//, negedge rst)
    begin
    case ( rsd_1clk_b4)
      4'd0: operand1 <= R0;
      4'd1: operand1 <= R1;
      4'd2: operand1 <= R2;
      4'd3: operand1 <= R3;
      4'd4: operand1 <= R4;
      4'd5: operand1 <= R5;
      4'd6: operand1 <= R6;
      4'd7: operand1 <= R7;
      4'd8: operand1 <= R8;
      4'd9: operand1 <= R9;
      4'd10: operand1 <= R10;
      4'd11: operand1 <= R11;
      4'd12: operand1 <= R12;
      4'd13: operand1 <= R13;
      4'd14: operand1 <= R14;
      4'd15: operand1 <= R15;
   endcase

   case (rsa)//( rsa_1clk_b4)
      4'd0: operand2 <= R0;
      4'd1: operand2 <= R1;
      4'd2: operand2 <= R2;
      4'd3: operand2 <= R3;
      4'd4: operand2 <= R4;
      4'd5: operand2 <= R5;
      4'd6: operand2 <= R6;
      4'd7: operand2 <= R7;
      4'd8: operand2 <= R8;
      4'd9: operand2 <= R9;
      4'd10: operand2 <= R10;
      4'd11: operand2 <= R11;
      4'd12: operand2 <= R12;
      4'd13: operand2 <= R13;
      4'd14: operand2 <= R14;
      4'd15: operand2 <= R15;
   endcase
   case (rsb)//( rsb_1clk_b4)
      4'd0: operand3 <= R0;
      4'd1: operand3 <= R1;
      4'd2: operand3 <= R2;
      4'd3: operand3 <= R3;
      4'd4: operand3 <= R4;
      4'd5: operand3 <= R5;
      4'd6: operand3 <= R6;
      4'd7: operand3 <= R7;
      4'd8: operand3 <= R8;
      4'd9: operand3 <= R9;
      4'd10: operand3 <= R10;
      4'd11: operand3 <= R11;
      4'd12: operand3 <= R12;
      4'd13: operand3 <= R13;
      4'd14: operand3 <= R14;
      4'd15: operand3 <= R15;
   endcase   
   end
   

always @ (posedge clk_, negedge rst)
  if (~rst)
    begin
      q_1clk_b4 <= q;
      q_2clk_b4 <= q;
      q_3clk_b4 <= q;
    end
  else
    begin
      q_1clk_b4 <= q;
      q_2clk_b4 <= q_1clk_b4;
      q_3clk_b4 <= q_2clk_b4;
    end

reg [15:0] flag1, flag2, flag3, flag4, flag5;
always @ ( posedge clk_)
begin
  //that all makes flags 0000 0000 0011 000(1/0) = 48 or 49 in ASCII
  flag1[0] <= flags[0]; // sign
  flag2[0] <= flags[1]; // zero
  flag3[0] <= flags[2]; // overflow
  flag4[0] <= flags[3]; // carry
  flag5[0] <= flags[4]; // carry_block
  
  flag1[3:1] <= 3'b000;
  flag2[3:1] <= 3'b000;
  flag3[3:1] <= 3'b000;
  flag4[3:1] <= 3'b000;
  flag5[3:1] <= 3'b000;
  
  flag1[5:4] <= 2'b11;
  flag2[5:4] <= 2'b11;
  flag3[5:4] <= 2'b11;
  flag4[5:4] <= 2'b11;
  flag5[5:4] <= 2'b11;
  
  flag1[15:6] <= 3'b000;
  flag2[15:6] <= 3'b000;
  flag3[15:6] <= 3'b000;
  flag4[15:6] <= 3'b000;
  flag5[15:6] <= 3'b000;
  
end

wire [15:0] part1;
wire [15:0] part2;
wire [15:0] part3;

command_translator command_translator(
.q (q_a[31:16]),
.part1 (part1[15:0]),
.part2 (part2[15:0]),
.part3 (part3[15:0]),
.clk (clk_)
);

always @ ( posedge clk_)
begin
  R[0] <= "R0";
  R[1] <= "= ";
  R[2] <= R0;
  R[3] <= 16'h0A0D; // #10#13
  R[4] <= "R1";
  R[5] <= "= ";
  R[6] <= R1;
  R[7] <= 16'h0A0D; // #10#13
  R[8] <= "R2";
  R[9] <= "= ";
  R[10] <= R2;
  R[11] <= 16'h0A0D; // #10#13
  R[12] <= "R3";
  R[13] <= "= ";
  R[14] <= R3;
  R[15] <= 16'h0A0D; // #10#13
  R[16] <= "R4";
  R[17] <= "= ";
  R[18] <= R4;
  R[19] <= 16'h0A0D; // #10#13
  R[20] <= "R5";
  R[21] <= "= ";
  R[22] <= R5;
  R[23] <= 16'h0A0D; // #10#13
  R[24] <= "R6";
  R[25] <= "= ";
  R[26] <= R6;
  R[27] <= 16'h0A0D; // #10#13
  R[28] <= "R7";
  R[29] <= "= ";
  R[30] <= R7;
  R[31] <= 16'h0A0D; // #10#13
  R[32] <= "R8";
  R[33] <= "= ";
  R[34] <= R8;
  R[35] <= 16'h0A0D; // #10#13
  R[36] <= "R9";
  R[37] <= "= ";
  R[38] <= R9;
  R[39] <= 16'h0A0D; // #10#13
  R[40] <= "R1";
  R[41] <= "0=";
  R[42] <= R10;
  R[43] <= 16'h0A0D; // #10#13
  R[44] <= "R1";
  R[45] <= "1=";
  R[46] <= R11;
  R[47] <= 16'h0A0D; // #10#13
  R[48] <= "R1";
  R[49] <= "2=";
  R[50] <= R12;
  R[51] <= 16'h0A0D; // #10#13
  R[52] <= "R1";
  R[53] <= "3=";
  R[54] <= R13;
  R[55] <= 16'h0A0D; // #10#13
  R[56] <= "R1";
  R[57] <= "4=";
  R[58] <= R14;
  R[59] <= 16'h0A0D; // #10#13
  R[60] <= "R1";
  R[61] <= "5=";
  R[62] <= R15;
  R[63] <= 16'h0A0D; // #10#13
  R[64] <= "PC";
  R[65] <= "= ";
  R[66] <= pc;
  R[67] <= 16'h0A0D; // #10#13
  R[68] <= "S ";
  R[69] <= "= ";
  R[70] <= flag1;
  R[71] <= 16'h0A0D; // #10#13
  R[72] <= "Z ";
  R[73] <= "= ";
  R[74] <= flag2;
  R[75] <= 16'h0A0D; // #10#13
  R[76] <= "O ";
  R[77] <= "= ";
  R[78] <= flag3;
  R[79] <= 16'h0A0D; // #10#13
  R[80] <= "C ";
  R[81] <= "= ";
  R[82] <= flag4;
  R[83] <= 16'h0A0D; // #10#13
  R[84] <= "CB";
  R[85] <= "= ";
  R[86] <= flag5;
  R[87] <= 16'h0A0D; // #10#13
  /*
  R[0] <= "He";
  R[1] <= "ll";
  R[2] <= "o ";
  R[3] <= 16'h0A0D;*/

/*  D[0] <= q_a[31:16];
  D[1] <= ": ";
  D[2] <= q_a[15:0];
  D[3] <= ", ";
  D[4] <= q_b[31:16];
  D[5] <= ", ";
  D[6] <= q_b[15:0];
  D[7] <= 16'h0A0D;*/
  D[0] <= part1;
  D[1] <= part2;
  D[2] <= part3;
  D[3] <= ": ";
  D[4] <= q_a[15:0];
  D[5] <= ", ";
  D[6] <= q_b[31:16];
  D[7] <= ", ";
  D[8] <= q_b[15:0];
  D[9] <= 16'h0A0D;
end

/**
 * Due to data "mode" must be 0 each zero, first and third index and 1 each second index
 **/
//reg [1:0] the_rest;
reg end_of_regout; //  while outputing final registers state it is 0.

always @ (posedge clk)
begin
//  the_rest <= index[1:0];
  if ( (~start ) || ( ~rst))
    mode <= 1'b0;
  else
   if (~end_of_regout) // ordinary R[*] transmitting rules
      case ( index[1:0])
       2'd0: mode <= 1'b0;
       2'd1: mode <= 1'b0;
       2'd2: 
        begin
          case ( index)
            16'd70: mode <= 1'b0;
            16'd74: mode <= 1'b0;
            16'd78: mode <= 1'b0;
            16'd82: mode <= 1'b0;
            16'd86: mode <= 1'b0;
            default mode <= 1'b1;
          endcase
        end    
       2'd3: mode <= 1'b0;
      endcase
    else // rules of displaying commands
      case ( d_index)
        6'd0: mode <= 1'b0;
        6'd1: mode <= 1'b0;
        6'd2: mode <= 1'b0;
        6'd3: mode <= 1'b0;
        6'd4: mode <= 1'b1;
        6'd5: mode <= 1'b0;
        6'd6: mode <= 1'b1;
        6'd7: mode <= 1'b0;
        6'd8: mode <= 1'b1;
        6'd9: mode <= 1'b0;
      endcase
end
  
/**
 * MUX, that changes the element in R[] to be divided into digits.
 * Marks the end of transmition with flag "eot"
 **/
reg [5:0] read_adr;
reg eot;
always @ ( posedge clk)
  if ( (~start) || ( ~rst))
    begin
     index <= 16'd0;
     eot <= 1'b0;
     end_of_regout <= 1'b0;
     d_index <= 6'd0;
     read_adr <= 6'd0;
    end
  else
    begin
     if ( eot) 
        end_of_regout <= 1'b0;
     if ( eot_f)
        eot <= 1'b1;
     if ( mode)
        if (( char_num == 5'd3) && ( tx_count == 4'd10))
           if ( index != WHOLE_NUMBER_OF_16BIT_DATA - 1)
              index <= index + 16'd1;
           else 
              begin
                if (~end_of_regout) 
                  end_of_regout <= 1'b1;  
                else
                if ( d_index != NUMBER_OF_16BWORDS_INFO_OF_1COMMAND - 1)
                  d_index <= d_index + 6'd1;   
                else    
                  begin
                    d_index <= 6'd0;
                    read_adr <= read_adr + 6'd2;
                  end
              end
     if ( ~mode) // so there are 2 ASCII symbols in 16-bit registers to be sent
         if (( char_num == 5'd1) && ( tx_count == 4'd10))
           if ( index != WHOLE_NUMBER_OF_16BIT_DATA - 1)
              index <= index + 16'd1;
           else 
              begin
                if (~end_of_regout) 
                  end_of_regout <= 1'b1;  
                else
                if ( d_index != NUMBER_OF_16BWORDS_INFO_OF_1COMMAND - 1)       
                  d_index <= d_index + 6'd1;   
                else   
                  begin 
                    d_index <= 6'd0;  
                    read_adr <= read_adr + 6'd2;
                  end             
              end
    end

/**
 * Marks the end of transmition with flag "eot"
 **/
reg [15:0] tmp_r;
always @ (posedge clk_) 
begin
  if ( ~end_of_regout) 
    tmp_r <= R[index]; 
  else
    tmp_r <= D[d_index];
end


/**
 * MUX, changing digits to be sent. It switches after previous digit was fully sent, so
 * char_num goes to be incremented
 **/

always @ ( posedge clk)//char_num)
    if ( mode)
      case ( char_num)
        5'd0: digit[3:0] <= tmp_r[15:12];
        5'd1: digit[3:0] <= tmp_r[11:8];
        5'd2: digit[3:0] <= tmp_r[7:4];
        5'd3: digit[3:0] <= tmp_r[3:0];
        default: digit <= 4'b0000;
       endcase 
    else
      case ( char_num)
        5'd0: digit[7:0] <= tmp_r[15:8];
        5'd1: digit[7:0] <= tmp_r[7:0];
        default: digit <= 8'b0000;
      endcase 

/**
 * instance to transfer digit into ASCII
 **/
convent convent(
.digit (digit[7:0]),
.mode (mode), // mode is described with description of convert below.
.out (char[0:7]), //inverted order due to the order of transmitting bits lower
.clock (clk_)
); 

/**
 * Each external clock - so each machine operation - we should put debugging data - "q" and appropriate registers' values into 
 * RAM via 2 * 32-bit buses.
 */
reg filling_buffer;
always @ ( posedge clk_, negedge rst)
 if (~rst) 
   begin
     wren_a <= 1'b0;
     wren_b <= 1'b0;
   end 
  else
    if ( filling_buffer)
      begin
        if ( data_not_computed == 3'b010)
          begin
            wren_a <= 1'b1;
            wren_b <= 1'b1;
          end         
      end
    else
    begin
      wren_a <= 1'b0;
      wren_b <= 1'b0;
    end      
/*    if (~start)
      begin
       wren_a <= 1'b1;
       wren_b <= 1'b1;       
      end
    else 
      if ( end_of_regout)
        begin
          wren_a <= 1'b0;
          wren_b <= 1'b0;
        end*/


reg [2:0] few_time_to_put_data_to_mem;

always @ ( posedge clk_, negedge rst)
  if (~rst)
    begin
      filling_buffer <= 1'b1;
      few_time_to_put_data_to_mem <= 3'b000;
    end
  else 
    begin
    if ( q == 16'd0)
      begin
        if ( few_time_to_put_data_to_mem == 3'b010) 
          begin
            filling_buffer <= 1'b0;
          end
        else
          few_time_to_put_data_to_mem <= few_time_to_put_data_to_mem + 3'b001;
      end
    end

reg [5:0] last_address;
reg is_wrens_switched;
reg end_of_regout_f;

always @ ( posedge clk_, negedge rst)
 if (~rst) 
   begin
     address_a <= 6'd0;
     address_b <= 6'd1;
     data_not_computed <= 3'b000;
     last_address <= 6'd0;
     is_wrens_switched <= 1'd0;
     eot_f <= 1'b0;
     end_of_regout_f <= 1'b0;
   end 
  else
    begin
      if ( filling_buffer) //case alu works
        begin
          data_a <= {q_2clk_b4, operand1}; 
          data_b <= {operand2, operand3};
          if ( data_not_computed == 3'b011)
            begin
              address_a <= address_a + 6'd2;
              address_b <= address_b + 6'd2;
              last_address <= last_address + 6'd2;
            end
          else
            data_not_computed <= data_not_computed + 3'b001;
        end
      else //case there was HALT
        begin
          if ( end_of_regout)
            if ( ~end_of_regout_f)
            begin
              is_wrens_switched <= 1'd1;
              address_a <= 6'd0;
              address_b <= 6'd1; 
              end_of_regout_f <= 1'b1;                      
            end
          if ( is_wrens_switched) 
            if ( address_a != (last_address + 6'd2))
              begin
                address_a <= read_adr;
                address_b <= read_adr+ 6'd1;
              end
            else
              eot_f <= 1'b1;
        end
    end    

/**
 * The counter of thansmitted packets - 11bits length
 * 0,1st and 10th are constants, 2-9 = 8bit - the data
 * to be interpreted by Hyperterminal as an ASCII symbol.
 * Due to the algorythm char_num goes 0,1,2,3,0
 **/
 

always @ ( posedge clk)
if ( (~start ) || ( ~rst))
	char_num <= 4'd0;
else
if ( tx_count == 4'd10 )
  if ( mode) // when in 16bit data there are 4 digits to be converted into ASCII symbols 
    if ( char_num == 4'd3) 
      char_num <= 4'd0;
    else
  	  char_num <= char_num + 4'd1;
 	else // when in 16bit data there are 2 raw ASCII symbols to be transmitted
 	   if ( char_num == 4'd1) 
      char_num <= 4'd0;
    else
  	  char_num <= char_num + 4'd1;

/**
 * Counts transmitted bits including service once
 * the counting variable is "tx_count" in range [0..10].
 * Bits being transmitted each posedge clk
 * "eot" is a flag. it is 1'b1 when the tranmition is over.
 * in that case there is always 1 at data pin "tx".
 **/


always @ ( posedge clk)
if ( (~start ) || ( ~rst))
	tx_count <= 4'd0;
else
  if ( tx_count == 4'd10 )
  	tx_count <= 4'd0;
  else
    if ( ~eot | tx_count != 4'd00 )
      if (~eot_f)
        tx_count <= tx_count + 1'b1;
      else
        tx_count <= 4'd00;
	   
/**
 * Transmits bit-by-bit each posedge clock
 **/
 
 
always @ ( posedge clk)
if ( (~start ) || ( ~rst))
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

/**
 * Converts input hexical digit in format of 4-bit bus into ASCII 8-bit number 
 **/
/** 
 * !!! IMPORTANT !!! If "mode" is 1 then the module converts as it should,
 * if "mode" == 0, then the module gives raw value as an ouput.
 * The mode is invented to simplify output of characters
 * So when you need to transfer char "!", you set mode as 0, 
 * tmp_r <= 16'h0021, so there will be transmited "0x00" and "0x21",
 * which have to be interpreted by hyperterm as "!"
 * Mode also defines how much bits of "digit" should be used
 **/

module convent(
  input wire [7:0] digit,
  input wire mode,
  output wire [7:0] out,
  input wire clock
  );

  reg [7:0] out_r;
  assign out = out_r;
  always @ ( posedge clock)
  if ( mode)
    case ( digit[3:0] )
      4'd0: out_r <= 8'd48;
      4'd1: out_r <= 8'd49;
      4'd2: out_r <= 8'd50;
      4'd3: out_r <= 8'd51;
      4'd4: out_r <= 8'd52;
      4'd5: out_r <= 8'd53;
      4'd6: out_r <= 8'd54;
      4'd7: out_r <= 8'd55;
      4'd8: out_r <= 8'd56;
      4'd9: out_r <= 8'd57;
      4'd10: out_r <= 8'd65;
      4'd11: out_r <= 8'd66;
      4'd12: out_r <= 8'd67;
      4'd13: out_r <= 8'd68;
      4'd14: out_r <= 8'd69;
      4'd15: out_r <= 8'd70;
    endcase
  else
    out_r <= digit;
endmodule

module command_translator(
input wire [15:0] q,
output wire [15:0] part1,
output wire [15:0] part2,
output wire [15:0] part3,
input clk
);
reg [47:0] word;
assign part1 = word[47:32], part2 = word[31:16], part3 = word[15:0];

always @ (posedge clk)
begin
case (q[15:12])
  4'hE: word <= "SETLO";
  4'hF: word <= "SETHI";
  4'h8: word <= "AND";
  4'h9: word <= "OR";
  4'hD: word <= "XOR";
  4'hA: word <= "ADD";
  4'hB: word <= "SUB";
  4'hC: word <= "MULT";
endcase

if (q[15:12] == 4'h3)
case (q[7:4])
  4'h0: word <= "LSL";
  4'h1: word <= "LSR";
  4'h2: word <= "LSL8";
  4'h3: word <= "LSR8";
  4'h4: word <= "ASL";
  4'h5: word <= "ASR";
  4'h6: if ( q[11] == 1'b1) word <= "SETF";
        else word <= "CLRF";
  4'h7: if ( q[3:0] == 4'h8) word <= "SAVEF";
        else word <= "RSTRF";
endcase

case (q[15:4])
  12'h100: word <= "BR";
  12'h120: word <= "BL";
  12'h130: word <= "BGE";
  12'h140: word <= "BLE";
  12'h150: word <= "BG";
  12'h160: word <= "BULE";
  12'h170: word <= "BUG";
  12'h180: word <= "BZ";
  12'h190: word <= "BNZ";
  12'h1A0: word <= "BC";
  12'h1B0: word <= "BNC";
  12'h1C0: word <= "BS";
  12'h1D0: word <= "BNS";
  12'h1E0: word <= "BV";
  12'h1F0: word <= "BNV";
endcase

if (q[15:13] == 3'b010) word <= "LOAD";
if (q[15:13] == 3'b011) word <= "STORE";
if (q[15:8] == 8'h00) word <= "BRR";
if (q[15:8] == 8'h08) word <= "BZR";
if (q[15:8] == 8'h09) word <= "BNZR";

if (q[15:12] == 4'h2) word <= "CALL";
if (q[15:4] == 12'h110) word <= "SWI";  
if (q == 16'h1111) word <= "RETURN";
if (q == 16'h1110) word <= "RTI";
if (q == 16'h0000) word <= "HALT";
end

endmodule
