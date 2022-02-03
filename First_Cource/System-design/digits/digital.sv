`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 13.02.2021 14:26:01
// Design Name: 
// Module Name: digital
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


module digital(
    input logic [3:0] number,
    output logic [6:0] segments
    );
    always_comb
    begin
        case(number)         
            0: segments = 7'b0111111;
            1: segments = 7'b0000110; 
            2: segments = 7'b1011011;
            3: segments = 7'b1001111; 
            4: segments = 7'b1100110; 
            5: segments = 7'b1101101; 
            6: segments = 7'b1111101; 
            7: segments = 7'b0000111; 
            8: segments = 7'b1111111;
            9: segments = 7'b1100111; 
        endcase
    end
endmodule
