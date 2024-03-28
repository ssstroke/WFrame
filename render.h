#pragma once

/*
Write RGB color to (x, y) coordinate of imaginary plane of
32-bit pixels that the `buffer` points to.
It is assumed that `r`, `g` and `b` are in [0, 255] range.
*/
void WriteColor(void *buffer, int x, int y, int r, int g, int b);
