#include <stdio.h>
#include <string.h>
#include "arco_terminal_input.h"

char g_str_list[][16] = {
        "asdfas",
        "qwerqw",
        "str123456",
        "str123555",
        "str122222",
        "sssssaa"
};

int main()
{
    arco_terminal_add_search_str("asdfasdf");
    arco_terminal_add_search_str("qwerqwe");
    arco_terminal_add_search_str("str1234567");
    arco_terminal_add_search_str("str1234");
    arco_terminal_add_search_str("str1233");
    arco_terminal_add_search_str("str1232");
    arco_terminal_add_search_str("arcodesuki");

    arco_terminal_set_input_perfix(">> ");

    char ipt_str[16];
    memset(ipt_str, '\0', sizeof(ipt_str));
    while (1) {
        arco_terminal_input(ipt_str);
        printf("get input str:%s\n", ipt_str);
        if (strcmp(ipt_str, "quit") == 0) break;
    }

    arco_terminal_destroy();

    return 0;
}