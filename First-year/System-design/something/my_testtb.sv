`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 13.02.2021 12:14:34
// Design Name: 
// Module Name: my_testtb
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
module my_testtb;

    logic [1:0] arg;
    logic res;
    logic err;

    My_test dut(
        .in ( arg ),
        .q  ( res )
    );
    initial
    begin
        err = 0;

        arg = 2'b00;
        #10;
        if (res != 0)
            err = 1;

        arg = 2'b01;
        #10;
        if (res != 1)
            err = 1;

        arg = 2'b10;
        #10;
        if (res != 1)
            err = 1;

        arg = 2'b11;
        #10;
        if (res != 0)
            err = 1;

        if (err)
            $display("Test failed");
        else
            $display("Test passed");

        $finish;
    end

endmodule