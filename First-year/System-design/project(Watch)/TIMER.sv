`timescale 1ns / 1ps

module TIMER(
    input clk,
    input [7:0] btn,
    input [3:0] flag,
    input [3:0] state,
    output logic [6:0] out0,
    output logic [6:0] out1,
    output logic [6:0] out2,
    output logic [6:0] out3,
    output out_speaker

    );
    logic out = 0;
    assign out_speaker = out;
    logic ring = 0;
    logic [23:0] cnt = 0;
    logic [16:0] count = 1;
    logic [5:0] second = 0;
    logic [5:0] minute = 0;
    logic [7:0] btn_d;
    logic [7:0] btn1;
    logic [23:0] cooldown = 0; 
    assign  btn1 = ~btn;
    logic [14:0] d = 0;
    assign out0 = minute / 10;
    assign out1 = minute % 10;
    assign out2 = second / 10;
    assign out3 = second % 10;
    always_ff@(posedge clk)
    begin
        btn_d <= btn;
        if (ring)
        begin
                if (d == 2)
                    begin
                        d <= 0;
                        out = !out;
                    end
                else d++;
        end
        if (state == 2 && (flag == 3 || flag == 4 || flag == 5 || flag == 6))
        begin
            ring <= 0;
            if (cooldown == 4) cooldown = 0;
            if (cooldown == 0) begin
                if (btn1[3] && btn_d[3]) begin out0 <= (out0 + 1) % 6; end
                if (btn1[4] && btn_d[4]) begin out1 <= (out1 + 1) % 10; end
                if (btn1[5] && btn_d[5]) begin out2 <= (out2 + 1) % 6; end
                if (btn1[6] && btn_d[6]) begin out3 <= (out3 + 1) % 10; end 
             end   
             else cooldown+=1;
             count = out0 * 60 * 10 + out1 * 60 + out2 * 10 + out3;
        end
        if ( ((state == 1 || state == 3) && count != 1) || (state == 2 && (flag == 0 || flag == 1 ||flag == 2 || flag == 7) && !ring))
        begin
            if (cnt == 4)
            begin
              cnt <= 0;
              if (count == 0) ring = 1; 
              else count <= count - 1;
              second <= count % 60;
              minute <= (count / 60) % 60;          
            end
            else cnt++;        
        end
    
    end
endmodule
