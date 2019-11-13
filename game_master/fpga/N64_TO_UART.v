module N64_TO_UART(EN, PCLK, PRESETN, FIFO_FULL, FIFO_EMPTY, SIN, SOUT, FABINT, N64_INOUT, DEBUG);
input            EN, PCLK, PRESETN, FIFO_FULL, FIFO_EMPTY, SIN;
inout            N64_INOUT;
output reg       SOUT;
output reg       FABINT;
output wire[4:0] DEBUG;

`define         STATE_CMD          3'b000
`define         STATE_RESPOND      3'b001
`define         STATE_READ         3'b010
`define         STATE_STALL        3'b011
`define         STATE_IDLE         3'b100

`define         CMD_BIT_DURATION   32'd96                      
`define         CMD_POLL           {1'b0, 32'hE8888888, 1'b1}
`define         CMD_SENTINEL       {1'b1, 32'hD1111111, 1'b0}
`define         REPONSE_DURATION   32'd24000000
`define         STALL_DURATION     32'd24000000

reg[33:0]       CMD_REG;
reg             CMD_REG_SHIFT;

reg[31:0]       GP_CNT_REG;
reg             GP_CNT_RESET;

reg[2:0]        PREV_STATE_REG;
reg[2:0]        STATE_REG;
reg[2:0]        NEXT_STATE;

reg[1:0] FIFO_FULL_SYNC_REG;
always @(posedge PCLK) begin
    FIFO_FULL_SYNC_REG <= {FIFO_FULL_SYNC_REG[0], FIFO_FULL};
    FABINT <= 1'b0;
    if(FIFO_FULL_SYNC_REG == 2'b01) begin
        FABINT <= 1'b1;
    end
end

assign DEBUG[0] = N64_INOUT;
assign DEBUG[1] = CMD_REG[0];
assign DEBUG[4:2] = STATE_REG;

reg               N64_WIRE;
assign N64_INOUT = N64_WIRE;

wire BIT_SENT = (GP_CNT_REG == `CMD_BIT_DURATION);
wire CMD_SENT =  BIT_SENT & (CMD_REG == `CMD_SENTINEL);
wire STALL_COMPLETE = (STATE_REG == `STATE_STALL);
wire RESPONSE_TIMEOUT = (GP_CNT_REG > `REPONSE_DURATION); 
wire STATE_CHANGE = (STATE_REG != PREV_STATE_REG);

always @(posedge PCLK) begin
    if(~PRESETN) begin
        PREV_STATE_REG <= `STATE_IDLE;
        STATE_REG <= `STATE_IDLE;
        GP_CNT_REG <= 0;
        CMD_REG <= `CMD_POLL;
    end
    else begin
        STATE_REG <= NEXT_STATE;
        PREV_STATE_REG <= STATE_REG;
        if(CMD_REG_SHIFT) begin
            CMD_REG <= {CMD_REG[0], CMD_REG[33:1]};
        end
        if(STATE_CHANGE | GP_CNT_RESET) begin
            GP_CNT_REG <= 0;
        end
        else begin
            GP_CNT_REG <= GP_CNT_REG + 1;
        end
    end
end

always @(*) begin
    SOUT = 1'b1;
    N64_WIRE = 1'bz;
    NEXT_STATE = STATE_REG;
    GP_CNT_RESET = STATE_CHANGE;
    CMD_REG_SHIFT = 1'b0;
    if(~EN) begin
        NEXT_STATE = `STATE_IDLE;
    end
    else begin
        case(STATE_REG)
            `STATE_CMD : begin
                N64_WIRE = CMD_REG[0];
                if(CMD_SENT) begin
                    NEXT_STATE = `STATE_RESPOND;
                end
                if(BIT_SENT) begin
                    GP_CNT_RESET = 1'b1;
                    CMD_REG_SHIFT = 1'b1;
                end
            end
            `STATE_RESPOND : begin
                SOUT = N64_INOUT;
                if(RESPONSE_TIMEOUT) begin
                    NEXT_STATE = `STATE_CMD;
                end
                if(FIFO_FULL) begin
                    NEXT_STATE = `STATE_READ;
                end
            end
            `STATE_READ : begin
                if(FIFO_EMPTY) begin
                    NEXT_STATE = `STATE_STALL;
                end
            end
            `STATE_STALL : begin
               if(STALL_COMPLETE) begin
                   NEXT_STATE = `STATE_CMD;
               end
           end
            `STATE_IDLE : begin
                if(EN) begin
                    NEXT_STATE = `STATE_CMD;
                end
            end
        endcase
    end
end

endmodule