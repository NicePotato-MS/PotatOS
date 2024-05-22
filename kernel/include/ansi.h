#pragma once

#define ANSI_PARSE_MODE_ESC 0 // Search for escape character
#define ANSI_PARSE_MODE_INTRODUCER 1  // Search for introducer (usually '[')
#define ANSI_PARSE_MODE_CSI_COMMAND 2 // Absorb commands


extern const char ANSI_RESET[];
extern const char ANSI_BOLD[];
extern const char ANSI_DIM[];
extern const char ANSI_UNDERLINE[];
extern const char ANSI_BLINK[];
extern const char ANSI_REVERSE[];
extern const char ANSI_HIDDEN[];

extern const char ANSI_BLACK[];
extern const char ANSI_RED[];
extern const char ANSI_GREEN[];
extern const char ANSI_YELLOW[];
extern const char ANSI_BLUE[];
extern const char ANSI_MAGENTA[];
extern const char ANSI_CYAN[];
extern const char ANSI_WHITE[];

extern const char ANSI_BRIGHT_BLACK[];
extern const char ANSI_BRIGHT_RED[];
extern const char ANSI_BRIGHT_GREEN[];
extern const char ANSI_BRIGHT_YELLOW[];
extern const char ANSI_BRIGHT_BLUE[];
extern const char ANSI_BRIGHT_MAGENTA[];
extern const char ANSI_BRIGHT_CYAN[];
extern const char ANSI_BRIGHT_WHITE[];

extern const char ANSI_BG_BLACK[];
extern const char ANSI_BG_RED[];
extern const char ANSI_BG_GREEN[];
extern const char ANSI_BG_YELLOW[];
extern const char ANSI_BG_BLUE[];
extern const char ANSI_BG_MAGENTA[];
extern const char ANSI_BG_CYAN[];
extern const char ANSI_BG_WHITE[];

extern const char ANSI_BG_BRIGHT_BLACK[];
extern const char ANSI_BG_BRIGHT_RED[];
extern const char ANSI_BG_BRIGHT_GREEN[];
extern const char ANSI_BG_BRIGHT_YELLOW[];
extern const char ANSI_BG_BRIGHT_BLUE[];
extern const char ANSI_BG_BRIGHT_MAGENTA[];
extern const char ANSI_BG_BRIGHT_CYAN[];
extern const char ANSI_BG_BRIGHT_WHITE[];