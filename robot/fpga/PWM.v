`timescale 1ns / 100ps

module PWM #(
    parameter BASE_ADDR = 16'h0000,
    parameter DEFAULT_PERIOD_CC = 32'd5000,
    parameter DEFAULT_PULSE_WIDTH_CC = 32'd2500
)
(
    input PCLK,
    input PRESERN,
    input PSEL,
    input PENABLE,
    output PREADY,
    output PSLVERR,
    input PWRITE,
    input [31:0] PADDR,
    input [31:0] PWDATA,
    output [31:0] PRDATA,
    input PWM_EN,
    output PWM_OUT
);
localparam PERIOD_CC_REG_ADDR = BASE_ADDR;
localparam PULSE_WIDTH_CC_REG_ADDR = BASE_ADDR + 4;

assign PSLVERR = 0;
assign PREADY = 1;
wire WRITE_EN = (PENABLE & PWRITE & PSEL);
wire PERIOD_CC_REG_SEL = (PADDR[15:0] == PERIOD_CC_REG_ADDR);
wire PULSE_WIDTH_CC_REG_SEL = (PADDR[15:0] == PULSE_WIDTH_CC_REG_ADDR);

reg [31:0] PERIOD_CC_REG;
reg [31:0] PULSE_WIDTH_CC_REG;
reg [31:0] PRDATA_REG;

reg [31:0] CC_COUNT_REG;
assign PWM_OUT = (PWM_EN & (CC_COUNT_REG < PULSE_WIDTH_CC_REG));
assign PRDATA = PRDATA_REG;

always @(posedge PCLK) begin
    if (!PRESERN) begin
        CC_COUNT_REG <= 0;
    end
    else if (CC_COUNT_REG == PERIOD_CC_REG) begin
        CC_COUNT_REG <= 0;
    end
    else begin
        CC_COUNT_REG <= CC_COUNT_REG + 1;
    end
end

always @(posedge PCLK) begin
    if (!PRESERN) begin
        PERIOD_CC_REG <= DEFAULT_PERIOD_CC;
        PULSE_WIDTH_CC_REG <= DEFAULT_PULSE_WIDTH_CC;
        PRDATA_REG <= 0;
    end
    else if(PULSE_WIDTH_CC_REG_SEL) begin
        PRDATA_REG <= PULSE_WIDTH_CC_REG;
        if(WRITE_EN) begin
            PULSE_WIDTH_CC_REG <= PWDATA;
        end
    end
    else if(PERIOD_CC_REG_SEL) begin
        PRDATA_REG <= PERIOD_CC_REG;
        if(WRITE_EN) begin
            PERIOD_CC_REG <= PWDATA;
        end
    end
    else begin
        PRDATA_REG <= 0;
    end
end

endmodule