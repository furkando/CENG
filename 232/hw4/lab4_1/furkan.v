`timescale 1ns / 1ps
module FUNCROM (input [3:0] romAddress, output reg[4:0] romData);

integer i,a,b,c,d,e;

always@*
begin
for (i = 4 ; i>0 ; i=i-1 ) begin
	  romData[i] = romAddress[i-1];
	end
	a = romAddress[1]*romAddress[0]*(~romAddress[2])*(~romAddress[3]) ;
	b = romAddress[2]*romAddress[0]*(~romAddress[1])*(~romAddress[3]) ;
	c = romAddress[2]*romAddress[1]*(~romAddress[0])*(~romAddress[3]) ;
	d = romAddress[3]*(~romAddress[0])*(~romAddress[1]) ;
	e = romAddress[3]*romAddress[0]*romAddress[1] ;
	romData[0] = a+b+c+d+e ;
	if(romAddress == 4'b0100 || romAddress == 4'b1110 )begin
	  romData[1] = 1;
	end 
end

endmodule
																						
module FUNCRAM (input mode,input [3:0] ramAddress, input [4:0] dataIn,input op, input [1:0] arg,  input CLK, output reg [8:0] dataOut);
reg [8:0] ram [0:15];
integer i,dec,result=0,kat,last;
reg [0:8] res ;
initial begin
  for (i =0 ;i<16 ;i=i+1 ) begin
	ram[i] = 9'b000000000 ;
  end
end




always @(ram[0],ram[1],ram[2],ram[3],ram[4],ram[5],ram[6],ram[7],ram[8],ram[9],ram[10],ram[11],ram[12],ram[13],ram[14],ram[15]) begin
  if(mode == 0 )begin
	dec = (8*ramAddress[3]) + (4*ramAddress[2]) + (2*ramAddress[1]) + (ramAddress[0]) ;
	dataOut = ram[dec];
  end
end

always @* 
begin 

	if(arg == 2'b00)begin
	  kat = 2;
	end
	if(arg == 2'b01)begin
	  kat = 1;	  
	end
	if(arg == 2'b10)begin
	  kat = -1;	  
	end
	if(arg == 2'b11)begin
	  kat = -2;	  
	end

	dec = 8*ramAddress[3] + 4*ramAddress[2] + 2*ramAddress[1] + ramAddress[0] ;

	if(op == 0)
	
	begin
		result = 0;

		if (dataIn[0] == 0) 
		begin
		  result = result +1 ;
		end 
		
		else 
		begin
		  result = result -1 ;
		end
		
		if (dataIn[1] == 0) 
		begin
		  result = result +(kat ) ;
		end 
		
		else 
		begin
		  result = result -(kat ) ;
		end
		
		if (dataIn[2] == 0) 
		begin
		  result = result +(kat * kat) ;
		end 
		
		else 
		begin
		  result = result -(kat * kat) ;
		end
		
		if (dataIn[3] == 0) 
		begin
		  result = result +(kat * kat * kat) ;
		end 
		
		else 
		begin
		  result = result -(kat * kat * kat) ;
		end
		
		if (dataIn[4] == 0) 
		begin
		  result = result +(kat * kat * kat * kat) ;
		end 
		
		else 
		begin
		  result = result -(kat * kat * kat * kat) ;
		end
		
		
		
	  result = result%7;
	  
	  if (result<0) 
	  begin
		result =result + 7;
	  end
	end
	

	else 
	begin
	result = 0;

		if (dataIn[1]==0) 
		begin
			result =result + 1  ;
		end 
		
		else 
		begin
			result =result - 1  ;
		end
		
		if (dataIn[2]==0) 
		begin
			result =result + 2 * (kat ) ;
		end 
		
		else 
		begin
			result =result - 2 * (kat ) ;
		end
		
		if (dataIn[3]==0) 
		begin
			result =result + 3 * (kat * kat) ;
		end 
		
		else 
		begin
			result =result - 3 * (kat * kat) ;
		end
		
		if (dataIn[4]==0) 
		begin
			result =result + 4 * (kat * kat * kat) ;
		end 
		
		else 
		begin
			result =result - 4 * (kat * kat * kat) ;
		end
	  
	end

	
		res[0] = 0;		
		res[1] = 0;
		res[2] = 0;
		res[3] = 0;
		res[4] = 0;
		res[5] = 0;
		res[6] = 0;
		res[7] = 0;
		res[8] = 0;

	

	if (result<0) 
	begin
		res[0] = 1;
		result = -result;
	end
	last = 8;

	  res[8] = result%2;
	  result = result/2;
	  res[7] = result%2;
	  result = result/2;
	  res[6] = result%2;
	  result = result/2;
	  res[5] = result%2;
	  result = result/2;
	  res[4] = result%2;
	  result = result/2;
	  res[3] = result%2;
	  result = result/2;
	  res[2] = result%2;
	  result = result/2;
	  res[1] = result%2;
	end


always@(posedge CLK)begin
  if (mode == 1) begin
			ram[dec] = res;
  end
end


endmodule


module FUNCMEMORY(input mode, input [6:0] memInput, input CLK, output wire [8:0] result);
	/*Don't edit this module*/
	wire [4:0]  romData;

	FUNCROM RO(memInput[6:3], romData);
	FUNCRAM RA(mode, memInput[6:3], romData, memInput[2],memInput[1:0], CLK, result);

endmodule 
