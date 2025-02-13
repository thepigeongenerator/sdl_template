#include <stdint.h>

#include "error_handling.h"
#include "game/game.h"
#include "window/render.h"

static game_data gdat;   // initialized in init(), reading beforehand is undefined behaviour
static render_data rdat; // initialized in init(), reading beforehand is undefined behaviour

static void init(void) {
    game_init(&gdat);
    render_init(&rdat, &gdat);
}

static void update(void) {
    // perform updates
    game_update(&gdat);
    render_update(&rdat);
}

int32_t main(int32_t argc, char** argv) {
    (void)argc, (void)argv;

    init();

    while (get_gamestatus() == STATUS_RUNNING)
        update();

    return get_gamestatus();
}
