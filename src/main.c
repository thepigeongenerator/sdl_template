#include "main.h"

#include <stdint.h>

#include "error_handling.h"

static gamestatus status = STATUS_RUNNING;

void set_gamestatus(gamestatus new_status) {
    status = new_status;
}

int32_t main(int32_t argc, char** argv) {
    (void)argc, (void)argv;
    debug("%s", "Hello, World!");
    return status;
}
