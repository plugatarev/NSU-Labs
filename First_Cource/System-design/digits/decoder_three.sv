`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 22.02.2021 21:33:36
// Design Name: 
// Module Name: decoder_three
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


module decoder_three(
    input logic [2:0] in,
    output logic [7:0] out
    );
    
always_comb begin
out[0] = (in == 0);
out[1] = (in == 1);
out[2] = (in == 2);
out[3] = (in == 3);
out[4] = (in == 4);
out[5] = (in == 5);
out[6] = (in == 6);
out[7] = (in == 7);
end
endmodule
