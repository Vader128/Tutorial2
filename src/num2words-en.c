#include <num2words-en.h>
#include <string.h>

static const char* const ONES[] = {
  "o'clock",
  "one",
  "two",
  "three",
  "four",
  "five",
  "six",
  "seven",
  "eight",
  "nine"
};

static const char* const TEENS[] ={
  "",
  "eleven",
  "twelve",
  "thirteen",
  "fourteen",
  "fifteen",
  "sixteen",
  "seventeen",
  "eightteen",
  "nineteen"
};

static const char* const TENS[] = {
  "",
  "ten",
  "twenty",
  "thirty",
  "forty",
  "fifty",
  "sixty",
  "seventy",
  "eighty",
  "ninety"
};

static const char* const DAYS[] = {
  "sunday",
  "monday",
  "tuesday",
  "wednesday",
  "thursday",
  "friday",
  "saturday"
};

static const char* const DATES[] = {
  "",
  "first",
  "second",
  "third",
  "fourth",
  "fifth",
  "sixth",
  "seventh",
  "eighth",
  "ninth",
  "tenth",
  "eleventh",
  "twelfth",
  "thirteenth",
  "fourteenth",
  "fifteenth",
  "sixteenth",
  "seventeenth",
  "eighteenth",
  "nineteenth",
  "twentieth",
  "twenty-first",
  "twenty-second",
  "twenty-third",
  "twenty-fourth",
  "twenty-fifth",
  "twenty-sixth",
  "twenty-seventh",
  "twenty-eighth",
  "twenty-ninth",
  "thirtieth",
  "thirty-first"
};

static const char* const MONTHS[] = {
  "",
  "january",
  "february",
  "march",
  "april",
  "may",
  "june",
  "july",
  "august",
  "september",
  "october",
  "november",
  "december"
};

static size_t append_number(char* words, int num) {
  int tens_val = num / 10 % 10;
  int ones_val = num % 10;

  size_t len = 0;

  if (tens_val > 0) {
    if (tens_val == 1 && num != 10) {
      strcat(words, TEENS[ones_val]);
      return strlen(TEENS[ones_val]);
    }
    strcat(words, TENS[tens_val]);
    len += strlen(TENS[tens_val]);
    if (ones_val > 0) {
      strcat(words, " ");
      len += 1;
    }
  }

  if (ones_val > 0 || num == 0) {
    strcat(words, ONES[ones_val]);
    len += strlen(ONES[ones_val]);
  }
  return len;
}

static size_t append_string(char* buffer, const size_t length, const char* str) {
  strncat(buffer, str, length);

  size_t written = strlen(str);
  return (length > written) ? written : length;
}


void time_to_words(int hours, int minutes, char* words, size_t length) {
  size_t remaining = length;
  memset(words, 0, length);

  if (hours == 0 || hours == 12) {
    remaining -= append_string(words, remaining, TEENS[2]);
  } else {
    remaining -= append_number(words, hours % 12);
  }

  remaining -= append_string(words, remaining, " ");
  remaining -= append_number(words, minutes);
  remaining -= append_string(words, remaining, " ");
}

void time_to_3words(int hours, int minutes, char *line1, char *line2, char *line3, size_t length) {
	char value[length];
	time_to_words(hours, minutes, value, length);
	
	memset(line1, 0, length);
	memset(line2, 0, length);
	memset(line3, 0, length);
	
	char *start = value;
	char *pch = strstr (start, " ");
	while (pch != NULL) {
		if (line1[0] == 0) {
			memcpy(line1, start, pch-start);
		}  else if (line2[0] == 0) {
			memcpy(line2, start, pch-start);
		} else if (line3[0] == 0) {
			memcpy(line3, start, pch-start);
		}
		start += pch-start+1;
		pch = strstr(start, " ");
	}
	
	// Truncate long teen values
	if (strlen(line2) > 7 && strcmp(line2, "thirteen") != 0) {
		char *pch = strstr(line2, "teen");
		if (pch) {
			memcpy(line3, pch, 4);
			pch[0] = 0;
		}
	}
}

void date_to_chars(int month, int day, char *date_line, size_t length) {
  size_t remaining = length;
  memset(date_line, 0, length);

  remaining -= append_string(date_line, remaining, MONTHS[month]);

  remaining -= append_string(date_line, remaining, " ");
  remaining -= append_string(date_line, remaining, DATES[day]);
}