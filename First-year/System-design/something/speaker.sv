`timescale 1ns / 1ps

module speaker(
    input [14:0] Hz,
    input clk,
    input [2:0] btn,
    output out
    );
    logic [14:0] cnt = 0;
    logic out_reg = 0;
    assign out = out_reg;
    always_ff@(posedge clk)
    begin
    if (btn) begin
         if (cnt == Hz)
         begin
              cnt = 0;
              out_reg = !out_reg;
         end
         else cnt++;
    end
    end
endmodule
