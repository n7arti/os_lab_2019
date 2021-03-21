#include "revert_string.h"
#include <string.h>
#include "swap.h"
void RevertString(char *str) {
    int len = strlen(str);
    for(int i = 0; i < len/2; i++)
        Swap(&str[i], &str[len-i-1]);
}