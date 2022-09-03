module watch_tb;

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
    logic [3:0] my_flag;
    logic [3:0] my_state = 0;
    WATCH G(
        .clk(res),
        .btn(~btn),
        .flag(my_flag),
        .state(my_state),
        .out0(out_0),
        .out1(out_1),
        .out2(out_2),
        .out3(out_3)
    );
    
     always #5 
     res = !res;
     initial
     begin
       
        //test watch
        #20
        btn = 8; my_flag = 3;#70;
        btn = 0;#70;
        btn = 8; my_flag = 3;#70;
        btn = 0;#70
        btn = 16; my_flag = 4;#70;
        btn = 0;#70;
        btn = 32; my_flag = 5;#70;
        btn = 0;#70;
        btn = 64; my_flag = 6;#70;
        btn = 0;#70;
        btn = 1; my_flag = 0;
        
     end
endmodule