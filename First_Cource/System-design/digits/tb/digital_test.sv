`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 13.02.2021 15:17:21
// Design Name: 
// Module Name: digital_test
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


module digital_test;
    logic [6:0] out;
    logic [3:0] in;
   
    digital dut(
        .number(in),
        .segments(out)
    );
    
    logic err = 0;

    initial begin
        in = 0;
        #10;
        if (out != 'b0111111) err = 1;
        
        in++; 
        #10;
        if (out != 'b0000110) err = 1;
        
        in++; 
        #10;
        if (out != 'b1011011) err = 1;
        
        in++; 
        #10;
        if (out != 'b1001111) err = 1;
        
        in++; 
        #10;
        if (out != 'b1100110) err = 1; 
        
        in++; 
        #10;
        if (out != 'b1101101) err = 1; 
        
        in++; 
        #10;
        if (out != 'b1111101) err = 1; 
        
        in++; 
        #10;
        if (out != 'b0000111) err = 1;
        
        in++; 
        #10;
        if (out != 'b1111111) err = 1;
        
        in++; 
        #10;
        if (out != 'b1100111) err = 1;   
          
        $display("########################################################");
        if (err)
            $display("Test failed");
        else
            $display("Test passed");

        $finish;
    end
endmodule
