module lab1 (x_0,x_1,x_2,z_0,z_1);
parameter delay=1;
input x_0, x_1, x_2;
output z_0, z_1;
wire x_0, x_1, x_2;
wire y0, y1, y2, y3;
reg z_0, z_1;
always @(y0, y1, y2, y3, x_2)
begin # delay
  z_0 = y1 || y2 || y3;
  z_1 = y1 || y0 || x_2;
end
assign #delay
  y1 = y0 && ~x_2,
  y2 = x_0 && x_2,
  y3 = ~x_0 && ~x_2,
  y0=~x_0 && ~x_1;
endmodule;