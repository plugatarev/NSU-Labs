`timescale 1ns / 1ps

module butten(
    input clk,
    input [3:0] btn,
    output logic [6:0] dig0
    );
    
    logic [4:0] cnt1 = 0;
    logic btn1;
    logic [22:0] cooldown = 1200000; 
    assign  btn1 = ~btn;
    logic btn_d;
    Seven_segments b0 ( 
        .digit(cnt1), 
        .out(dig0)
        );
        
    always_ff@(posedge clk)
    begin
        if (cooldown == 0) begin
              btn_d <= btn;
              if (btn1 && btn_d) cnt1 <= (cnt1 + 1) % 10;  
              cooldown = 1200000;
            end
        else begin
            cooldown-=1;
        end
    end
    
endmodule
