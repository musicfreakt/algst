module lab3(clock, z_0, z_1, z_2);

output wire z_0, z_1, z_2;
reg [2:0] stim;

// добавляем двоичный счетчик и генерируем стим

complex m1(stim, z_0, z_1, z_2);

// initial 
// begin 
//     chech_point = 0;
//     for (j = 0; j <= 7; j = j + 1) 
//     begin #10;
//        stim = j;
//        chech_point = # delay 1;
//        #3;
//        chech_point = 0;  
//     end
// end

endmodule 
