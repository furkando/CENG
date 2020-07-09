`timescale 1ns / 1ps

module testbench_ic1500;

	// Inputs
	reg a0;
	reg f0;
	reg a1;
	reg f1;
	reg clk;

	// Outputs
	wire q0;
	wire q1;
	wire y;

	// Instantiate the Unit Under Test (UUT)
	ic1500 uut (
		.a0(a0), 
		.f0(f0), 
		.a1(a1), 
		.f1(f1), 
		.clk(clk), 
		.q0(q0), 
		.q1(q1), 
		.y(y)
	);
    // At 5, 15, 25, .. clk will change 1->0 
    // At 10, 20, 30, .. clk will change 0->1
    initial clk = 1;
	always #5 clk = ~clk;
    
	initial begin
		// Initialize Inputs
		a0 = 0;
		f0 = 0;
		a1 = 0;
		f1 = 0;

		// Wait 5 ns for global reset to finish
		#5;
        
		// Add stimulus here
		
	end
      
endmodule


