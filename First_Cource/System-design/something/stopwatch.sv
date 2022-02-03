`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 24.04.2021 12:39:29
// Design Name: 
// Module Name: Project
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Project(
    input clk,
    output logic [6:0] dig0,
    output logic [6:0] dig1,
    output logic [6:0] dig2,
    output logic [6:0] dig3,
    output logic [8:0] led
    );   
    logic [23:0] cnt = 0;
    logic [16:0] count = 0;
    logic [5:0] second = 0;
    logic [5:0] minute = 0;
    logic [4:0] hour = 0;
    
    Seven_segments b0 (
      .digit(hour / 10),
      .out(dig0)
    );
    
    Seven_segments b1 (
      .digit(hour % 10),
      .out(dig1)
    );
    
    Seven_segments b2 (
      .digit(minute / 10),
      .out(dig2)
    );
    
    Seven_segments b3 (
      .digit(minute % 10),
      .out(dig3)
    );
    
    always_ff@(posedge clk)
    begin
         if (cnt == 12000000)
         begin
              cnt = 0;
              count++;
              if (count == 86400) count = 0;
              second = count % 60;
              minute = (count / 60) % 60;
              hour = (count / 3600) % 24;  
              for (int i = 0; i < 8; i++)
                 begin
                    led[i] = second % 2;
                    second /= 2;
                 end           
         end
         else cnt++;

    end
endmodule
