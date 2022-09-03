`timescale 1ns / 1ps

module different_tons(
    input clk,
    input [2:0] btn,
    output speaker
    );
    logic [2:0] btn1;
    assign btn1 = ~btn;
    logic [14:0] Hz = 0;
    
    speaker DUT (
        .Hz(Hz),
        .clk(clk),
        .btn(btn1),
        .out(speaker)
    );
    
    always_comb 
        begin
           if (btn1[0] || btn1[1] || btn1[2]) 
            begin
                if (btn1[0]) Hz = 6000;
                if (btn1[1]) Hz = 15000;
                if (btn1[2]) Hz = 30000;
                
            end
        end
    
endmodule
