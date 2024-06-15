#ifndef INPUT_TEST_ARCO_TERMINAL_INPUT_H
#define INPUT_TEST_ARCO_TERMINAL_INPUT_H

/**
 * function: arco_terminal_init
 * 功能: 初始化模拟终端
 * 说明: 可不调用, 执行时会自动初始化
 */
void arco_terminal_init();

/**
 * funtion: arco_terminal_destroy
 * 功能: 恢复终端原始状态
 * 说明: 如果不调用可能会导致程序结束后终端输入字符不回显
 */
void arco_terminal_destroy();

/**
 * funtion: arco_terminal_set_input_perfix
 * 功能: 设置控制台输入前缀, 比如>>这样的 (菲必须
 */
int arco_terminal_set_input_perfix(char* console_perfix);

/**
 * funtion: arco_terminal_add_search_str
 * 功能: 注册搜索字符串
 * 说明: 按tap键可以尝试补全这些被注册的字符串 (菲必须
 */
int arco_terminal_add_search_str(char* src_str);

/**
 * funtion: arco_terminal_input
 * 功能: 模拟终端的输入函数
 * 说明: 调用此函数来完成模拟终端一样的输入
 *      如果不满意这个名字可以给她套个壳
 */
int arco_terminal_input(char* ipt_str);


#endif //INPUT_TEST_ARCO_TERMINAL_INPUT_H
