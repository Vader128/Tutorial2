#pragma once
#include <string.h>

void time_to_words(int hours, int minutes, char* words, size_t length);
void time_to_3words(int hours, int minutes, char *line1, char *line2, char *line3, size_t length);
void date_to_chars(int month, int day, char *date_line, size_t length);