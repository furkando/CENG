`timescale 1ns / 1ps

module testbench_l4(
    );
	reg[3:0] userID;
	reg team;
	reg [1:0] mode;
	reg CLK;
	wire  [7:0] numPlyLP;
	wire  [7:0] numPlyCF;
	wire  teamWng;
	wire capWngLP;
	wire capWngCF;
	wire findRes;
	wire [3:0] listOut;
	wire  listMode;
	integer result = 0;

    lab4_2 uut(
	.userID(userID),
	.CLK(CLK),
	.team(team),
	.mode(mode),
	.numPlyLP(numPlyLP),
	.numPlyCF(numPlyCF),
	.teamWng(teamWng),
	.capWngLP(capWngLP),
	.capWngCF(capWngCF),
	.findRes(findRes),
	.listOut(listOut),
	.listMode(listMode)
);

	initial CLK = 1;
		always #5 CLK = ~CLK;

	initial begin
		$monitor("Time=%t | userID=%b team=%b mode=%b | numPlyLP=%d numPlyCF=%d teamWng=%b capWngLP=%b,capWngCF=%b,findRes=%b,listOut=%d, listMode=%b",
					$time, userID, team, mode, numPlyLP, numPlyCF, teamWng, capWngLP, capWngCF,findRes,listOut,listMode);
		userID = 4'b0000;
		team = 0;
		mode = 01;
		#5;



//--------------------------------------------------------------------------------------------//

		//0001 ID ile Takım LP(0) ye login olmak istiyor.
		//memoryt0 = {0000,0000,0000,0000,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

		userID = 4'b0001;
		team = 0;
		mode = 01;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0) begin

            $display("Test 1: Successful");
			result = result + 1;
			end
			else $display("error_1 ");
		//memoryt0 = {0001,0000,0000,0000,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

//--------------------------------------------------------------------------------------------//


		//1011 ID ile Takım LP(0) ye login olmak istiyor.
		//memoryt0 = {0001,0000,0000,0000,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

		userID = 4'b1011;
		team = 0;
		mode = 01;
		#10;
		if ( teamWng == 1'b1 && capWngLP == 1'b0 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0) begin
			$display("Test 2: Successful");
			result = result + 1;
			end
			else $display("error_2");
		//memoryt0 = {0001,0000,0000,0000,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

//--------------------------------------------------------------------------------------------//

		//0001 ID ile Takım LP(0) ye login olmak istiyor.
		//memoryt0 = {0001,0000,0000,0000,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}


		userID = 4'b0001;
		team = 0;
		mode = 01;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0)begin
			 $display("Test 3: Successful");
			result = result + 1;
			end
			else $display("error_3");

		//memoryt0 = {0001,0000,0000,0000,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

//--------------------------------------------------------------------------------------------//

		//0011 ID ile Takım LP(0) ye login olmak istiyor.
		//memoryt0 = {0001,0000,0000,0000,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

		userID = 4'b0011;
		team = 0;
		mode = 01;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0)begin
			 $display("Test 4: Successful");
			result = result + 1;
			end
			else $display("error_4");

		//memoryt0 = {0001,0011,0000,0000,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

//--------------------------------------------------------------------------------------------//

		//0111 ID ile Takım LP(0) ye login olmak istiyor.
		//memoryt0 = {0001,0011,0000,0000,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

		userID = 4'b0111;
		team = 0;
		mode = 01;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0)begin
			 $display("Test 5: Successful");
			result = result + 1;
			end
			else $display("error_5");

		//memoryt0 = {0001,0011,0111,0000,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

//--------------------------------------------------------------------------------------------//

		//0101 ID ile Takım LP(0) ye login olmak istiyor.
		//memoryt0 = {0001,0011,0111,0000,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

		userID = 4'b0101;
		team = 0;
		mode = 01;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0) begin
			$display("Test 6: Successful");
			result = result + 1;
			end
			else $display("error_6");

		//memoryt0 = {0001,0011,0111,0101,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

//--------------------------------------------------------------------------------------------//

		//0100 ID ile Takım LP(0) ye login olmak istiyor.
		//memoryt0 = {0001,0011,0111,0101,0000}
		//memoryt1 = {0000,0000,0000,0000,0000}

		userID = 4'b0100;
		team = 0;
		mode = 01;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b1 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0)begin
			 $display("Test 7: Successful");
			result = result + 1;
			end
			else $display("error_7");

		//memoryt0 = {0001,0011,0111,0101,0100}
		//memoryt1 = {0000,0000,0000,0000,0000}

//--------------------------------------------------------------------------------------------//

		//0110 ID ile Takım LP(0) ye login olmak istiyor.
		//memoryt0 = {0001,0011,0111,0101,0100}
		//memoryt1 = {0000,0000,0000,0000,0000}

		userID = 4'b0110;
		team = 0;
		mode = 01;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b1 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0) begin
				$display("Test 8: Successful");
				result = result + 1;
				end
			else $display("error_8");

		//memoryt0 = {0001,0011,0111,0101,0100}
		//memoryt1 = {0000,0000,0000,0000,0000}

//--------------------------------------------------------------------------------------------//



//--------------------------------------------------------------------------------------------//

		//0111 findRes
		//memoryt0 = {0001,0011,0111,0101,0100}
		//memoryt1 = {0000,0000,0000,0000,0000}
		userID = 4'b0111;
		team = 0;
		mode = 10;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b1 && capWngCF == 1'b1 && findRes == 1'b1 && listMode == 1'b0) begin
			$display("Test 9: Successful");
			result = result + 1;
			end
			else $display("error_9");
//--------------------------------------------------------------------------------------------//

		//0110 findRes
		//memoryt0 = {0001,0011,0111,0101,0100}
		//memoryt1 = {0000,0000,0000,0000,0000}
		userID = 4'b0110;
		team = 0;
		mode = 2'b10;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b1 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0) begin
			$display("Test 10: Successful");
			result = result + 1;
			end
			else $display("error_10 findRes: %d",findRes);

//--------------------------------------------------------------------------------------------//

		//List team 0
		//memoryt0 = {0001,0011,0111,0101,0100}
		//memoryt1 = {0000,0000,0000,0000,0000}
		userID = 4'b0111;
		team = 0;
		mode = 11;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b1 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b1)begin
			 $display("Test 11: Successful");
			result = result + 1;
			 if(listOut==4'b0001) $display("DATA0: %b",listOut);
			else $display("ERROR data0");
			#10;
			 if(listOut==4'b0011)$display("DATA1: %b",listOut);
			else $display("ERROR data1");
			#10;
			 if(listOut==4'b0111)$display("DATA2: %b",listOut);
			else $display("ERROR data2");
			#10;
			 if(listOut==4'b0101)$display("DATA3: %b",listOut);
			else $display("ERROR data3");
			#10;
			 if(listOut==4'b0100)$display("DATA4: %b",listOut);
			else $display("ERROR data4");
			#10;
			end
			else $display("error_11");

//--------------------------------------------------------------------------------------------//

        //1000 ID ile Takım LP(0) ye login olmak istiyor.
        //memoryt0 = {0001,0011,0111,0101,0100}
        //memoryt1 = {0000,0000,0000,0000,0000}

    	userID = 4'b1000;
    	team = 0;
    	mode = 01;
    	#10;
    	if ( teamWng == 1'b1 && capWngLP == 1'b1 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0)begin
    		$display("Test 12: Successful");
    		result = result + 1;
    		end
    		else $display("error_12 hata; %d %d %d %d",capWngLP,capWngCF,findRes,listMode);

        //memoryt0 = {0001,0011,0111,0101,0100}
        //memoryt1 = {0000,0000,0000,0000,0000}

//--------------------------------------------------------------------------------------------//

        //1000 ID ile Takım CF(1) ye login olmak istiyor.
        //memoryt0 = {0001,0011,0111,0101,0100}
        //memoryt1 = {0000,0000,0000,0000,0000}

    	userID = 4'b1000;
    	team = 1;
    	mode = 01;
    	#10;
    	if ( teamWng == 1'b0 && capWngLP == 1'b1 && capWngCF == 1'b0 && findRes == 1'b0 && listMode == 1'b0)begin
    		$display("Test 13: Successful");
    		result = result + 1;
    		end
    		else $display("error_13");

        //memoryt0 = {0001,0011,0111,0101,0100}
        //memoryt1 = {1000,0000,0000,0000,0000}

//--------------------------------------------------------------------------------------------//


        //1100 ID ile Takım CF(1) ye login olmak istiyor.
        //memoryt0 = {0001,0011,0111,0101,0100}
        //memoryt1 = {1000,1100,0000,0000,0000}

    	userID = 4'b1100;
    	team = 1;
    	mode = 01;
    	#10;
    	if ( teamWng == 1'b0 && capWngLP == 1'b1 && capWngCF == 1'b0 && findRes == 1'b0 && listMode == 1'b0)begin
    		$display("Test 14: Successful");
    		result = result + 1;
    		end
    		else $display("error_14");

        //memoryt0 = {0001,0011,0111,0101,0100}
        //memoryt1 = {1000,1100,0000,0000,0000}

//--------------------------------------------------------------------------------------------//

        //0011 ID ile Takım LP(0) dan logout olmak istiyor.
        //memoryt0 = {0001,0011,0111,0101,0100}
        //memoryt1 = {1000,1100,0000,0000,0000}

    	userID = 4'b0011;
    	team = 0;
    	mode = 00;
    	#10;
    	if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b0 && findRes == 1'b0 && listMode == 1'b0)begin
    		$display("Test 15: Successful");
    		result = result + 1;
    		end
    		else $display("error_15");

        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,0000,0000,0000}

//--------------------------------------------------------------------------------------------//


        //1110 ID ile Takım LP(0) e login olmak istiyor.
        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,0000,0000,0000}

    	userID = 4'b1110;
    	team = 0;
    	mode = 01;
    	#10;
    	if ( teamWng == 1'b1 && capWngLP == 1'b0 && capWngCF == 1'b0 && findRes == 1'b0 && listMode == 1'b0)begin
    		$display("Test 16: Successful");
    		result = result + 1;
    		end
    		else $display("error_16");

        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,0000,0000,0000}

//--------------------------------------------------------------------------------------------//

        //1110 ID ile Takım CF(1) e login olmak istiyor.
        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,0000,0000,0000}

        userID = 4'b1110;
        team = 1;
        mode = 01;
        #10;
        if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b0 && findRes == 1'b0 && listMode == 1'b0)begin
            $display("Test 17: Successful");
            result = result + 1;
            end
            else $display("error_17");

        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,1110,0000,0000}

//--------------------------------------------------------------------------------------------//

        //0011 findRes
        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,1110,0000,0000}
        userID = 4'b0011;
        team = 0;
        mode = 2'b10;
        #10;
        if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b0 && findRes == 1'b0 && listMode == 1'b0) begin
            $display("Test 18: Successful");
            result = result + 1;
            end
            else $display("error_18");

//--------------------------------------------------------------------------------------------//

        //1110 findRes
        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,1110,0000,0000}
        userID = 4'b1110;
        team = 0;
        mode = 2'b10;
        #10;
        if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b0 && findRes == 1'b0 && listMode == 1'b0) begin
            $display("Test 19: Successful");
            result = result + 1;
            end
            else $display("error_19");

//--------------------------------------------------------------------------------------------//

        //1111 ID ile Takım CF(1) e login olmak istiyor.
        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,1110,0000,0000}

        userID = 4'b1111;
        team = 1;
        mode = 01;
        #10;
        if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b0 && findRes == 1'b0 && listMode == 1'b0)begin
            $display("Test 20: Successful");
            result = result + 1;
            end
            else $display("error_20");

        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,1110,1111,0000}

//--------------------------------------------------------------------------------------------//


        //1001 ID ile Takım LP(0) e login olmak istiyor.
        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,1110,1111,0000}

        userID = 4'b1001;
        team = 0;
        mode = 01;
        #10;
        if ( teamWng == 1'b1 && capWngLP == 1'b0 && capWngCF == 1'b0 && findRes == 1'b0 && listMode == 1'b0)begin
            $display("Test 21: Successful");
            result = result + 1;
            end
            else $display("error_21");

        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,1110,1111,0000}

//--------------------------------------------------------------------------------------------//


        //1001 ID ile Takım CF(1) e login olmak istiyor.
        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,1110,1111,0000}

        userID = 4'b1001;
        team = 1;
        mode = 01;
        #10;
        if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0)begin
            $display("Test 22: Successful");
            result = result + 1;
            end
            else $display("error_22");

        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,1110,1111,1001}

//--------------------------------------------------------------------------------------------//

		//List team 0
        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,1110,1111,1001}
		userID = 4'b0111;
		team = 0;
		mode = 11;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b1)begin
			 $display("Test 23: Successful");
			result = result + 1;
			 if(listOut==4'b0001) $display("DATA0: %b",listOut);
			else $display("ERROR data0");
			#10;
			 if(listOut==4'b0000)$display("DATA1: %b",listOut);
			else $display("ERROR data1");
			#10;
			 if(listOut==4'b0111)$display("DATA2: %b",listOut);
			else $display("ERROR data2");
			#10;
			 if(listOut==4'b0101)$display("DATA3: %b",listOut);
			else $display("ERROR data3");
			#10;
			 if(listOut==4'b0100)$display("DATA4: %b",listOut);
			else $display("ERROR data4");
			#10;
			end
			else $display("error_23");

//--------------------------------------------------------------------------------------------//

		//List team 1
        //memoryt0 = {0001,0000,0111,0101,0100}
        //memoryt1 = {1000,1100,1110,1111,1001}
		userID = 4'b0111;
		team = 1;
		mode = 11;
		#10;
		if ( teamWng == 1'b0 && capWngLP == 1'b0 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b1)begin
			 $display("Test 24: Successful");
			result = result + 1;
			 if(listOut==4'b1000) $display("DATA0: %b",listOut);
			else $display("ERROR data0");
			#10;
			 if(listOut==4'b1100)$display("DATA1: %b",listOut);
			else $display("ERROR data1");
			#10;
			 if(listOut==4'b1110)$display("DATA2: %b",listOut);
			else $display("ERROR data2");
			#10;
			 if(listOut==4'b1111)$display("DATA3: %b",listOut);
			else $display("ERROR data3");
			#10;
			 if(listOut==4'b1001)$display("DATA4: %b",listOut);
			else $display("ERROR data4");
			#10;
			end
			else $display("error_24");

//--------------------------------------------------------------------------------------------//


            //0110 ID ile Takım LP(0) e login olmak istiyor.
            //memoryt0 = {0001,0000,0111,0101,0100}
            //memoryt1 = {1000,1100,1110,0000,0000}

            userID = 4'b0110;
            team = 0;
            mode = 01;
            #10;
            if ( teamWng == 1'b0 && capWngLP == 1'b1 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0)begin
                $display("Test 25: Successful");
                result = result + 1;
                end
                else $display("error_25");

            //memoryt0 = {0001,0110,0111,0101,0100}
            //memoryt1 = {1000,1100,1110,1111,1001}

//--------------------------------------------------------------------------------------------//



            //00110 ID ile Takım LP(0) e login olmak istiyor.
            //memoryt0 = {0001,0110,0111,0101,0100}
            //memoryt1 = {1000,1100,1110,1111,1001}

            userID = 4'b0110;
            team = 0;
            mode = 01;
            #10;
            if ( teamWng == 1'b0 && capWngLP == 1'b1 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0)begin
                $display("Test 26: Successful");
                result = result + 1;
                end
                else $display("error_26");

            //memoryt0 = {0001,0110,0111,0101,0100}
            //memoryt1 = {1000,1100,1110,1111,1001}

//--------------------------------------------------------------------------------------------//




            //1011 ID ile Takım LP(0) e login olmak istiyor.
            //memoryt0 = {0001,0110,0111,0101,0100}
            //memoryt1 = {1000,1100,1110,1111,1001}

            userID = 4'b1011;
            team = 0;
            mode = 01;
            #10;
            if ( teamWng == 1'b1 && capWngLP == 1'b1 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0)begin
                $display("Test 27: Successful");
                result = result + 1;
                end
                else $display("error_27");

            //memoryt0 = {0001,0110,0111,0101,0100}
            //memoryt1 = {1000,1100,1110,1111,1001}

//--------------------------------------------------------------------------------------------//


            //1011 ID ile Takım CF(1) e login olmak istiyor.
            //memoryt0 = {0001,0110,0111,0101,0100}
            //memoryt1 = {1000,1100,1110,1111,1001}

            userID = 4'b1011;
            team = 1;
            mode = 01;
            #10;
            if ( teamWng == 1'b0 && capWngLP == 1'b1 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0)begin
                $display("Test 28: Successful");
                result = result + 1;
                end
                else $display("error_28");

            //memoryt0 = {0001,0110,0111,0101,0100}
            //memoryt1 = {1000,1100,1110,1111,1001}

//--------------------------------------------------------------------------------------------//


            //0011 ID ile Takım CF(1) e login olmak istiyor.
            //memoryt0 = {0001,0110,0111,0101,0100}
            //memoryt1 = {1000,1100,1110,1111,1001}

            userID = 4'b0011;
            team = 1;
            mode = 01;
            #10;
            if ( teamWng == 1'b1 && capWngLP == 1'b1 && capWngCF == 1'b1 && findRes == 1'b0 && listMode == 1'b0)begin
                $display("Test 29: Successful");
                result = result + 1;
                end
                else $display("error_29");

            //memoryt0 = {0001,0110,0111,0101,0100}
            //memoryt1 = {1000,1100,1110,1111,1001}

//--------------------------------------------------------------------------------------------//


		if(result == 29) $display("ACCOMPLISHED 29/29");
		else $display("Result != 29");
		$display("Testbench was finished!");
		$finish;
	end


endmodule
