module test;

parameter delay = 6;

// parameter truth_table_0 = 8'b01011110;
parameter truth_table_0 = 8'b01010010;
parameter truth_table_1 = 8'b01110101;  
parameter truth_table_2 = 8'b10100110;

wire z_0, z_1, z_2;
reg [2:0] stim;
reg [3:0] j;
reg chech_point, error_0, error_1, error_2;
wire vt_0, vt_1, vt_2;

complex m1(stim, z_0, z_1, z_2);

initial 
begin 
    chech_point = 0;
    for (j = 0; j <= 7; j = j + 1) 
    begin #10;
       stim = j;
       chech_point = # delay 1;
       #3;
       chech_point = 0;  
    end
end

assign vt_0 = truth_table_0[stim];
assign vt_1 = truth_table_1[stim];
assign vt_2 = truth_table_2[stim];

always @ (posedge chech_point)
begin
    error_0 = z_0 == vt_0 ? 0 : 1;
    error_1 = z_1 == vt_1 ? 0 : 1;
    error_2 = z_2 == vt_2 ? 0 : 1;
end

endmodule 
