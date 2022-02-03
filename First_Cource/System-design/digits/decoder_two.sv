`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 22.02.2021 19:58:05
// Design Name: 
// Module Name: decoder_two
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


module decoder_two(
    input logic [1:0] in,
    output logic [3:0] out
    );
always_comb begin
out[0] = (in == 2'b00);
out[1] = (in == 2'b01);
out[2] = (in == 2'b10);
out[3] = (in == 2'b11);
end
endmodule
