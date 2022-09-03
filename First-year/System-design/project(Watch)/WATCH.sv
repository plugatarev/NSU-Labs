`timescale 1ns / 1ps
module WATCH(
    input clk,
    input [7:0] btn,
    input [3:0] flag,
    input [3:0] state,
    output logic [6:0] out0,
    output logic [6:0] out1,
    output logic [6:0] out2,
    output logic [6:0] out3,
    output logic [8:0] out_led
    );   
   
    logic [23:0] cnt = 0;
    logic [16:0] count = 0;
    logic [5:0] second = 0;
    logic [5:0] minute = 0;
    logic [4:0] hour = 0;
    
    logic [7:0] btn_d = 0;
    logic [7:0] btn1;
    logic [23:0] cooldown = 0; 
    assign  btn1 = ~btn;
                 
    assign out0 = hour / 10;
    assign out1 = hour % 10;
    assign out2 = minute / 10;
    assign out3 = minute % 10;
    always_ff@(posedge clk)
    begin
        btn_d <= btn;
        if (state == 0  && (flag == 3 || flag == 4 || flag == 5 || flag == 6))
        begin
            if (cooldown == 4) cooldown = 0;
            if (cooldown == 0) begin
                if (btn1[3] && btn_d[3]) begin out0 <= (out0 + 1) % 3; cooldown = 1; end
                if (btn1[4] && btn_d[4]) begin out1 <= (out1 + 1) % 4; cooldown = 1; end
                if (btn1[5] && btn_d[5]) begin out2 <= (out2 + 1) % 6;cooldown = 1; end
                if (btn1[6] && btn_d[6]) begin out3 <= (out3 + 1) % 10; cooldown = 1; end
             end   
             else cooldown+=1;
             count = out0 * 3600 * 10 + out1 * 3600 + out2 * 60 * 10 + out3 * 60;
                                         
         end
         if ( (state == 0 && (flag == 0 || flag == 1 || flag == 2)) || state == 1 || state == 2) 
         begin
             if (cnt == 4)
             begin
                  cnt <= 0;
                  count <= count + 1;
                  if (count == 86400) count = 0;
                  second <= count % 60;
                  minute <= (count / 60) % 60;
                  hour <= (count / 3600) % 24;  
                  for (int i = 0; i < 8; i++)
                     begin
                        out_led[i] = second % 2;
                        second /= 2;
                     end           
             end
             else cnt++;
        end

    end
endmodule
