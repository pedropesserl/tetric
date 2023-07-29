#include "tetric.c"

int main() {
    struct termios term_config;
    disable_canonical_stdin(&term_config);
    hide_cursor();
    int term_rows, term_cols;
    get_terminal_size(&term_rows, &term_cols);

    int board[BDROWS][BDCOLS] = {0};
    srand(time(NULL));
    clock_t fall_control, fps_control;
    
    ItemsXY items_xy;
    int level = 1;
    int points = 0;
    float fall_time = 0.8;
    
    initialize_screen(term_rows, term_cols, &items_xy);

    Piece piece = {0}, piece_held = {0}, next_piece = new_piece(rand() % 7 + 1, fall_time);

    for (;;) {
        int hard_dropped = 0, was_held = 0;

        fall_control = clock();
        fps_control = clock();

        piece = next_piece;
        next_piece = new_piece(rand() % 7 + 1, fall_time);
        render_next(next_piece, items_xy.next);

        if (piece_board_collision(board, piece)) { // filled screen
            stamp_piece(board, piece);
            render_board(board, items_xy.board, piece);
            quit(&term_config, term_rows, term_cols);
        }

        stamp_piece(board, piece);

        // UPDATE PIECE
        fall_control = clock();
        while (down_is_valid(board, piece)) {
piece_falling:
            if (((double)clock() - fps_control)/CLOCKS_PER_SEC >= 1.0/FPS) {
                fps_control = clock();
                render_board(board, items_xy.board, piece);
            }
            if (((double)clock() - fall_control)/CLOCKS_PER_SEC >= piece.fall_time) {
                fall_control = clock();
                update_falling_piece(board, &piece);
            }
            if (kb_hit()) {
                process_keypress(fgetc(stdin), &term_config, term_rows, term_cols,
                                 board, &items_xy, &piece, &next_piece, &piece_held,
                                 &was_held, &hard_dropped);
                if (was_held) {
                    render_hold(piece_held, items_xy.hold);
                    render_next(next_piece, items_xy.next);
                }
            }
        }

        // wiggle room after touching ground
        fall_control = clock();
        while (!hard_dropped &&
               ((double)clock() - fall_control)/CLOCKS_PER_SEC < 1.25*piece.fall_time) {
            if (((double)clock() - fps_control)/CLOCKS_PER_SEC >= 1.0/FPS) {
                fps_control = clock();
                render_board(board, items_xy.board, piece);
            }
            if (kb_hit()) {
                process_keypress(fgetc(stdin), &term_config, term_rows, term_cols,
                                 board, &items_xy, &piece, &next_piece, &piece_held,
                                 &was_held, &hard_dropped);
                if (was_held) {
                    render_hold(piece_held, items_xy.hold);
                    render_next(next_piece, items_xy.next);
                }
            }

            if (down_is_valid(board, piece)) // is now off of a ledge
                goto piece_falling;
        }
        int rows_cleared = 0;
        clear_filled_rows(board, &rows_cleared);
        if (rows_cleared > 0) {
            update_game_state(rows_cleared, &level, &points, &fall_time);
            render_points(points, items_xy.points);
            render_level(level, items_xy.level);
        }
    }

    return 0;
}
