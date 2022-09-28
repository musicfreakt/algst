module lab2_4;
parameter truth_table = 8'b01100001;
wire error;
wire z, z_t;
reg clock, D;
reg [2:0] x;
integer takt;
initial 
begin
    clock = 1'b0;
    x = 3'b000;
    $monitor("takt = %d, %b %b %b, %b %b", takt, x[2], x[1] ,x[0], z ,z_t);
    for (takt = 0; takt < 8; takt = takt + 1)
    begin 
    #20 clock = 1'b1;
    #5  x = takt;
    #5 clock = 1'b0;
    end
    #20 $stop;
end

assign error = (z == z_t) ? 0 : 1;
assign z_t = truth_table[x];
assign z = (~x[2] & ~x[1] & ~x[0]) | (x[2] & ~x[1] & x[0]) | (x[2] & x[1] & ~x[0]);

always @ (clock, z)
    if (clock) D = z;

endmodule
