## Tetric

Tetric is a Tetris clone, written in C, to play on the terminal. Made without
ncurses.

<img src="example.gif" width="350">

### Quick Start

```bash
$ git clone https://github.com/pedropesserl/tetric.git
$ make
```

Note: requires UTF-8 compatible terminal emulator; works best if the characters
are exactly twice as tall as they are wide. Also, play on a terminal at least
22 rows tall and 37 columns wide.

No keypress storage, hence no real fancy rotation system. Gameplay is similar
to NES Tetris'. Soft drop is instantaneous too, the only difference to hard
drop is that it doesn't lock the piece in place automatically.

Highscore is kept in a file called "highscore", in the root directory.

### Controls:
- `a` turn left
- `d` turn right
- `s` turn 180
- `f` hold
- `←` move left
- `→` move right
- `↓` soft drop
- `␣` hard drop
- `1` reset
- `!` quit

### Sources:
- https://harddrop.com/wiki/Tetris_Wiki
- https://en.wikipedia.org/wiki/ANSI_escape_code
- https://www.w3.org/TR/xml-entity-names/025.html
