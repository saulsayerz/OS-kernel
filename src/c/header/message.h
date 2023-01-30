#include "std_type.h"

struct message {
    byte current_directory;
    char arg1[64];
    char arg2[64];
    char arg3[64];
    int next_program_segment;
    byte other[317];
};
