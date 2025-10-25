/**
@file driver_uart.c
@brief 串口重定位与 stdio 相关的底层实现（用于 printf/scanf 重定向）
@note 仅包含应用层需要覆盖的 `_read`/`_write` 与 RetargetInit 等接口，避免与系统 `syscalls.c` 的实现重复定义
@author sleet
@date 2025/9/30
*/

#include "main.h"
#include "driver_uart.h"
#include <_ansi.h>

#include <_syslist.h>

#include <errno.h>

#include <sys/time.h>

#include <sys/times.h>


#include <stdint.h>

#if !defined(OS_USE_SEMIHOSTING)

#define STDIN_FILENO 0

#define STDOUT_FILENO 1

#define STDERR_FILENO 2



UART_HandleTypeDef *gHuart;



/**
 * @brief 将标准 I/O 重定向到指定的 UART
 * @param huart 指向要用于 printf/scanf 的 UART 句柄
 * @note 该函数会同时设置 stdout 为无缓冲模式
 */
void RetargetInit(UART_HandleTypeDef *huart)

{

    gHuart = huart;

    /* Disable I/O buffering for STDOUT stream, so that
* chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);

}


/**
 * @brief write 接口重定向（被 libc 调用）
 * @param fd 文件描述符（只处理 STDOUT/STDERR）
 * @param ptr 要发送的数据指针
 * @param len 数据长度
 * @return 成功返回写入的字节数，失败返回负值
 */
int _write(int fd, char *ptr, int len)

{

 HAL_StatusTypeDef hstatus;

    if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
     {
         hstatus = HAL_UART_Transmit(gHuart, (uint8_t *) ptr, len, HAL_MAX_DELAY);
        if (hstatus == HAL_OK)
         return len;
         else
         return EIO;
         }
     errno = EBADF;
     return -1;

}


/**
 * @brief read 接口重定向（被 libc 调用）
 * @param fd 文件描述符（只处理 STDIN）
 * @param ptr 接收缓冲区指针
 * @param len 最大接收长度
 * @return 成功返回实际读取的字节数，失败返回负值
 */
int _read(int fd, char *ptr, int len)

{
    HAL_StatusTypeDef hstatus;
    if (fd == STDIN_FILENO)
     {
         hstatus = HAL_UART_Receive(gHuart, (uint8_t *) ptr, 1, HAL_MAX_DELAY);
         if (hstatus == HAL_OK)
         return 1;
         else
         return EIO;
         }
     errno = EBADF;
     return -1;

}



#endif //#if !defined(OS_USE_SEMIHOSTING)