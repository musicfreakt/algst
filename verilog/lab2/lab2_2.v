module lab2;

parameter truth_table = 8'b01100001;
wire error;
wire z_t;
reg z;
reg [2:0] x;
integer takt;

initial 
begin
    x <= 3'b000;
    $monitor( "takt = %d, %b %b %b, %b %b", takt, x[2], x[1] ,x[0], z ,z_t);
    for (takt = 0; takt < 8; takt = takt + 1)
    begin
      #20
      x <= takt; 
    end
    #20 $stop;
end

assign error = (z == z_t) ? 0 : 1;
assign z_t = truth_table[{x[2], x[1], x[0]}];

always @(*) 
begin 
  if (x[2] == 0)
    z <= ~(x[1] | x[0]);
  else
    z <= (x[1] ^ x[0]);
end

endmodule