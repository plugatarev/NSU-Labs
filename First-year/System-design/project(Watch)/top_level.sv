`timescale 1ns / 1ps

module top_level(
    input [7:0] btn,
    input clk,
    output logic [6:0] dig0,
    output logic [6:0] dig1,
    output logic [6:0] dig2,
    output logic [6:0] dig3,
    output logic [8:0] led,
    output speaker

    );
    logic [3:0] state1 = 0;
    logic [3:0] state = 0;
    butten DUT (
        .clk(clk),
        .btn(btn),
        .flag(state)
    );
    
    logic [6:0] out_dig0;
    logic [6:0] out_dig1;
    logic [6:0] out_dig2;
    logic [6:0] out_dig3;
    
    Seven_segments b0 (
      .digit(out_dig0),
      .out(dig0)
    );
    
    Seven_segments b1 (
      .digit(out_dig1),
      .out(dig1)
    );
    
    Seven_segments b2 (
      .digit(out_dig2),
      .out(dig2)
    );
    
    Seven_segments b3 (
      .digit(out_dig3),
      .out(dig3)
    );
  
    logic [6:0] watch0;
    logic [6:0] watch1;
    logic [6:0] watch2;
    logic [6:0] watch3;
    logic [8:0] watch_led;
    WATCH DUT1(
       .clk(clk),
       .btn(btn),
       .flag(state),
       .state(state1),
       .out0(watch0),
       .out1(watch1),
       .out2(watch2),
       .out3(watch3),
       .out_led(watch_led)     
    );
    
    logic [6:0] stopwatch0;
    logic [6:0] stopwatch1;
    logic [6:0] stopwatch2;
    logic [6:0] stopwatch3;
    STOPWATCH DUT2(
       .clk(clk),
       .btn(btn),
       .flag(state),
       .state(state1),
       .out0(stopwatch0),
       .out1(stopwatch1),
       .out2(stopwatch2),
       .out3(stopwatch3)    
       );
    
    logic [6:0] timer0;
    logic [6:0] timer1;
    logic [6:0] timer2;
    logic [6:0] timer3;
    TIMER DUT4(
       .clk(clk),
       .btn(btn),
       .flag(state),
       .state(state1),
       .out0(timer0),
       .out1(timer1),
       .out2(timer2),
       .out3(timer3),
       .out_speaker(speaker)
    );    
    logic [6:0] tmp0;
    logic [6:0] tmp1;
    logic [6:0] tmp2;
    logic [6:0] tmp3;
    logic [8:0] tmp_led;
    logic [2:0] tmp;
    always_ff@(posedge clk)

    begin
        
        if (state == 0 || state == 0)
            begin
                state1 = 0;
                out_dig0 = watch0;
                out_dig1 = watch1;
                out_dig2 = watch2;
                out_dig3 = watch3;
                led = watch_led;                
            end 
       if (state == 1 || state == 1)
            begin
                state1 = 1;
                out_dig0 = stopwatch0;
                out_dig1 = stopwatch1;
                out_dig2 = stopwatch2;
                out_dig3 = stopwatch3;
            end 
        if (state == 2 || state1 == 2)
            begin
                state1 = 2;
                out_dig0 = timer0;
                out_dig1 = timer1;
                out_dig2 = timer2;
                out_dig3 = timer3;
            end 
        tmp <= state1; 
    end    
endmodule
