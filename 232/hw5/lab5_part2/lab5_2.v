`timescale 1ns / 1ps

module Elevator(input CLK,
					input [1:0] mode,
					input [3:0] request,
					output reg[3:0] currentFloor1,
					output reg[3:0] currentFloor2,
					output reg doorOpen1, 
					output reg doorOpen2, 
					output reg [3:0] listingLeds,
					output reg listBusy,
					output reg state);

//Write your code below
reg [3:0] taskPool [0:7];
reg [3:0] task1;
reg [3:0] task2;
integer i = 0;
integer j = 0;
integer flag = 0;
initial begin
	currentFloor1[3:0] = 1;
	currentFloor2[3:0] = 1;
	doorOpen1=1;
	doorOpen2=1;
	listingLeds=4'b0000;
	listBusy = 0;
	state = 0;
	for (i =0 ;i<8 ;i = i+1 ) 
	begin
		taskPool[i] = 4'b0000 ;
  	end
end
always@(posedge CLK)begin
  	if(mode == 2'b00)
	begin
		state = 0;
		flag = 0;
		listBusy = 1'b0;
		for (i =0 ;i<8 ;i = i+1 ) 
		begin
			if(taskPool[i]==request)
			begin
				flag = 1;
			end
  		end
		if(flag == 0)
		begin
			for (i =0 ;i<8 ;i = i+1 ) 
			begin
				if(taskPool[i]==4'b0000)
				begin
					taskPool[i]=request;
					i = 8;
				end
			end		
		end  
	end
	else if(mode == 2'b01)
	begin
		state = 0;
		listBusy = 1'b1;
		for (i =0 ;i<8 ;i = i+1 ) 
		begin
			if(taskPool[i] != 4'b0000 )
			begin
				if ( taskPool[i] == listingLeds) 
				begin
					if(i == 7)
					begin
						listBusy = 1'b0;
						listingLeds = 4'b0000;  
						i=8;
					end
					else
					begin
						if(taskPool[i+1] != 4'b0000)
						begin
							listingLeds = taskPool[i+1];	
							i = 8;			  					  
						end
						else
						begin
							listBusy = 1'b0;
							listingLeds = 4'b0000;
							i=8;    
						end
					end
				end
				else
				begin
					if(i==0 && listingLeds==4'b0000)
					begin
						listingLeds = taskPool[i];
						i = 8;
					end
				end
			end
			else
			begin
				listBusy = 1'b0;
				listingLeds = 4'b0000; 
				i = 8;		  
			end
  		end		
	end
	else if(mode == 2'b10)
	begin
		state = 0;
		listBusy = 1'b0;
		for (i =0 ;i<8 ;i = i+1 ) 
		begin
			if(taskPool[i]==request)
			begin
				taskPool[i] = 4'b0000;
				for ( j=i ;j<7 ;j=j+1 ) 
				begin
					taskPool[j]=taskPool[j+1];
				end
				taskPool[7] = 4'b0000;
				i=7;
			end
  		end	
	end
	else 
	begin
		listBusy = 1'b0;
		if(state == 0)
		begin
			if(taskPool[0]!=4'b0000 && taskPool[1]!=4'b0000)
			begin
				task1 = taskPool[0];
				for (i = 0 ; i<7 ;i= i+1 ) 
				begin
					taskPool[i] = taskPool[i+1];
				end
				taskPool[7]=4'b0000;
				task2 = taskPool[0];
				for (i = 0 ; i<7 ;i = i+1 ) 
				begin
					taskPool[i] = taskPool[i+1];
				end
				taskPool[7]=4'b0000;
				if(task1 > currentFloor1)
				begin
					doorOpen1 = 0;
					currentFloor1 = currentFloor1 + 4'b0001 ;  
				end
				else if (task1< currentFloor1) 
				begin
					doorOpen1 = 0;
					currentFloor1 = currentFloor1 - 4'b0001 ;  
				end
				if(task2 > currentFloor2)
				begin
					doorOpen2 = 0;
					currentFloor2 = currentFloor2 + 4'b0001 ;  
				end
				else if (task2< currentFloor2) 
				begin
					doorOpen2 = 0;
					currentFloor2 = currentFloor2 - 4'b0001 ;  
				end				
				state = 1;
			end
			else if(taskPool[0]!=4'b0000 && taskPool[1]==4'b0000)
			begin
				task1 = taskPool[0];
				for (i = 0 ; i<7 ;i = i+1 ) 
				begin
					taskPool[i] = taskPool[i+1];
				end
				taskPool[7]=4'b0000;
				if(task1 > currentFloor1)
				begin
					doorOpen1=0;
					currentFloor1 = currentFloor1 + 4'b0001 ;  
				end
				else if (task1 < currentFloor1) 
				begin
					doorOpen1=0;
					currentFloor1 = currentFloor1 - 4'b0001 ;  
				end
				state = 1;  
			end
			else
			begin
				state = 0;  
			end
		end
		else
		begin
			if(task1 != currentFloor1 && task2 != currentFloor2)
			begin
				if(task1 > currentFloor1)
				begin
					doorOpen1 = 0;
					currentFloor1 = currentFloor1 + 4'b0001 ;  
				end
				else if (task1< currentFloor1) 
				begin
					doorOpen1 = 0;
					currentFloor1 = currentFloor1 - 4'b0001 ;  
				end
				if(task2 > currentFloor2)
				begin
					doorOpen2 = 0;
					currentFloor2 = currentFloor2 + 4'b0001 ;  
				end
				else if (task2< currentFloor2) 
				begin
					doorOpen2 = 0;
					currentFloor2 = currentFloor2 - 4'b0001 ;  
				end

				if (task1 != currentFloor1 && task2 == currentFloor2) 
				begin
					doorOpen2 = 1;
					if(taskPool[0]!=4'b0000)
					begin
						task2 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;					  
					end				  
				end

				else if (task1 == currentFloor1 && task2 != currentFloor2) 
				begin
					doorOpen1 = 1;
					if(taskPool[0]!=4'b0000)
					begin
						task1 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;					  
					end					  
				end

				else if(task1 == currentFloor1 && task2 == currentFloor2)
				begin
					doorOpen1 = 1;
					doorOpen2 = 1;
					if(taskPool[0]!=4'b0000 && taskPool[1]!=4'b0000)
					begin
						task1 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;
						task2 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;		
					end
					else if(taskPool[0]!=4'b0000 && taskPool[1]==4'b0000)
					begin
						task1 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;
					end
					else
					begin
						state = 0;  
					end				  
				end	
				else
				begin
				  
				end			
			end

			else if(task1 != currentFloor1 && task2 == currentFloor2)
			begin
				doorOpen2 = 1;
				if(taskPool[0]!=4'b0000)
				begin
					task2 = taskPool[0];
					for (i = 0 ; i<7 ;i = i+1 ) 
					begin
						taskPool[i] = taskPool[i+1];
					end
					taskPool[7]=4'b0000;	
				  
				end
				
				if(task1 > currentFloor1)
				begin
					doorOpen1 = 0;
					currentFloor1 = currentFloor1 + 4'b0001 ;  
				end
				else if (task1< currentFloor1) 
				begin
					doorOpen1 = 0;
					currentFloor1 = currentFloor1 - 4'b0001 ;  
				end
				if(task2 > currentFloor2)
				begin
					doorOpen2 = 0;
					currentFloor2 = currentFloor2 + 4'b0001 ;  
				end
				else if (task2< currentFloor2) 
				begin
					doorOpen2 = 0;
					currentFloor2 = currentFloor2 - 4'b0001 ;  
				end

				if (task1 != currentFloor1 && task2 == currentFloor2) 
				begin
					doorOpen2 = 1;
					if(taskPool[0]!=4'b0000)
					begin
						task2 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;					  
					end				  
				end

				else if (task1 == currentFloor1 && task2 != currentFloor2) 
				begin
					doorOpen1 = 1;
					if(taskPool[0]!=4'b0000)
					begin
						task1 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;					  
					end					  
				end

				else if(task1 == currentFloor1 && task2 == currentFloor2)
				begin
					doorOpen1 = 1;
					doorOpen2 = 1;
					if(taskPool[0]!=4'b0000 && taskPool[1]!=4'b0000)
					begin
						task1 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;
						task2 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;		
					end
					else if(taskPool[0]!=4'b0000 && taskPool[1]==4'b0000)
					begin
						task1 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;
					end
					else
					begin
						state = 0;  
					end				  
				end	
				else
				begin
				  
				end			
			end
			else if (task1 == currentFloor1 && task2 != currentFloor2) 
			begin
				doorOpen1 = 1;
				if(taskPool[0]!=4'b0000)
				begin
					task1 = taskPool[0];
					for (i = 0 ; i<7 ;i = i+1 ) 
					begin
						taskPool[i] = taskPool[i+1];
					end
					taskPool[7]=4'b0000;					  
				end			
				if(task1 > currentFloor1)
				begin
					doorOpen1 = 0;
					currentFloor1 = currentFloor1 + 4'b0001 ;  
				end
				else if (task1< currentFloor1) 
				begin
					doorOpen1 = 0;
					currentFloor1 = currentFloor1 - 4'b0001 ;  
				end
				if(task2 > currentFloor2)
				begin
					doorOpen2 = 0;
					currentFloor2 = currentFloor2 + 4'b0001 ;  
				end
				else if (task2< currentFloor2) 
				begin
					doorOpen2 = 0;
					currentFloor2 = currentFloor2 - 4'b0001 ;  
				end

				if (task1 != currentFloor1 && task2 == currentFloor2) 
				begin
					doorOpen2 = 1;
					if(taskPool[0]!=4'b0000)
					begin
						task2 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;					  
					end				  
				end

				else if (task1 == currentFloor1 && task2 != currentFloor2) 
				begin
					doorOpen1 = 1;
					if(taskPool[0]!=4'b0000)
					begin
						task1 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;					  
					end					  
				end

				else if(task1 == currentFloor1 && task2 == currentFloor2)
				begin
					doorOpen1 = 1;
					doorOpen2 = 1;
					if(taskPool[0]!=4'b0000 && taskPool[1]!=4'b0000)
					begin
						task1 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;
						task2 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;		
					end
					else if(taskPool[0]!=4'b0000 && taskPool[1]==4'b0000)
					begin
						task1 = taskPool[0];
						for (i = 0 ; i<7 ;i = i+1 ) 
						begin
							taskPool[i] = taskPool[i+1];
						end
						taskPool[7]=4'b0000;
					end
					else
					begin
						state = 0;  
					end				  
				end	
				else
				begin
				  
				end							  
			end
			else
			begin
				if(taskPool[0]!=4'b0000 && taskPool[1]!=4'b0000)
				begin
					task1 = taskPool[0];
					for (i = 0 ; i<7 ;i = i+1 ) 
					begin
						taskPool[i] = taskPool[i+1];
					end
					taskPool[7]=4'b0000;
					task2 = taskPool[0];
					for (i = 0 ; i<7 ;i = i+1 ) 
					begin
						taskPool[i] = taskPool[i+1];
					end
					taskPool[7]=4'b0000;
					if(task1 > currentFloor1)
					begin
						doorOpen1 = 0;
						currentFloor1 = currentFloor1 + 4'b0001 ;  
					end
					else if (task1< currentFloor1) 
					begin
						doorOpen1 = 0;
						currentFloor1 = currentFloor1 - 4'b0001 ;  
					end
					if(task2 > currentFloor2)
					begin
						doorOpen2 = 0;
						currentFloor2 = currentFloor2 + 4'b0001 ;  
					end
					else if (task2< currentFloor2) 
					begin
						doorOpen2 = 0;
						currentFloor2 = currentFloor2 - 4'b0001 ;  
					end				
				end
				else if(taskPool[0]!=4'b0000 && taskPool[1]==4'b0000)
				begin
					task1 = taskPool[0];
					for (i = 0 ; i<7 ;i = i+1 ) 
					begin
						taskPool[i] = taskPool[i+1];
					end
					taskPool[7]=4'b0000;
					if(task1 > currentFloor1)
					begin
						doorOpen1=0;
						currentFloor1 = currentFloor1 + 4'b0001 ;  
					end
					else if (task1 < currentFloor1) 
					begin
						doorOpen1=0;
						currentFloor1 = currentFloor1 - 4'b0001 ;  
					end
				end
				else
				begin
					state = 0;  
				end		  
			end 
		end
	end
end

endmodule