/*
 * linux/kernel/keyboard.S
 *
 * (C) 1991 LInus Toravlds
 */

/*
 *   Thanks to Alfred Leung for US keyboard patches
 *     Wolfgang Thiel for German keyboard patches
 *  Marc Corsini for the French keyboard
 */

#include <linux/config.h>

.text
.globl _keyboard_interrupt

/*
 * there are for the keyboard read functions
 */
size = 1024

head =4 
