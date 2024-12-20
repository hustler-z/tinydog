// @Hustler's Project

#include <board.h>
#include <config.h>

// ####################################################

#if ENABLED(RK3566)
struct board tiny_board = {
    .name = "rk3566-cortex-a55",
    .pa_start = 0x0,
    .pa_end = 0x0,
};
#endif

int board_init(void) {
    return 0;
}
