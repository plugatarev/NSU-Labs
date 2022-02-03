`timescale 1ns / 1ps

module digital(
    input [7:0] btn,
    output logic [6:0] dig0,
    output logic [6:0] dig1,
    output logic [6:0] dig2,
    output logic [6:0] dig3
    );
    
    logic [7:0] btn1;
    assign btn1 = ~btn;
    Seven_segments b0 (
      .digit(btn1[3:0]),
      .out(dig0)
    );
    
    Seven_segments b1 (
      .digit(btn1[7:4]),
      .out(dig1)
    );
    
    Seven_segments b2 (
      .digit( (btn1[3:0]+ btn1[7:4]) / 10),
      .out(dig2)
    );
    
    Seven_segments b3 (
      .digit((btn1[3:0]+ btn1[7:4]) % 10),
      .out(dig3)
    );
endmodule
