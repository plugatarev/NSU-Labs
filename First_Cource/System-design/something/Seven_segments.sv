`timescale 1ns / 1ps
module Seven_segments(
    input [3:0] digit,
    output logic [6:0] out
    );
       
    always_comb
    begin
        case(digit)         
            0: out = 7'b0111111;
            1: out = 7'b0000110; 
            2: out = 7'b1011011;
            3: out = 7'b1001111; 
            4: out = 7'b1100110; 
            5: out = 7'b1101101; 
            6: out = 7'b1111101; 
            7: out = 7'b0000111; 
            8: out = 7'b1111111;
            9: out = 7'b1100111; 
        endcase
    end
endmodule
