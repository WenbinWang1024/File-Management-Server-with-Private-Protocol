#include "../head/str_dealing.h"

char * pretreat_str(char * s, int len) {
    // delete abundant '\n's at the end.
    for (int i = len - 1; i >= 0; --i) {
        if ('\n' == s[i]) {
            s[i] = '\0';
            break;
        }
        if ('\0' != s[i]) {
            break;
        }
    }

    return s;
}
