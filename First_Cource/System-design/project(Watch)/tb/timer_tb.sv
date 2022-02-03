module timer_tb;

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
    logic [3:0] my_state = 2;
    logic [3:0] my_flag = 0;

    logic [7:0] my_led;
    TIMER j(
        .clk(res),
        .btn(~btn),
        .flag(my_flag),
        .state(my_state),
        .out0(out_0),
        .out1(out_1),
        .out2(out_2),
        .out3(out_3),
        .out_speaker(speak)
    );
    
     always #5 
     res = !res;
     initial
     begin
        #20

        //test timer
        #20
        btn = 8; my_flag = 3;#50;
        btn = 0;#50;
        btn = 8; my_flag = 3;#50;
        btn = 0;#50
        btn = 16; my_flag = 4;#50;
        btn = 0;#50;
        btn = 32; my_flag = 5;#50;
        btn = 0;#50;
        btn = 64; my_flag = 6;#50;
        btn = 0;#50;
        btn = 2; my_flag = 2;
     end
endmodule