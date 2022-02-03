`timescale 1ns / 1ps

module piano_tb;
    logic clk = 0;
    logic [2:0] btn;
    different_tons dut(
        .clk(clk),
        .btn(btn),
        .speaker(speaker)
    );
     always #10 
     clk = !clk;
     initial
     begin
        btn[0] = 1; btn[1] = 1; btn[2] = 0; #50;
        btn[0] = 1; btn[1] = 0; btn[2] = 1; #50;
        btn[0] = 0; btn[1] = 1; btn[2] = 1; #50;
        
     end
endmodule