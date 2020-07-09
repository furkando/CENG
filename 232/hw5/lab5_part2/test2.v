`timescale 1ns / 1ps

module testbench_l5(
    );
	reg[3:0] request;
	reg [1:0] mode;
	reg CLK;
	wire  [3:0] currentFloor1;
	wire  [3:0] currentFloor2;
	wire  doorOpen1;
	wire doorOpen2;
	wire listBusy;
	wire state;
	wire [3:0] listingLeds;
	integer result = 0;
	 
    Elevator uut(
	.request(request),
	.CLK(CLK),
	.currentFloor1(currentFloor1),
	.mode(mode),
	.currentFloor2(currentFloor2),
	.doorOpen1(doorOpen1),
	.doorOpen2(doorOpen2),
	.listBusy(listBusy),
	.state(state),
	.listingLeds(listingLeds)
);

	initial CLK = 1;
		always #5 CLK = ~CLK;
	
	initial begin
		$monitor("Time=%t | mode=%b request=%b | currentFloor1=%d currentFloor2=%d doorOpen1=%b doorOpen2=%b,listingLeds=%b,listBusy=%b,state=%b", 
					$time, mode,request,currentFloor1,currentFloor2, doorOpen1,doorOpen2,listingLeds,listBusy,state);

	request = 4'b0000;
	mode = 00;
	#5;
		

//--------------------------------------------------------------------------------------------//
		mode = 00;
		request = 4'b0001;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 1: Successful"); 
			result = result + 1;
			end	
			else $display("error_1 ");


//--------------------------------------------------------------------------------------------//

		mode = 00;
		request = 4'b0010;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 2: Successful"); 
			result = result + 1;
			end	
			else $display("error_2 ");


//--------------------------------------------------------------------------------------------//

	mode = 00;
		request = 4'b0011;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 3: Successful"); 
			result = result + 1;
			end	
			else $display("error_3");


//--------------------------------------------------------------------------------------------//

		mode = 00;
		request = 4'b0100;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 4: Successful"); 
			result = result + 1;
			end	
			else $display("error_4");


//--------------------------------------------------------------------------------------------//	

	mode = 00;
		request = 4'b0101;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 5: Successful"); 
			result = result + 1;
			end	
			else $display("error_5");


//--------------------------------------------------------------------------------------------//

		mode = 00;
		request = 4'b0110;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 6: Successful"); 
			result = result + 1;
			end	
			else $display("error_6");


//--------------------------------------------------------------------------------------------//

	mode = 00;
		request = 4'b0111;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 7: Successful"); 
			result = result + 1;
			end	
			else $display("error_7");


//--------------------------------------------------------------------------------------------//

		mode = 00;
		request = 4'b1000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 8: Successful"); 
			result = result + 1;
			end	
			else $display("error_8");


//--------------------------------------------------------------------------------------------//

	mode = 00;
		request = 4'b1001;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 9: Successful"); 
			result = result + 1;
			end	
			else $display("error_9");


//--------------------------------------------------------------------------------------------//

	mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b1 && listingLeds == 4'b0001 && state==0) begin
			$display("Test 10: Successful"); 
			result = result + 1;
			end	
			else $display("error_10");


//--------------------------------------------------------------------------------------------//

mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b1 && listingLeds == 4'b0010 && state==0) begin
			$display("Test 11: Successful"); 
			result = result + 1;
			end	
			else $display("error_11");


//--------------------------------------------------------------------------------------------//

mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b1 && listingLeds == 4'b0011 && state==0) begin
			$display("Test 12: Successful"); 
			result = result + 1;
			end	
			else $display("error_12");


//--------------------------------------------------------------------------------------------//

mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b1 && listingLeds == 4'b0100 && state==0) begin
			$display("Test 13: Successful"); 
			result = result + 1;
			end	
			else $display("error_13");


//--------------------------------------------------------------------------------------------//

mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b1 && listingLeds == 4'b0101 && state==0) begin
			$display("Test 14: Successful"); 
			result = result + 1;
			end	
			else $display("error_14");


//--------------------------------------------------------------------------------------------//

mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b1 && listingLeds == 4'b0110 && state==0) begin
			$display("Test 15: Successful"); 
			result = result + 1;
			end	
			else $display("error_15");


//--------------------------------------------------------------------------------------------//

mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b1 && listingLeds == 4'b0111 && state==0) begin
			$display("Test 16: Successful"); 
			result = result + 1;
			end	
			else $display("error_16");


//--------------------------------------------------------------------------------------------//

mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b1 && listingLeds == 4'b1000 && state==0) begin
			$display("Test 17: Successful"); 
			result = result + 1;
			end	
			else $display("error_17");


//--------------------------------------------------------------------------------------------//

mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 18: Successful"); 
			result = result + 1;
			end	
			else $display("error_18");


//--------------------------------------------------------------------------------------------//

mode = 10;
		request = 4'b0001;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 19: Successful"); 
			result = result + 1;
			end	
			else $display("error_19");


//--------------------------------------------------------------------------------------------//

mode = 10;
		request = 4'b0010;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 20: Successful"); 
			result = result + 1;
			end	
			else $display("error_20");


//--------------------------------------------------------------------------------------------//

mode = 10;
		request = 4'b0011;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 21: Successful"); 
			result = result + 1;
			end	
			else $display("error_21");


//--------------------------------------------------------------------------------------------//

mode = 10;
		request = 4'b0101;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 22: Successful"); 
			result = result + 1;
			end	
			else $display("error_22");


//--------------------------------------------------------------------------------------------//

mode = 10;
		request = 4'b0110;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 23: Successful"); 
			result = result + 1;
			end	
			else $display("error_23");


//--------------------------------------------------------------------------------------------//

mode = 10;
		request = 4'b0111;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 24: Successful"); 
			result = result + 1;
			end	
			else $display("error_24");


//--------------------------------------------------------------------------------------------//
mode = 10;
		request = 4'b1000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 25: Successful"); 
			result = result + 1;
			end	
			else $display("error_25");


//--------------------------------------------------------------------------------------------//

mode = 00;
		request = 4'b1111;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 26: Successful"); 
			result = result + 1;
			end	
			else $display("error_26");


//--------------------------------------------------------------------------------------------//

mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b1 && listingLeds == 4'b0100 && state==0) begin
			$display("Test 27: Successful"); 
			result = result + 1;
			end	
			else $display("error_27");


//--------------------------------------------------------------------------------------------//

mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b1 && listingLeds == 4'b1111 && state==0) begin
			$display("Test 28: Successful"); 
			result = result + 1;
			end	
			else $display("error_28");


//--------------------------------------------------------------------------------------------//

mode = 01;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 29: Successful"); 
			result = result + 1;
			end	
			else $display("error_29");


//--------------------------------------------------------------------------------------------//
		mode = 10;
		request = 4'b1111;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 30: Successful"); 
			result = result + 1;
			end	
			else $display("error_30");


//--------------------------------------------------------------------------------------------//
mode = 10;
		request = 4'b0100;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 31: Successful"); 
			result = result + 1;
			end	
			else $display("error_31");


//--------------------------------------------------------------------------------------------//

mode = 00;
		request = 4'b1011;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 32: Successful"); 
			result = result + 1;
			end	
			else $display("error_32");


//--------------------------------------------------------------------------------------------//

mode = 00;
		request = 4'b0011;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 33: Successful"); 
			result = result + 1;
			end	
			else $display("error_33");


//--------------------------------------------------------------------------------------------//
	mode = 00;
		request = 4'b0101;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 34: Successful"); 
			result = result + 1;
			end	
			else $display("error_34");


//--------------------------------------------------------------------------------------------//
mode = 00;
		request = 4'b0010;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0) begin
			$display("Test 35: Successful"); 
			result = result + 1;
			end	
			else $display("error_35");


//--------------------------------------------------------------------------------------------//
mode = 11;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==1 && doorOpen2==0 && currentFloor2 == 4'b0010 &&  doorOpen1==0 && currentFloor1==4'b0010) begin
			$display("Test 36: Successful"); 
			result = result + 1;
			end	
			else $display("error_36");


//--------------------------------------------------------------------------------------------//
mode = 11;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==1 && doorOpen2==1 && currentFloor2 == 4'b0011 &&  doorOpen1==0 && currentFloor1==4'b0011) begin
			$display("Test 37: Successful"); 
			result = result + 1;
			end	
			else $display("error_37");


//--------------------------------------------------------------------------------------------//
mode = 11;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==1 && doorOpen2==0 && currentFloor2 == 4'b0100 &&  doorOpen1==0 && currentFloor1==4'b0100) begin
			$display("Test 38: Successful"); 
			result = result + 1;
			end	
			else $display("error_38");


//--------------------------------------------------------------------------------------------//
mode = 11;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==1 && doorOpen2==1 && currentFloor2 == 4'b0101 &&  doorOpen1==0 && currentFloor1==4'b0101) begin
			$display("Test 39: Successful"); 
			result = result + 1;
			end	
			else $display("error_39");


//--------------------------------------------------------------------------------------------//
mode = 11;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==1 && doorOpen2==0 && currentFloor2 == 4'b0100 &&  doorOpen1==0 && currentFloor1==4'b0110) begin
			$display("Test 40: Successful"); 
			result = result + 1;
			end	
			else $display("error_40");


//--------------------------------------------------------------------------------------------//
mode = 11;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==1 && doorOpen2==0 && currentFloor2 == 4'b0011 &&  doorOpen1==0 && currentFloor1==4'b0111) begin
			$display("Test 41: Successful"); 
			result = result + 1;
			end	
			else $display("error_41");


//--------------------------------------------------------------------------------------------//
mode = 11;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==1 && doorOpen2==1 && currentFloor2 == 4'b0010 &&  doorOpen1==0 && currentFloor1==4'b1000) begin
			$display("Test 42: Successful"); 
			result = result + 1;
			end	
			else $display("error_42");


//--------------------------------------------------------------------------------------------//
mode = 11;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==1 && doorOpen2==1 && currentFloor2 == 4'b0010 &&  doorOpen1==0 && currentFloor1==4'b1001) begin
			$display("Test 43: Successful"); 
			result = result + 1;
			end	
			else $display("error_43");


//--------------------------------------------------------------------------------------------//
mode = 11;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==1 && doorOpen2==1 && currentFloor2 == 4'b0010 &&  doorOpen1==0 && currentFloor1==4'b1010) begin
			$display("Test 44: Successful"); 
			result = result + 1;
			end	
			else $display("error_44");


//--------------------------------------------------------------------------------------------//
mode = 11;
		request = 4'b0000;
		#10;
		if ( listBusy==1'b0 && listingLeds == 4'b0000 && state==0 && doorOpen2==1 && currentFloor2 == 4'b0010 &&  doorOpen1==1 && currentFloor1==4'b1011) begin
			$display("Test 45: Successful"); 
			result = result + 1;
			end	
			else $display("error_45");


//--------------------------------------------------------------------------------------------//

		if(result == 45) $display("ACCOMPLISHED 45/45");
		else $display("Result != 45");
		$display("Testbench was finished");	
		$finish;
	end
	

endmodule

