#pragma once

#define ANSI_PARSE_MODE_ESC 0 // Search for escape character
#define ANSI_PARSE_MODE_INTRODUCER 1  // Search for introducer (usually '[')
#define ANSI_PARSE_MODE_CSI_COMMAND 2 // Absorb commands

namespace ansi {
    extern const char reset[];
    extern const char bold[];
    extern const char dim[];
    extern const char underline[];
    extern const char blink[];
    extern const char reverse[];
    extern const char hidden[];

    namespace fg {
        extern const char black[];
        extern const char red[];
        extern const char green[];
        extern const char yellow[];
        extern const char blue[];
        extern const char magenta[];
        extern const char cyan[];
        extern const char white[];

        extern const char bright_black[];
        extern const char bright_red[];
        extern const char bright_green[];
        extern const char bright_yellow[];
        extern const char bright_blue[];
        extern const char bright_magenta[];
        extern const char bright_cyan[];
        extern const char bright_white[];
    }

    namespace bg {
        extern const char black[];
        extern const char red[];
        extern const char green[];
        extern const char yellow[];
        extern const char blue[];
        extern const char magenta[];
        extern const char cyan[];
        extern const char white[];

        extern const char bright_black[];
        extern const char bright_red[];
        extern const char bright_green[];
        extern const char bright_yellow[];
        extern const char bright_blue[];
        extern const char bright_magenta[];
        extern const char bright_cyan[];
        extern const char bright_white[];
    }
}
