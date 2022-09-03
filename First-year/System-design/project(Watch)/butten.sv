`timescale 1ns / 1ps

module butten(
    input clk,
    input [7:0] btn,
    output logic [3:0] flag
    );
    parameter par = 12;
    logic [7:0] btn1;
    logic [23:0] cooldown = 0; 
    assign  btn1 = ~btn;
    logic [4:0] flag_old = 0;
    logic [7:0] btn_d = 0;
    logic f = 0;
    always_ff@(posedge clk)
    begin
        f = 0;
        if (cooldown == 4) cooldown = 0;
        if (cooldown == 0) begin
              if (btn1[0] && btn_d[0]) begin flag = 0; f = 1; cooldown = 1; end
              if (btn1[1] && btn_d[1]) begin flag = 1; f = 1; cooldown = 1; end
              if (btn1[2] && btn_d[2]) begin flag = 2; f = 1; cooldown = 1; end
              if (btn1[3] && btn_d[3]) begin flag = 3; f = 1; cooldown = 1; end
              if (btn1[4] && btn_d[4]) begin flag = 4; f = 1; cooldown = 1; end
              if (btn1[5] && btn_d[5]) begin flag = 5; f = 1; cooldown = 1; end
              if (btn1[6] && btn_d[6]) begin flag = 6; f = 1; cooldown = 1; end
              if (btn1[7] && btn_d[7]) begin flag = 7; f = 1; cooldown = 1; end
        end
        else cooldown+=1;
        if (f == 0) flag = flag_old;
        flag_old <= flag;
        btn_d <= btn;

    end
    
endmodule
