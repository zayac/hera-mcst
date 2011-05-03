//test bench for my HERA CPU
`timescale 1ps / 1ps
module tb();
//------------------------------------------
//input ports

reg clk;
reg rst;

//------------------------------------------
//output ports


//------------------------------------------

parameter halfperiod = 5;
parameter period = 10;
parameter twocycle = 20;
parameter delay = 100;

integer errors, cycles;

//------------------------------------------
//unit under test
hera hera(
	.clk(clk),
	.rst(rst),
	.out_test()
	);


//------------------------------------------
//clock generation
initial 
	begin
		clk=0;
		forever
			#halfperiod clk= ~clk;
	end
	
//cycles count
//begin
//	cycles = 0;
//	forever #period cycles = cycles + 1;
//end


//------------------------------------------
task set_reset;
	begin
		rst = 1;
		#twocycle;
		rst = 0;
		#twocycle;
		rst = 1;
	end
endtask

task current_op;
	begin
		//if (myhera.instr === {})
			$display ("Current operation instruction is %H", hera.q);
	end
endtask

task CheckFlags;
	begin
		$display("S:%B	|	Z:%B	|	V:%B	|	C:%B	|	CB:%B", hera.alu_top.sign, hera.alu_top.zero, hera.alu_top.overflow, hera.alu_top.carry, hera.alu_top.carry_block);
	end
endtask

task RegContent;
	begin
		$display("R0:%B", hera.hera_regf.R0);
		$display("R1:%B", hera.hera_regf.R1);
		$display("R2:%B", hera.hera_regf.R2);
		$display("R3:%B", hera.hera_regf.R3);
		$display("R4:%B", hera.hera_regf.R4);
		$display("R5:%B", hera.hera_regf.R5);
		$display("R6:%B", hera.hera_regf.R6);
		$display("R7:%B", hera.hera_regf.R7);
		$display("R8:%B", hera.hera_regf.R8);
		$display("R9:%B", hera.hera_regf.R9);
		$display("R10:%B", hera.hera_regf.R10);
		$display("R11:%B", hera.hera_regf.R11);
		$display("R12:%B", hera.hera_regf.R12);
		$display("R13:%B", hera.hera_regf.R13);
		$display("R14:%B", hera.hera_regf.R14);
		$display("R15:%B", hera.hera_regf.R15);
		$display("PC:%B", hera.pc);	
	end
endtask

initial
	begin
		set_reset;
		$display("1st:");
		current_op;
		CheckFlags;
		RegContent;
		#period;
		$display("2nd:");
		current_op;
		CheckFlags;
		RegContent;
		#period;
		$display("3rd:");
		current_op;
		CheckFlags;
		RegContent;
		#period;
		$display("4th:");
		current_op;
		CheckFlags;
		RegContent;
		#period;
//		$stop;
	end

endmodule