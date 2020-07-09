`timescale 1ns / 1ps

module testbench_RAM(
    );
	 
	reg mode;
	reg [3:0] ramAddress;
	reg [4:0] dataIn;
	reg op;
	reg [1:0] arg;
	reg CLK;
	wire[8:0] dataOut;
	integer result = 0;
	 
	FUNCRAM uut(
		.mode(mode),
		.ramAddress(ramAddress),
		.dataIn(dataIn),
		.op(op),
		.arg(arg),
		.CLK(CLK),
		.dataOut(dataOut)
	);

	initial CLK=1;
	always #5 CLK = ~CLK;

	initial begin
	
		mode = 0;
		ramAddress = 0;
		dataIn = 0;
		op = 0;
		arg = 0;
		CLK = 0;
		
		#5
	
		$display("Starting Testbench");
		
		$monitor("Time=%t  | mode=%b  | ramAddress=%b  | dataIn=%b  | op=%b  | arg=%b  | dataOut=%b", $time, mode, ramAddress, dataIn, op, arg, dataOut);

		mode=1;ramAddress=4'b0000;dataIn=5'b00000;op=0;arg=2'b00;//P(2)%7
				#10
				mode=0;ramAddress=4'b0000;dataIn=5'b00000;op=0;arg=2'b00;//P(2)%7

		#10;
		if(dataOut != 9'b000000011) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		mode=1;ramAddress=4'b0000;dataIn=5'b00000;op=1;arg=2'b01;//P'(1)
				#10

		mode=0;ramAddress=4'b0000;dataIn=5'b00000;op=1;arg=2'b01;//P'(1)
		
		#10;
		if(dataOut != 9'b000001010) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		
		mode=1;ramAddress=4'b1111;dataIn=5'b11111;op=1;arg=2'b00;//P'(2)
						#10

				mode=0;ramAddress=4'b1111;dataIn=5'b11111;op=1;arg=2'b00;//P'(2)

		#10;
		if(dataOut != 9'b100110001) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		
		mode=1;ramAddress=4'b0001;dataIn=5'b00010;op=1;arg=2'b01;//P'(1) = 8
						#10

				mode=0;ramAddress=4'b0001;dataIn=5'b00010;op=1;arg=2'b01;//P'(1) = 8

		#10;
		if(dataOut != 9'b000001000) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		
		mode=1;ramAddress=4'b0010;dataIn=5'b00100;op=0;arg=2'b10;//P(-1)%7 = -1
						#10

				mode=0;ramAddress=4'b0010;dataIn=5'b00100;op=0;arg=2'b10;//P(-1)%7 = -1

		#10;
		if(dataOut != 9'b000000110) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		
		mode=1;ramAddress=4'b0100;dataIn=5'b01010;op=1;arg=2'b11;//P'(-2) = -46
						#10

				mode=0;ramAddress=4'b0100;dataIn=5'b01010;op=1;arg=2'b11;//P'(-2) = -46

		#10;
		if(dataOut != 9'b100110001) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		
		mode=1;ramAddress=4'b0110;dataIn=5'b01101;op=0;arg=2'b00;//P(2)%7 = 5
						#10

				mode=0;ramAddress=4'b0110;dataIn=5'b01101;op=0;arg=2'b00;//P(2)%7 = 5

		#10;
		if(dataOut != 9'b000000101) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		
		mode=1;ramAddress=4'b1001;dataIn=5'b10010;op=1;arg=2'b01;//P'(1) = 0
						#10

				mode=0;ramAddress=4'b1001;dataIn=5'b10010;op=1;arg=2'b01;//P'(1) = 0

		#10;
		if(dataOut != 9'b000000000) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		
		mode=1;ramAddress=4'b1100;dataIn=5'b11001;op=0;arg=2'b10;//P(-1)%7 = -1
						#10

				mode=0;ramAddress=4'b1100;dataIn=5'b11001;op=0;arg=2'b10;//P(-1)%7 = -1

		#10;
		if(dataOut != 9'b000000110) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		
		mode=1;ramAddress=4'b0011;dataIn=5'b00111;op=0;arg=2'b01;//P(1)%7
		#10
		mode=0;ramAddress=4'b0011;dataIn=5'b00111;op=0;arg=2'b01;//P(1)%7
		#10;
		if(dataOut != 9'b000000110) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		
		mode=1;ramAddress=4'b0101;dataIn=5'b01011;op=1;arg=2'b10;//P'(-1)
		#10
		mode=0;ramAddress=4'b0101;dataIn=5'b01011;op=1;arg=2'b10;//P'(-1)
		#10;
		if(dataOut != 9'b100001010) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		mode=1;ramAddress=4'b0111;dataIn=5'b01110;op=1;arg=2'b00;//P'(2)
		#10
		mode=0;ramAddress=4'b0111;dataIn=5'b01110;op=1;arg=2'b00;//P'(2)
		#10;
		if(dataOut != 9'b000001111) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		mode=1;ramAddress=4'b1000;dataIn=5'b10001;op=1;arg=2'b11;//P'(-2) 
		#10
		mode=0;ramAddress=4'b1000;dataIn=5'b10001;op=1;arg=2'b11;//P'(-2) 
		#10;
		if(dataOut != 9'b000101001) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		mode=1;ramAddress=4'b1010;dataIn=5'b10100;op=0;arg=2'b10;//P(-2)%7 
		#10
		mode=0;ramAddress=4'b1010;dataIn=5'b10100;op=0;arg=2'b10;//P(-2)%7 
		#10;
		if(dataOut != 9'b000000100) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		mode=1;ramAddress=4'b1101;dataIn=5'b11010;op=1;arg=2'b11;//P'(-2) 
		#10
		mode=0;ramAddress=4'b1101;dataIn=5'b11010;op=1;arg=2'b11;//P'(-2) 
		#10;
		if(dataOut != 9'b000001111) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		mode=1;ramAddress=4'b1110;dataIn=5'b11110;op=0;arg=2'b00;//P(2)%7 
		#10
		mode=0;ramAddress=4'b1110;dataIn=5'b11110;op=0;arg=2'b00;//P(2)%7 
		#10;
		if(dataOut != 9'b000000110) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		mode=1;ramAddress=4'b1100;dataIn=5'b11001;op=0;arg=2'b10;//Reading the register 12.
		#10;
		mode=0;ramAddress=4'b1100;dataIn=5'b11001;op=0;arg=2'b10;//Reading the register 12.
		#10;
		if(dataOut != 9'b000000110) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end
		mode=1;ramAddress=4'b1111;dataIn=5'b11111;op=1;arg=2'b01;//Rewriting the register 15.
		#10;
		mode=0;ramAddress=4'b1111;dataIn=5'b11111;op=1;arg=2'b01;//Rewriting the register 15.
		#10;
		if(dataOut != 9'b100001010) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end

		mode=1;ramAddress=4'b0011;dataIn=5'b11001;op=0;arg=2'b10;//Reading the register .
		#10;
		mode=0;ramAddress=4'b0011;dataIn=5'b11001;op=0;arg=2'b10;//Reading the register .
		#10;
		if(dataOut != 9'b000000110) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end

		mode=1;ramAddress=4'b0000;dataIn=5'b00000;op=0;arg=2'b00;//Rewriting.
		#10;
		mode=0;ramAddress=4'b0000;dataIn=5'b00000;op=0;arg=2'b00;//Rewriting.
		#10;
		if(dataOut != 9'b000000011) $display("Error occured");
		else 
		begin
		$display("Successful");
		result = result+1;
		end

		
		$display("Success Rate: %d/20",result);
		$display("Testbench was finished");	
		$finish;
	end
	
	
endmodule
