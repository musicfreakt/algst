`timescale 1ns/100ps

module lab2;
parameter truth_table=8'bxxxxxxxx //change
wire error;
reg x_0, x_1, x_2, z, clock;
reg [3:0] takt;
reg D;
initial begin
  clock='b0;
  x_0='b0;
  x_1='b0;
  x_2 ='b0;
$monitor("takt = %d, %b %b1 %b1,%b %b1", takt, x_0, x_1, x_2, z_0, z_1);
for (takt=0;takt!=8;takt=takt+1)
  begin
  #20 // clock='b1;
   #5  x_0= takt[0];
       x_1= takt[1];
       x_2= takt[2];
 // #5 clock='b0 ;
  end;
$finish;
end;
assign error = (z1 == truth_table[{x_2, x_1, x_0}]) ? 0 : 1;

always @(x0,x1,x2) 
begin 
 // todo;
end;
endmodule;

primitive my_logic(zt,a,b,c)
output zt;
input a,b,c;
 table // todo
       // a, b, c : zt;
          ?  ?  ? : 0;
endtable;
endprimitive;
