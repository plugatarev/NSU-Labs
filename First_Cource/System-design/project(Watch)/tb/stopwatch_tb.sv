module stopwatch_tb;

    logic [4:0] out = 0;
    logic         res = 0;
    logic [25:0] cnt = 0;
    logic [7:0] btn;
    logic [4:0] R = 0;
    logic [6:0] out_1;
    logic [6:0] out_2;
    logic [6:0] out_3;
    logic [6:0] out_0;
    logic [3:0] my_state = 1;
    logic [3:0] my_flag;
    STOPWATCH DUT(
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
       
       #15;
        //test stopwatch
        my_flag = 7;#500;
        my_flag = 6;#20;
        my_flag = 5;#20;

        
       
     end
endmodule