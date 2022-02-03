`timescale 1ns / 1ps

module LEG(
    input clk,
  //  input digit,
    output logic rgbchain
    );
    logic [3:0] T = 0;
    logic digit;
    logic [23:0] rgb = 1023;
    always_ff@(posedge clk)
    begin
            if (digit == 0) 
            begin 
                  if (T < 5) rgbchain = 1;
                  else rgbchain = 0;
                  if (T == 15) T = 0;
                  else T++;
            end
            
            if (digit == 1) 
            begin
                if (T < 9) rgbchain = 1;
                else rgbchain = 0;
                if (T == 15) T = 0;
                else T++;
            end
       end
    
endmodule
