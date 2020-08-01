#include "../head/str_dealing.h"

char * pretreat_str(char * s, int len) {
    // pretreat c_string s with a non-blank character as the initial
    // and delete abundant '\n's at the end of s.
    int pos = 0;   
    for (pos = 0; pos < len; ++pos) {
        if (' ' != s[pos]) {
            break;
        }
    }

    if ('\n' != s[len - 1] || 0 != s[len - 1]) {
        return s + pos;
    }
    
    for (int i = len - 1; i >= pos; --i) {
        if ('\n' != s[i]) {
            s[i] = 0;
        }
        else {
            s[i] = 0;
            break;
        }
    }

    return s + pos;
}
