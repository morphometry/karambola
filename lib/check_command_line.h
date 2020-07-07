#ifndef CHECK_COMMAND_LINE
#define CHECK_COMMAND_LINE

#include "CalcOptions.h"


void check_command_line(CalcOptions *CO, int argc, char *argv[]);


void check_command_line_bilimbi(CalcOptions *CO, int argc, char *argv[]);

inline
bool is_off_file(const std::string &filename)
{
    return filename.rfind(".off") == filename.size() - 4 || filename.rfind(".OFF") == filename.size() - 4;
}

#endif // CHECK_COMMAND_LINE
