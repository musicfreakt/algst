module st_m (
    reset,
    clock,
    in_data, 
    out_data
);
parameter state_len = 3;
parameter inp_len = 3;
parameter out_len = 2;
parameter [state_len - 1:0] 
    s0 = 3'b000,
    s1 = 3'b010,
    s2 = 3'b100,
    s3 = 3'b001;

parameter [inp_len - 1:0]
    x0 = 3'b001,
    x1 = 3'b010,
    x2 = 3'b100;

parameter [out_len - 1:0] 
    y0 = 2'b01,
    y1 = 2'b10,
    y2 = 2'b11;

input wire reset, clock;
input wire [inp_len - 1:0] in_data;
output reg [out_len - 1:0] out_data;

reg [state_len - 1:0] current_state, next_state;
// reg [out_len - 1:0] data_out;

always @ (posedge clock or posedge reset)
begin
    if (reset)
        current_state <= s0;
    else
        current_state <= next_state;
end

always @ (current_state, in_data)
begin 
   case(current_state)
        s0: if (in_data == x1) 
                next_state <= s1;    
            else if (in_data == x2) 
                next_state <= s2;
            else 
                next_state <= s0;
        s1: if (in_data == x0) 
                next_state <= s0;    
            else if (in_data == x1) 
                next_state <= s2;
            else 
                next_state <= s3; 
        s2: if (in_data == x1) 
                next_state <= s3;
            else
                next_state <= s0;
        s3: if (in_data == x2) 
                next_state <= s1;
            else 
                next_state <= s0; 
        default:
            next_state <= s0;
    endcase
end

always @(current_state)
begin
    case (current_state)
        s0: out_data <= y0; 
        s1: out_data <= y1;
        s2: out_data <= y1;
        s3: out_data <= y2;
        default: out_data <= y0;
    endcase
end

endmodule

