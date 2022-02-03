`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 24.04.2021 14:12:22
// Design Name: 
// Module Name: CUSTAMIZATION
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


module CUSTAMIZATION(
    input clk,
    input [3:0] btn,
    output logic [3:0] out0,
    output logic [3:0] out1,
    output logic [3:0] out2,
    output logic [3:0] out3
    );
    
    logic [4:0] cnt1 = 0;
    logic [3:0] btn1;
    logic [22:0] cooldown = 1200000; 
    assign  btn1 = ~btn;
    logic [3:0] btn_d;
    always_ff@(posedge clk)
    begin
        if (cooldown == 0) begin
              btn_d <= btn;
              if (btn1[0] && btn_d[0]) out0 = (out0 + 1) % 6;
              if (btn1[1] && btn_d[1]) out1 = (out1 + 1) % 10;
              if (btn1[2] && btn_d[2]) out2 = (out2 + 1) % 6;  
              if (btn1[3] && btn_d[3]) out3 = (out3 + 1) % 10;
              cooldown = 1200000;
            end
        else begin
            cooldown-=1;
        end
    end    
endmodule
