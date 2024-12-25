// @Hustler's Project

#include <serial.h>
#include <print.h>

struct serial {
    u32 baudrate, clock_hz, data_bits, parity, stop_bits, fifo_size, irq;
    u64 io_base;
    u64 io_size;
    u8 lsr_mask;
};

void serial_putc(char c) {

}

char serial_getc(void) {
    return 'c';
}

void serial_outs(const char *s) {

}

int serial_init(void) {
    set_outs(serial_outs);
    set_cin(serial_getc);
    set_cout(serial_putc);

    return 0;
}
