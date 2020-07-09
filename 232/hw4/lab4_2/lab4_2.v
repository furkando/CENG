`timescale 1ns / 1ps

module lab4_2(
	input[3:0] userID, 
	input CLK, 
	input team,
	input [1:0] mode,
	output reg  [7:0] numPlyLP,
	output reg  [7:0] numPlyCF,
	output reg  teamWng,
	output reg capWngLP,
	output reg capWngCF,
	output reg findRes,
	output reg [3:0] listOut,
	output reg  listMode
    );
//Write your code below
reg [3:0] LP [0:4];
reg [3:0] CF [0:4];
integer i =0;
integer flag = 0;
integer flag2 = 0;
integer flag3 = 0;
integer lepe = 0;
integer cefe = 0;
initial begin
  for (i = 0 ; i<5 ;i=i+1 ) 
  begin
	LP[i] = 4'b0000;
	CF[i] = 4'b0000;
  end
	numPlyLP = 8'b00000000;
	numPlyCF = 8'b00000000;
	teamWng = 0;
	capWngCF = 1;
	capWngLP = 1;
	findRes = 0;
	listMode = 0;
end

always @(posedge CLK) 
begin 
flag = 0;
flag2 = 0;
flag3 = 0;
	if (mode[0] == 0 && mode[1]==0) 
	begin
	listMode = 0;
		if (userID[3]==0) 
		begin
			for (i = 4 ;i>=0 ; i=i-1 ) 
			begin
				if (LP[i]==userID) 
				begin
				LP[i]=4'b0000;
				numPlyLP = numPlyLP - 8'b00000001;
					if (numPlyLP == 8'b00000000) 
					begin
						capWngLP = 1;
					end
					else
					begin
					  capWngLP = 0;
					end
				end
			end
		end 
		else 
		begin
			for (i = 4 ;i>=0 ; i=i-1 ) 
			begin
				if (CF[i]==userID) 
				begin
				CF[i]=4'b0000;
				numPlyCF = numPlyCF - 8'b00000001;
					if (numPlyCF == 8'b00000000 ) 
					begin
						capWngCF = 1;
					end
					else
					begin
					  capWngCF = 0;
					end
				end
			end
		end
	end

	if (mode[0] == 1 && mode[1] == 0) 
	begin
	listMode = 0;
		if (userID[3] == team) 
		begin
			teamWng = 0;
			if (team==0) 
			begin
				for (i = 4 ; i>=0 ;i=i-1 ) 
				begin
					if (LP[i]==userID) 
					begin
					flag = 1;
					end
				end
				if (flag == 0) 
				begin
					if(numPlyLP < 8'b00000101) 
					begin
						for (i = 4 ; i>=0 ; i = i-1 ) 
						begin
							if (LP[i]==4'b0000 && flag2==0) 
							begin
							LP[i] = userID;
							numPlyLP = numPlyLP + 8'b00000001 ;
							flag2=1;
							capWngLP = 0;
								if (numPlyLP == 8'b00000101 ) 
								begin
									capWngLP = 1;
								end
							end
						end
					end
					else 
					begin
					end
				end
			end 
			
			else 
			begin
				for (i = 4 ; i>=0 ;i=i-1 ) 
				begin
					if (CF[i]==userID) 
					begin
					flag = 1;
					end
				end
				if (flag == 0) 
				begin
					if(numPlyCF < 8'b00000101) 
					begin
						for (i = 4 ; i>=0 ; i = i-1 ) 
						begin
							if (CF[i]==4'b0000 && flag2==0) 
							begin
							CF[i] = userID;
							numPlyCF = numPlyCF + 8'b00000001 ;
							flag2=1;
							capWngCF = 0;
								if (numPlyCF == 8'b00000101 ) begin
									capWngCF = 1;
								end
							end
						end
					end
					else 
					begin
					end
				end	  
			end
		end 
		else 
		begin
			teamWng = 1 ;
		end
	end

	if (mode[0] == 0 && mode[1] == 1) 
	begin
	listMode = 0;
		if (team == 0) 
		begin
			for (i=4 ; i>=0;i = i-1 ) 
			begin
				if (LP[i] == userID) 
				begin
					findRes = 1 ;
					flag3 = 1;
				end
			end
			if (flag3 == 0) 
			begin
			findRes = 0;
			end
		end 
		else 
		begin
			for (i=4 ; i>=0;i = i-1 ) 
			begin
				if (CF[i] == userID) 
				begin
					findRes = 1 ;
					flag3 = 1;
				end
			end
			if (flag3 == 0) 
			begin
			findRes = 0;
			end	
		end
	end

	if (mode[0] == 1 && mode[1] == 1) 
	begin
	listMode = 1;
		if (team == 0) 
		begin
			listOut <= LP[4-lepe];
			lepe = lepe + 1;
			if (lepe == 5)
			begin
				lepe = 0;
			end
		end 
		else 
		begin
			listOut <= CF[4-cefe];
			cefe = cefe + 1;
			if (cefe == 5)
			begin
				cefe = 0;
			end
		end
	end
end
endmodule
