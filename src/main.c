#include "tetric.c"

int main() {
    struct termios term_config;
    disable_canonical_stdin(&term_config);
    hide_cursor();
    int term_rows, term_cols;
    get_terminal_size(&term_rows, &term_cols);

    srand(time(NULL));
    clock_t fall_control, fps_control;
    
    GameState gs = {
        .board = {{0}},
        .points = 0,
        .level = 0,
        .total_rows = 0,
        .fall_time = 0.8,
        .piece = {0},
        .p_held = {0},
        .p_next = new_piece(rand() % 7 + 1),
        .hard_dropped = 0,
        .was_held = 0,
    };
    
    initialize_screen(term_rows, term_cols, &gs);

    for (;;) {
        gs.hard_dropped = 0;
        gs.was_held = 0;

        fall_control = clock();
        fps_control = clock();

        gs.piece = gs.p_next;
        gs.p_next = new_piece(rand() % 7 + 1);
        render_next(gs.p_next, gs.next_xy);

        if (piece_board_collision(gs.board, gs.piece)) { // filled screen
            stamp_piece(gs.board, gs.piece);
            render_board(gs.board, gs.board_xy, gs.piece);
            quit(&term_config, term_rows, term_cols);
        }

        stamp_piece(gs.board, gs.piece);

        // UPDATE PIECE
        fall_control = clock();
        while (down_is_valid(gs.board, gs.piece)) {
piece_falling:
            if (((double)clock() - fps_control)/CLOCKS_PER_SEC >= 1.0/FPS) {
                fps_control = clock();
                render_board(gs.board, gs.board_xy, gs.piece);
            }
            if (((double)clock() - fall_control)/CLOCKS_PER_SEC >= gs.fall_time) {
                fall_control = clock();
                update_falling_piece(gs.board, &gs.piece);
            }
            if (kb_hit()) {
                process_keypress(fgetc(stdin), &term_config, term_rows, term_cols, &gs);
                if (gs.was_held) {
                    render_hold(gs.p_held, gs.hold_xy);
                    render_next(gs.p_next, gs.next_xy);
                }
            }
        }

        // wiggle room after touching ground
        fall_control = clock();
        while (!gs.hard_dropped &&
               ((double)clock() - fall_control)/CLOCKS_PER_SEC < 1.25*gs.fall_time) {
            if (((double)clock() - fps_control)/CLOCKS_PER_SEC >= 1.0/FPS) {
                fps_control = clock();
                render_board(gs.board, gs.board_xy, gs.piece);
            }
            if (kb_hit()) {
                process_keypress(fgetc(stdin), &term_config, term_rows, term_cols, &gs);
                if (gs.was_held) {
                    render_hold(gs.p_held, gs.hold_xy);
                    render_next(gs.p_next, gs.next_xy);
                }
            }

            if (down_is_valid(gs.board, gs.piece)) { // is now off of a ledge
                goto piece_falling;
            }
        }
        int rows_cleared = 0;
        clear_filled_rows(gs.board, &rows_cleared);
        if (rows_cleared > 0) {
            update_game_state(&gs, rows_cleared);
            render_points(gs.points, gs.points_xy);
            render_level(gs.level, gs.level_xy);
        }
        gs.total_rows += rows_cleared;
    }

    return 0;
}
