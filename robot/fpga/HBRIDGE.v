`timescale 1ns / 100ps

module HBRIDGE #(
    parameter BASE_ADDR = 16'h0000
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
output [1:0] HBRIDGE_CMD,
output HBRIDGE_EN
);
localparam HBRIDGE_CMD_REG_ADDR = BASE_ADDR;
localparam HBRIDGE_EN_REG_ADDR = BASE_ADDR + 16'h0004;

assign PSLVERR = 0;
assign PREADY = 1;
wire WRITE_EN = (PENABLE & PWRITE & PSEL);
wire HBRIDGE_CMD_REG_SEL = (WRITE_EN & (PADDR[15:0] == HBRIDGE_CMD_REG_ADDR));
wire HBRIDGE_EN_REG_SEL = (WRITE_EN & (PADDR[15:0] == HBRIDGE_EN_REG_ADDR));

reg[1:0] HBRIDGE_CMD_REG;
reg HBRIDGE_EN_REG;
assign HBRIDGE_EN = HBRIDGE_EN_REG;
assign HBRIDGE_CMD = HBRIDGE_CMD_REG;
assign PRDATA = 0;

always @(posedge PCLK) begin
    if (!PRESERN) begin
        HBRIDGE_CMD_REG <= 2'b0;
        HBRIDGE_EN_REG <= 1'b0;
    end
    else if (HBRIDGE_CMD_REG_SEL) begin
        HBRIDGE_CMD_REG <= PWDATA[1:0];
    end
    else if(HBRIDGE_EN_REG_SEL) begin
        HBRIDGE_EN_REG <= PWDATA[0];
    end
end
endmodule