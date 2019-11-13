

module negate(uart_in, led_out);
input uart_in;
output led_out;

assign led_out = ~uart_in;

endmodule

