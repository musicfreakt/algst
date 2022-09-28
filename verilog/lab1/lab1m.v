module lab1_m;
 reg x_0,x_1,x_2, z_0,z_1;
 reg y0, y1, y2, y3;
 reg [2:0] takt;
 initial begin   x_0='b0; x_1='b0;  x_2='b0;
         $monitor( "takt = %d, %b %b1 %b1,%b %b1", 
                   takt, x_0, x_1,x_2, z_0,z_1);
     for (takt=0;takt!=3'b111; takt=takt+1)
         begin
         #20 x_0= takt[0]; 
         x_1= takt[1]; 
         x_2= takt[2]; 
         end
     $stop;
     end
 always @(x_0,x_1,x_2)
      begin  y0 <= ~x_0 && ~x_1;
           y1 <= y0 && ~x_2;
             y2 <= x_0 && x_2;
             y3 <= ~x_0 && ~x_2 ; 
            z_0 <=  y1 || y2 || y3;
             z_1 <=  y1 ||  y0 || x_2 ; 
      end
endmodule
