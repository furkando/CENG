`timescale 1ns / 1ps

module af(
    input a,
    input f,
    input clk,
    output reg q
    );
    
    initial begin
        q = 0;
    end
	 
    // write your code here
	 
	 always @(posedge clk)
	 begin
		q <= a ? (~f):(f ? q : ~q);
	 end
		
endmodule


module ic1500(
    input a0, 
    input f0, 
    input a1, 
    input f1, 
    input clk, 
    output q0, 
    output q1, 
    output y
    );
    // write your code here
	 
	 af output1(a0,f0,clk,q0) ; 
	 af output2(a1,f1,clk,q1) ;
	 assign y = q0^q1 ;
	
endmodule
