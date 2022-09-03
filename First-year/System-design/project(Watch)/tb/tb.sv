module my_xor_tb;

    logic [4:0] out = 0;
    logic         res = 0;
    logic [25:0] cnt = 0;
    logic [7:0] btn = 0;
    logic [4:0] R = 0;

    butten dut(
        .clk( res ),
        .btn(~btn),
        .flag(out)
    );
    
     always #5 
     res = !res;
     initial
     begin
        btn = 1;#200;
        btn = 2;#200;
        btn = 4;#200;
        btn = 8;#200;
     end
endmodule