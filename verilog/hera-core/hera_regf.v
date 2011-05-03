`default_nettype none
`timescale 1 ns / 1 ns

module hera_regf(
	input   wire        clk,
	input   wire        rst,
	input   wire [3:0]  rsa, //least part of instructoin
	input   wire [3:0]  rsb, //second  part of instr. 
	input   wire [3:0]  rd, // result addr
	input   wire        load_en,
	input   wire        call_en,
	input   wire  	   	return_en,
	input	wire		mul_en,
	input   wire [15:0] load,  
	input   wire [15:0] rd_data, // from  ALU to result addr
	input   wire [15:0] rd_temp, // temp reg used in mult, into r13
	output  wire [2:0]  load_flags, // availibe cary zero
	output  wire [15:0] rsa_data, 
	output  wire [15:0] rsb_data
	);

	reg [15:0] R0;
	reg [15:0] R1;
	reg [15:0] R2;
	reg [15:0] R3;
	reg [15:0] R4;
	reg [15:0] R5;
	reg [15:0] R6;
	reg [15:0] R7;
	reg [15:0] R8;
	reg [15:0] R9;
	reg [15:0] R10;
	reg [15:0] R11;
	reg [15:0] R12;
	reg [15:0] R13;
	reg [15:0] R14;
	reg [15:0] R15;
	parameter r0 = 4'b0,
		r1 = 4'b0001,
		r2 = 4'b0010,
		r3 = 4'b0011,
		r4 = 4'b0100,
		r5 = 4'b0101,
		r6 = 4'b0110,
		r7 = 4'b0111,
		r8 = 4'b1000,
		r9 = 4'b1001,
		r10 = 4'b1010,
		r11 = 4'b1011,
		r12 = 4'b1100,
		r13 = 4'b1101,
		r14 = 4'b1110,
		r15 = 4'b1111;

	reg [3:0] rd_pre;
	reg [15:0] rd_data_pre;
	reg [3:0] load_dir;
	reg loading;

	assign rsa_data = 	( loading == 1'b1 && load_en == 1'b0 && load_dir == rsa )?load:
						(rsa == r0)?R0:
						(rsa == r1)?R1:
						(rsa == r2)?R2:
						(rsa == r3)?R3:
						(rsa == r4)?R4:
						(rsa ==r5)?R5:
						(rsa == r6)?R6:
						(rsa == r7)?R7:
						(rsa == r8)?R8:
						(rsa == r9)?R9:
						(rsa == r10)?R10:
						(rsa == r11)?R11:
						(rsa == r12)?R12:
						(rsa == r13)?R13:
						(rsa == r14)?R14:
						(rsa == r15)?R15:16'b0;
	
	assign rsb_data = 	( loading == 1'b1 && load_en == 1'b0 && load_dir == rsb )?load:
						(rsb == r0)?R0:
						(rsb == r1)?R1:
						(rsb == r2)?R2:
						(rsb == r3)?R3:
						(rsb == r4)?R4:
						(rsb == r5)?R5:
						(rsb == r6)?R6:
						(rsb == r7)?R7:
						(rsb == r8)?R8:
						(rsb == r9)?R9:
						(rsb == r10)?R10:
						(rsb == r11)?R11:
						(rsb == r12)?R12:
						(rsb == r13)?R13:
						(rsb == r14)?R14:
						(rsb == r15)?R15:16'b0;
	assign load_flags = ( loading == 1'b0 )?1'b0:
						( load == 16'b0 )?{ 1'b1, load[15], 1'b0 }:
						{ 1'b1, load[15], 1'b1 };
	
  always@(  negedge rst or posedge clk ) begin
    if (~rst) begin
      R0 <= 16'b0;
      R1 <= 16'b0;
      R2 <= 16'b0;
      R3 <= 16'b0;
      R4 <= 16'b0;
      R5 <= 16'b0;
      R6 <= 16'b0;
      R7 <= 16'b0;
      R8 <= 16'b0;
      R9 <= 16'b0;
      R10 <= 16'b0;
      R11 <= 16'b0;
      R12 <= 16'b0;
      R13 <= 16'b0;
      R14 <= 16'b0;
      R15 <= 16'b0;
      load_dir <= 0;
      loading <= 0;
	  rd_pre <= 0;
	  rd_data_pre <= 0;
    end
	else if ( loading == 1'b1 ) begin
			if ( call_en == 1'b1 ) begin
				loading <=  1'b0;
				load_dir <= 0;
				R13 <= ( load_dir == r13 )?load:R14;
				R14 <= ( load_dir == r14 )?load:R15;
				R15 <= ( load_dir == r15 )?load:(R15 + rd_temp);
				case ( load_dir )
					r0: R0 <= 0;
					r1: R1 <= load;
					r2: R2 <= load;
					r3: R3 <= load;
					r4: R4 <= load;
					r5: R5 <= load;
					r6: R6 <= load;
					r7: R7 <= load;
					r8: R8 <= load;
					r9: R9 <= load;
					r10: R10 <= load;
					r11: R11 <= load;
					r12: R12 <= load;
					default: R0 <= 0;
				endcase
			end
			else if ( return_en == 1'b1) begin
					loading <=  1'b0;
					load_dir <= 0;
					R14 <= ( load_dir == r14 )?load:R13;
					R15 <= ( load_dir == r15 )?load:R14;
					case ( load_dir )
						r0: R0 <= 0;
						r1: R1 <= load;
						r2: R2 <= load;
						r3: R3 <= load;
						r4: R4 <= load;
						r5: R5 <= load;
						r6: R6 <= load;
						r7: R7 <= load;
						r8: R8 <= load;
						r9: R9 <= load;
						r10: R10 <= load;
						r11: R11 <= load;
						r12: R12 <= load;
						r13: R13 <= load;
						default: R0 <= 0;
					endcase
				end
				else begin
					if ( load_en == 1'b1 ) begin
						case ( load_dir )
							r0: R0 <= 0;
							r1: R1 <= load;
							r2: R2 <= load;
							r3: R3 <= load;
							r4: R4 <= load;
							r5: R5 <= load;
							r6: R6 <= load;
							r7: R7 <= load;
							r8: R8 <= load;
							r9: R9 <= load;
							r10: R10 <= load;
							r11: R11 <= load;
							r12: R12 <= load;
							r13: R13 <= load;
							r14: R14 <= load;
							r15: R15 <= load;
							default: R0 <= 0;
						endcase
						load_dir <= rd;
						loading <= 1'b1;
					end			
					else begin
						loading <=  1'b0;
						load_dir <= 0;
						if ( rd == load_dir ) begin
						  if ( mul_en == 1'b1 ) R13 <= rd_temp;
              else if ( rd == r13 ) R13 <= rd_data;
                  else R13 <= R13;
							case ( rd )
								r0: R0 <= 0;
								r1: R1 <= rd_data;
								r2: R2 <= rd_data;
								r3: R3 <= rd_data;
								r4: R4 <= rd_data;
								r5: R5 <= rd_data;
								r6: R6 <= rd_data;
								r7: R7 <= rd_data;
								r8: R8 <= rd_data;
								r9: R9 <= rd_data;
								r10: R10 <= rd_data;
								r11: R11 <= rd_data;
								r12: R12 <= rd_data;
								//r13: R13 <= rd_data;
								r14: R14 <= rd_data;
								r15: R15 <= rd_data;
								default: R0 <= 0;
							endcase
						end
						else begin
							case ( load_dir )
								r0: R0 <= 0;
								r1: R1 <= load;
								r2: R2 <= load;
								r3: R3 <= load;
								r4: R4 <= load;
								r5: R5 <= load;
								r6: R6 <= load;
								r7: R7 <= load;
								r8: R8 <= load;
								r9: R9 <= load;
								r10: R10 <= load;
								r11: R11 <= load;
								r12: R12 <= load;
								r13: R13 <= load;
								r14: R14 <= load;
								r15: R15 <= load;
								default: R0 <= 0;
							endcase
							case ( rd )
								r0: R0 <= 0;
								r1: R1 <= rd_data;
								r2: R2 <= rd_data;
								r3: R3 <= rd_data;
								r4: R4 <= rd_data;
								r5: R5 <= rd_data;
								r6: R6 <= rd_data;
								r7: R7 <= rd_data;
								r8: R8 <= rd_data;
								r9: R9 <= rd_data;
								r10: R10 <= rd_data;
								r11: R11 <= rd_data;
								r12: R12 <= rd_data;
								//r13: R13 <= rd_data;
								r14: R14 <= rd_data;
								r15: R15 <= rd_data;
								default: R0 <= 0;
							endcase
						end
					end
				end
		end
		else begin
			if ( load_en == 1'b1 ) begin
				load_dir <= rd;
				loading <= 1'b1;
			end
			else begin
				loading <= 1'b0;
				if ( call_en == 1'b1 ) begin
					R13 <= R14;
					R14 <= R15;
					R15 <= R15 + rd_temp;
				end
				else if ( return_en == 1'b1) begin
						R14 <= R13;
						R15 <= R14;
						R13 <= rd_temp;
					end
					else begin
						if ( mul_en == 1'b1 ) R13 <= rd_temp;
						else if ( rd == r13 ) R13 <= rd_data;
							else R13 <= R13;
						case ( rd )
							r0: R0 <= 0;
							r1: R1 <= rd_data;
							r2: R2 <= rd_data;
							r3: R3 <= rd_data;
							r4: R4 <= rd_data;
							r5: R5 <= rd_data;
							r6: R6 <= rd_data;
							r7: R7 <= rd_data;
							r8: R8 <= rd_data;
							r9: R9 <= rd_data;
							r10: R10 <= rd_data;
							r11: R11 <= rd_data;
							r12: R12 <= rd_data;
							// r13: R13 <= rd_data;
							r14: R14 <= rd_data;
							r15: R15 <= rd_data;
							default: R0 <= 0;
						endcase
					end
				end
			end
  end
endmodule