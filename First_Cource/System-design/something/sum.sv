`timescale 1ns / 1ps

module sum(
    input clk,
    input btn,
    output logic [6:0] dig0
    );
    
    logic [23:0] cnt = 0;
    logic [4:0] cnt1 = 0;
    
    Seven_segments b0 ( 
        .digit(cnt1), 
        .out(dig0)
        );
        
    always_ff@(posedge clk)
    begin
        cnt++;
        if (cnt == 12000000)
        begin        
            cnt1++;
            cnt = 0;
        end
        if (cnt1 > 9) cnt1 = 0; 
    end
    
endmodule
