# arco_terminal_input
linux环境上模拟终端输入的c语言程序

作者  : arco  

日期  : 2023.6.24  

# 使用方式
0. 调用arco_terminal_init初始化模拟终端(非必须,执行模拟终端输入函数时会自动初始化

1. 调用arco_terminal_add_search_str函数将目标字符串添加到查找表(非必须)  

2. arco_terminal_set_input_perfix设置控制台打印前缀(非必须)  

3. 调用arco_terminal_input当做scanf函数接收输入(主要功能)  

4. 结束时调用arco_terminal_destory恢复终端(必须)  
