`timescale 1ns / 100ps

// motor_test
module MOTOR #(
    parameter BASE_ADDR = 16'h0000
)
(
    // Inputs
    PADDR,
    PCLK,
    PENABLE,
    PRESERN,
    PSEL,
    PWDATA,
    PWRITE,
    // Outputs
    HBRIDGE_CMD,
    PRDATA,
    PREADY,
    PSLVERR,
    PWM_OUT
);

localparam HBRIDGE_ADDR = BASE_ADDR;
localparam PWM_ADDR = BASE_ADDR + 16'h0008;

input  [31:0] PADDR;
input         PCLK;
input         PENABLE;
input         PRESERN;
input         PSEL;
input  [31:0] PWDATA;
input         PWRITE;

output [1:0]  HBRIDGE_CMD;
output [31:0] PRDATA;
output        PREADY;
output        PSLVERR;
output        PWM_OUT;

wire HBRIDGE_EN;
wire [31:0] PRDATA_0;

assign PREADY = 1'b1;
assign PSLVERR = 1'b0;
assign PRDATA = PRDATA_0;

HBRIDGE #( 
        .BASE_ADDR ( HBRIDGE_ADDR ) 
)
HBRIDGE_0(
        // Inputs
        .PCLK        (PCLK),
        .PRESERN     (PRESERN),
        .PSEL        (PSEL),
        .PENABLE     (PENABLE),
        .PWRITE      (PWRITE),
        .PADDR       (PADDR),
        .PWDATA      (PWDATA),
        // Outputs
        .PREADY      (),
        .PSLVERR     (),
        .PRDATA      (),
        .HBRIDGE_CMD (HBRIDGE_CMD),
        .HBRIDGE_EN  (HBRIDGE_EN) 
        );

PWM #( 
        .BASE_ADDR              (PWM_ADDR),
        .DEFAULT_PERIOD_CC      (5000),
        .DEFAULT_PULSE_WIDTH_CC (2500)
)
PWM_0(
        // Inputs
        .PCLK    (PCLK),
        .PRESERN (PRESERN),
        .PSEL    (PSEL),
        .PENABLE (PENABLE),
        .PWRITE  (PWRITE),
        .PADDR   (PADDR),
        .PWDATA  (PWDATA),
        .PWM_EN  (1'b1),
        // Outputs
        .PREADY  (),
        .PSLVERR (),
        .PRDATA  (PRDATA_0),
        .PWM_OUT (PWM_OUT) 
        );
endmodule