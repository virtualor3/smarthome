/**
 * @file pwm.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

//不要用注册file_operation
/**
 * @brief: 不要使用module_platform_driver
 * @example: 如果使用probe，可以用下面的形式
 * void xxx_init(void)
 * {
 *      platform_register_driver(...);
 * }
 */
//只要实现smartdriver.h里的功能函数就行
//头文件可以自行添加，自行添加的头文件是pwm.c的私有头文件
//所有全局变量请放到自行添加的头文件中