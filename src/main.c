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
    int boardx, boardy, holdx, holdy, nextx, nexty;
    
    initialize_screen(term_rows, term_cols, &boardx, &boardy,
                      &holdx, &holdy, &nextx, &nexty);

    Piece piece = {0}, piece_held = {0}, next_piece = new_piece(rand() % 7 + 1, 0.8);

    for (;;) {
        int hard_dropped = 0, was_held = 0;

        fall_control = clock();
        fps_control = clock();

        piece = next_piece;
        next_piece = new_piece(rand() % 7 + 1, 0.8);
        render_next(next_piece, nextx, nexty);

        stamp_piece(board, piece);

        if (!down_is_valid(board, piece)) // filled screen
            quit(&term_config, term_rows, term_cols);

        // UPDATE PIECE
        fall_control = clock();
        while (down_is_valid(board, piece)) {
piece_falling:
            if (((double)clock() - fps_control)/CLOCKS_PER_SEC >= 1.0/FPS) {
                fps_control = clock();
                render_board(board, boardx, boardy);
            }
            if (((double)clock() - fall_control)/CLOCKS_PER_SEC >= piece.fall_time) {
                fall_control = clock();
                update_falling_piece(board, &piece);
            }
            if (kb_hit()) {
                process_keypress(fgetc(stdin), &term_config, term_rows, term_cols,
                                 board, &piece, &next_piece, &piece_held,
                                 &was_held, &hard_dropped);
                if (was_held) {
                    render_hold(piece_held, holdx, holdy);
                    render_next(next_piece, nextx, nexty);
                }
            }
        }

        // wiggle room after touching ground
        fall_control = clock();
        while (!hard_dropped &&
               ((double)clock() - fall_control)/CLOCKS_PER_SEC < 1.25*piece.fall_time) {
            if (((double)clock() - fps_control)/CLOCKS_PER_SEC >= 1.0/FPS) {
                fps_control = clock();
                render_board(board, boardx, boardy);
            }
            if (kb_hit()) {
                process_keypress(fgetc(stdin), &term_config, term_rows, term_cols,
                                 board, &piece, &next_piece, &piece_held,
                                 &was_held, &hard_dropped);
                if (was_held) {
                    render_hold(piece_held, holdx, holdy);
                    render_next(next_piece, nextx, nexty);
                }
            }

            if (down_is_valid(board, piece)) // is now off of a ledge
                goto piece_falling;
        }
    }

    return 0;
}
