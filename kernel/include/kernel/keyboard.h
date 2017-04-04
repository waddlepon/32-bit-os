#ifndef KEYBOARD_H
#define KEYBOARD_H

#define ESC 27

#define F1 0x80
#define F2 (F1 + 1)
#define F3 (F2 + 1)
#define F4 (F3 + 1)
#define F5 (F4 + 1)
#define F6 (F5 + 1)
#define F7 (F6 + 1)
#define F8 (F7 + 1)
#define F9 (F8 + 1)
#define F10 (F9 + 1)
#define F11 (F10 + 1)
#define F12 (F11 + 1)

#define INS 0x90
#define DEL (INS + 1)
#define HOME (DEL + 1)
#define END (HOME + 1)
#define PGUP (END + 1)
#define PGDN (PGUP + 1)
#define LEFT (PGDN + 1)
#define UP (LEFT + 1)
#define DOWN (UP + 1)
#define RIGHT (DOWN + 1)

#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36

void keyboard_init();

#endif
