module lab2;

parameter truth_table = 8'b01100001;
wire error;
wire z, z_t;
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

assign error = (z == z_t) ? 0 : 1;sim:/lab2/truth_table sim:/lab2/error sim:/lab2/z sim:/lab2/z_t sim:/lab2/x sim:/lab2/takt
assign z_t = truth_table[{x[2], x[1], x[0]}];
assign z = (~x[2] & ~x[1] & ~x[0]) | (x[2] & ~x[1] & x[0]) | (x[2] & x[1] & ~x[0]);

endmodule
