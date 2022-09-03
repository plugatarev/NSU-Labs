`timescale 1ns / 1ps

module STOPWATCH(
    input clk,
    input [7:0] btn,
    input [3:0] flag,
    input [3:0] state,

    output logic [6:0] out0,
    output logic [6:0] out1,
    output logic [6:0] out2,
    output logic [6:0] out3
    );
    
    logic [23:0] cnt = 0;
    logic [16:0] count = 0;
    logic [5:0] second = 0;
    logic [5:0] minute = 0;
    assign out0 = minute / 10;    
    assign out1 = minute % 10;
    assign out2 = second / 10;
    assign out3 = second % 10;
    logic butt;
    always_ff@(posedge clk)
    begin
         if (state == 1) begin
             if (flag == 7) butt = 1;
             if (flag == 6) butt = 0;
             if (flag == 5) 
                begin
                    count = 0;
                    second = 0;
                    minute = 0;
                end
             if (butt)
                begin
                     if (cnt == 4)
                     begin
                          cnt <= 0;
                          count <= count + 1;
                          if (count == 3600) count <= 0;
                          second <= count % 60;
                          minute <= (count / 60) % 60;          
                     end
                     else cnt++;
                end
        end
    end
endmodule
