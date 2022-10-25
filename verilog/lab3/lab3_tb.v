module lab3_tb;

// parameter truth_table_0 = 8'b01011110;
parameter truth_table_0 = 8'b01010010;
parameter truth_table_1 = 8'b01110101;  
parameter truth_table_2 = 8'b10100110;

wire z_0, z_1, z_2;
reg clk;
reg chech_point, error_0, error_1, error_2;
wire vt_0, vt_1, vt_2;
integer i;

lab3 l(clk, z_0, z_1, z_2);

initial 
begin 
    clk = 0;
    for (i = 0; i < 8; i = i + 1)
        #10 clk = ~clk;
end

assign vt_0 = truth_table_0[i];
assign vt_1 = truth_table_1[i];
assign vt_2 = truth_table_2[i];

always @ (posedge chech_point)
begin
    error_0 = z_0 == vt_0 ? 0 : 1;
    error_1 = z_1 == vt_1 ? 0 : 1;
    error_2 = z_2 == vt_2 ? 0 : 1;
end

endmodule 