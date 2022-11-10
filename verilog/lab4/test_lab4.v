module test_st_m;

reg reset, clock;
reg [2:0] in_data;
wire [1:0] out_data;
integer takt;

st_m v1(.in_data(in_data), .out_data(out_data), .reset(reset), .clock(clock));

initial 
begin 
    clock = 'b0;
    reset = 'b0;
    #5 reset = 'b1;
    #10 reset = 'b0;
    #5;
    for (takt = 0; takt < 8; takt = takt + 1)
    begin
        case (takt)
            0: in_data = 3'b001;
            1: in_data = 3'b010;
            2: in_data = 3'b100;
            3: in_data = 3'b001;
            4: in_data = 3'b100;
            5: in_data = 3'b010;
            6: in_data = 3'b100;
            7: in_data = 3'b010;
        endcase
        #5 clock = 'b1;
        #10 clock = 'b0;
        #20;
    end
    // $finish; 
end

endmodule

