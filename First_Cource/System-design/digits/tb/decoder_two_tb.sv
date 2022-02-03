module decoder_test;
    logic [1:0] in;
    logic [3:0] out;
   
    decoder_two dut(
        .in(in),
        .out(out)
    );
    
    logic err = 0;

    initial begin
        in = 0; #10;
        if (out != 4'b0001) err = 1;
        in = 1; #10
        if (out != 4'b0010) err = 1;
        in = 2; #10
        if (out != 4'b0100) err = 1;
        in = 3; #10
        if (out != 4'b1000) err = 1;
        $display("########################################################");
        if (err)
            $display("Test failed");
        else
            $display("Test passed");

        $finish;
    end
endmodule