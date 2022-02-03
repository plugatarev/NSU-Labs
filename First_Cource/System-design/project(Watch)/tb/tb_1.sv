module tb_1;

    logic [4:0] out = 0;
    logic         res = 0;
    logic [25:0] cnt = 0;
    logic [7:0] btn;
    logic [4:0] R = 0;
    logic [6:0] out_1;
    logic [6:0] out_2;
    logic [6:0] out_3;
    logic [6:0] out_0;
    logic speak = 0;
    logic [3:0] my_state = 0;
    logic [7:0] my_led;
    top_level j(
        .btn(~btn),
        .clk(res),
        .dig0(out_0),
        .dig1(out_1),
        .dig2(out_2),
        .dig3(out_3),
        .led(my_led),
        .speaker(speak)
    );
    
     always #5 
     res = !res;
     initial
     begin
       
        //test watch
        #20
        btn = 0;#30;
        #50;
        btn = 8;#30;
        btn = 0;#30;
        btn = 8;#30;
        btn = 0;#30;
        btn = 8;#30;
        btn = 0;#30;
        btn = 8;#30;
        btn = 0;#30;
        btn = 16;#30;
        btn = 0;#30;
        btn = 32;#30;
        btn = 0;#30;
        btn = 64;#30;
        btn = 0;#30;
       
        //test stopwatch
        btn = 2;#50;
        btn = 0;#50;
        btn = 128;#30;
        #400;
        btn = 64;#50;
        btn = 32;#50;
       
        //test timer
        btn = 4;#30;
        btn = 0;#30;
        btn = 32;#30;
        btn = 0;#30;
        btn = 64;#30;
        btn = 0;#30;
        btn = 128;#30;
        btn = 0;#2000;
        btn = 16;#10;

        btn = 16;#30;
     end
endmodule