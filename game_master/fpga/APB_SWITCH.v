module apb_switch(
input PCLK,
input PRESERN,
input PSEL,
input PENABLE,
output PREADY,
output PSLVERR,
input PWRITE,
input [31:0] PADDR,
input wire [31:0] PWDATA,
output reg [31:0] PRDATA,
output EN
);
`define ADDR 16'h1000

assign PSLVERR = 0;
assign PREADY = 1;
wire write_enable = (PENABLE & PWRITE & PSEL & (PADDR[15:0] == `ADDR));

reg EN_REG;
assign EN = EN_REG;

always @(posedge PCLK) begin
    if(~PRESERN) begin
        EN_REG <= 0;
    end
    else begin
        PRDATA[31:1]    <= 0;
        PRDATA[0]       <= EN_REG;
        if(write_enable) begin
            EN_REG      <= PWDATA[0];
        end
    end
end
endmodule