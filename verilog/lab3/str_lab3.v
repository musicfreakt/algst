module complex(x, z_0, z_1, z_2);

input wire [2:0] x;
output wire z_0, z_1, z_2;

wire [7:0] y;

decod mod1(x, y);

or mod2(z_0, y[1], y[4], y[6]);
or mod3(z_1, y[0], y[2], y[4], y[5], y[6]);
or mod4(z_2, y[1], y[2], y[5], y[7]);

endmodule 
