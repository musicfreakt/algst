module lab3(clock, z_0, z_1, z_2);

input wire clock;
output wire z_0, z_1, z_2;

wire [2:0] stim;
wire [7:0] y;

lpm_counter1 cnt(clock, stim);
lpm_decode1 decoder(stim, y[0], y[1], y[2], y[4], y[5], y[6], y[7]);

or or1(z_0, y[1], y[4], y[6]);
or or2(z_1, y[0], y[2], y[4], y[5], y[6]);
or or4(z_2, y[1], y[2], y[5], y[7]);


endmodule 
