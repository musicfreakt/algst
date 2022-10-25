module decod(in, out);

parameter delay = 1;
parameter n = 3;
parameter u = 8;

input wire [n-1:0] in;
output reg [u-1:0] out;
reg [n:0] i;

always @ (in)
begin #delay;
    for (i = 0; i < u; i = i + 1)
       out[i] = (in == i) ? 1 : 0;
end

endmodule
