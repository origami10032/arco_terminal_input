/*
 * author: arco
 * date  : 2023.6.24
 * 使用方法:
 * 0. 调用arco_terminal_init初始化模拟终端(可缺省
 * 1. 调用arco_terminal_add_search_str函数将目标字符串添加到查找表
 * 2. arco_terminal_set_input_perfix设置控制台打印前缀(可不选)
 * 3. 调用arco_terminal_input当做scanf函数接收输入
 * 4. 结束时调用arco_terminal_destory恢复终端
 */

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "arco_terminal_input.h"

#define NOT_MATCH    -1
#define PERFIX_MATCH  0

/*
 * 链表定义区start
 */

typedef struct SearchListNode {
    char* str;
    struct SearchListNode* before;
    struct SearchListNode* next;
} SearchListNode;

typedef struct SearchList {
    SearchListNode* head;
    SearchListNode* end;
    int length;
} SearchList;

SearchListNode* init_search_list_node(const char* src_str)
{
    SearchListNode* node;
    node = (SearchListNode*) malloc(sizeof(SearchListNode*));
    node->before = (SearchListNode*) malloc(sizeof(SearchListNode));
    node->next = (SearchListNode*) malloc(sizeof(SearchListNode));
    if (src_str != NULL) {
        node->str = (char*) malloc((strlen(src_str) + 1) * sizeof(char));
        // strcpy函数自动添加\0
        strcpy(node->str, src_str);
    }
    else {
        node->str = NULL;
    }
    return node;
}

SearchList* init_search_list()
{
    SearchList* list = (SearchList*) malloc(sizeof(SearchList));
    list->head = (SearchListNode*) malloc(sizeof(SearchListNode*));
    list->end = (SearchListNode*) malloc(sizeof(SearchListNode*));
    list->head->before = NULL;
    list->head->next = list->end;
    list->end->next = NULL;
    list->end->before = list->head;

    list->length = 0;

    return list;
}

void search_list_add_node(SearchList* list, SearchListNode* list_node)
{
    SearchListNode* p = list->end->before;
    p->next = list_node;
    list_node->before = p;
    list->end->before = list_node;
    list_node->next = list->end;
    list->length++;
}

void search_list_add_str(SearchList* list, const char* src_str)
{
    SearchListNode* node = init_search_list_node(src_str);
    search_list_add_node(list, node);
}

void print_search_list(SearchList* list)
{
    SearchListNode* p = list->head->next;
    while (p->next != NULL) {
        printf("%s\t", p->str);
        p = p->next;
    }
}

/*
 * 链表定义区end
 */



/*
 * 匹配函数定义区start
 */

int g_arco_terminal_init_flag = 0;
char g_console_perfix[16];
SearchList* g_search_scope;

int str_match(const char* src_str, char* dst_str)
{
    int i, ret = PERFIX_MATCH;
    // 异常情况校验
    if (src_str == NULL || dst_str == NULL) return NOT_MATCH;
    if (strlen(src_str) > strlen(dst_str)) return NOT_MATCH;
    if (strlen(dst_str) == 0) return NOT_MATCH;
    // 按字符匹配
    for (i = 0; i < strlen(src_str); i++) {
        if (src_str[i] != dst_str[i]) {
            return NOT_MATCH;
        }
    }

    return ret;
}

int get_match_str_list(SearchList* list, SearchList* match_str_list, const char* src_str)
{
    int ret, match_num = 0;
    SearchListNode* p = list->head->next;
    while (p->next != NULL) {
        ret = str_match(src_str, p->str);
        if (ret == PERFIX_MATCH) {
            search_list_add_str(match_str_list, p->str);
            match_num++;
        }
        p = p->next;
    }
    return match_num;
}

int tap_search(char* arco_buffer, char *search_result, char pre_c)
{
    int match_num = 0;
    // 用链表替换二维数组
    SearchList* match_str_list = init_search_list();
    // 查找链表中所有匹配结果
    match_num = get_match_str_list(g_search_scope, match_str_list, arco_buffer);

    if (match_num == 0) return 0;
    else if (match_num == 1) {
        strcpy(search_result, match_str_list->head->next->str);
    }
    else {
        if (pre_c == '\t') {
            printf("\n");
            print_search_list(match_str_list);
            printf("\n");
        }
        // free先不写
    }

    return match_num;
}

/*
 * 匹配函数定义区end
 */



/*
 * 模拟终端输入工具区start
 */

unsigned int origin_c_lflag;
unsigned char origin_ccVMIN;
unsigned char origin_ccVTIME;

// 初始化模拟终端、查找表、控制台前缀
void arco_terminal_init()
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    // 禁用标准输入模式和显示输入字符
    term.c_lflag &= ~(ICANON | ECHO);
    term.c_cc[VMIN] = 1;
    term.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    // 刷新缓冲区强制输出
    fflush(stdout);
    // 初始化控制台前缀
    memset(g_console_perfix, '\0', sizeof(g_console_perfix));
    // 初始化搜索范围链表
    g_search_scope = init_search_list();

    g_arco_terminal_init_flag = 1;
}

// 结束时还原终端设置终端
void arco_terminal_destroy()
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    // 恢复标准输入模式和显示输入字符
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    g_arco_terminal_init_flag = 0;
}

// 设置控制台输入前缀, 不设置即没有
int arco_terminal_set_input_perfix(char* console_perfix)
{
    if (strlen(console_perfix) > sizeof(g_console_perfix)) {
        printf("target console perfix too long!\n");
        return -1;
    }
    memset(g_console_perfix, '\0', sizeof(g_console_perfix));
    strcpy(g_console_perfix, console_perfix);
}

// 添加字符串到查找表
int arco_terminal_add_search_str(char* src_str)
{
    if (g_arco_terminal_init_flag != 1) {
        arco_terminal_init();
    }
    search_list_add_str(g_search_scope, src_str);
}

// 模拟终端输入函数
int arco_terminal_input(char* ipt_str)
{
    if (g_arco_terminal_init_flag != 1) {
        arco_terminal_init();
    }

    int i = 0, j;
    char c, pre_c = '\0', arco_buffer[32];

    memset(arco_buffer, '\0', sizeof(arco_buffer));
    printf("%s", g_console_perfix);
    while (1) {
        c = getchar();
        // Backspace: 退一格、用空格清除一个字符、再退一格, 指针减1
        if (c == '\b') {
            printf("\b \b");
            arco_buffer[i - 1] = '\0';
            i--;
        }
        else if (c == '\n') {
            printf("\n");
            break;
        }
            /*
             * tap键有3个逻辑
             * 1.匹配到唯一结果:直接打印
             * 2.匹配到多结果:第二次及以后敲时打印
             * 3.匹配不到:不做处理
             */
        else if (c == '\t') {
            int match_num;
            char search_result[16];
            memset(search_result, '\0', sizeof(search_result));
            match_num = tap_search(arco_buffer, search_result, pre_c);
            // 2的处理交给tap_search函数, 3不做处理, 因此这里仅处理1
            // 1的处理
            if (match_num == 1) {
                for (j = 0; j < i; j++) {
                    printf("\b \b");
                    fflush(stdout);
                }
                strcpy(arco_buffer, search_result);
                i = strlen(search_result);
                printf("%s", arco_buffer);
            }
                // 情况2的处理:切换到下一次输入
            else if (match_num > 1) {
                // 多匹配第2次敲tab, tap_search处理之后切换到下一次输入
                if (pre_c == '\t') {
                    // 先打印控制台前缀
                    printf("%s", g_console_perfix);
                    // 打印arco缓冲区变量
                    printf("%s", arco_buffer);
                }
            }
        }
        else {
            printf("%c", c);
            arco_buffer[i] = c;
            i++;
        }
        pre_c = c;
    }
    printf("%s\n", arco_buffer);
    strcpy(ipt_str, arco_buffer);
}

/*
 * 模拟终端输入工具区end
 */