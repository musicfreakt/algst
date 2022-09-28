primitive my_logic(zt,a,b,c);
output zt;
input a,b,c;
 table
       // a, b, c : zt;
          0  0  0 : 1;
          0  0  1 : 0;
          0  1  0 : 0;
          0  1  1 : 0;
          1  0  0 : 0;
          1  0  1 : 1;
          1  1  0 : 1;
          1  1  1 : 0;
endtable
endprimitive