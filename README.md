# 2023-OS-Project

# REPORT

## Installing



## Lab: Xv6 and Unix utilities

### Boot xv6 (easy)

1. 实验目的：

   获取源代码并构建、运行、启动xv6。

2. 实验步骤：
   1. 获取实验室的 xv6 源代码并查看 `util` 分支。运行`$ git clone git://g.csail.mit.edu/xv6-labs-2021`获取代码到本地，并`$ cd xv6-labs-2021`、`$ git checkout util`来查看util分支。
   2. 构建并运行xv6。运行`$ make qemu`。后输入ls运行一个初始文件系统的程序，来查看是否成功构建了xv6。
   3. 退出qemu，可用`Ctrl-a x`。
   
3. 实验中遇到的问题与解决方法：暂无
4. 实验心得：学会了如何启动运行xv6系统，往后的操作都在cd进xv6-labs-2021目录后进行

### sleep (easy)

1. 实验目的：编写程序实现程序“睡眠”，使系统暂停一段时间刻度，这段时间刻度由用户给出。如输入sleep 10指令，系统暂停10个时间刻度。如果用户忘记传递参数，睡眠应打印错误消息。

2. 实验步骤：

  1. 编写程序。用程序的sleep来实现停顿。程序名为sleep.c

  2. 将sleep.c保存在user文件夹中，并添加sleep到Makefile中的UPROGS下面。（添加一句$U/_sleep\）

  3. 放入后make qemu编译，输入sleep 10，系统暂停了一段时间。![image-20230710225229369](assets/image-20230710225229369.png)

  4. 测试程序。在主系统运行make grade，输入，运行结果：

     ![image-20230710224443949](assets/image-20230710224443949.png)

     说明程序运行正确。

3. 实验中遇到的问题与解决方法：

   1. 编写好程序、放入user后仍无法运行，原因是没有make qemu重新编译源代码并生成执行文件，重新编译即可正确运行sleep程序

   2. 测试运行时无法运行指令，需要运行sudo chmod +x ./grade-lab-util，来赋予grade-lab-util可执行权限。

4. 实验心得：

   1. 头文件：

      1. `#include "kernel/types.h"`: 包含 types.h 头文件，其中定义了一些 xv6 内核中使用的基本数据类型，如 uint、uchar、ushort 等。

      2. `#include "kernel/stat.h"`: 包含 stat.h 头文件，其中定义了用于文件和文件系统的状态信息结构 stat，以及一些与文件操作相关的常量和函数原型。

      3. `#include "user/user.h"`：包含 user.h 头文件，其中定义了一些用户空间程序常用的函数原型，如 fork()、exec()、exit() 等。

   2. `int main(int argc, char* argv[])`：main函数接受两个参数，整数argc表示命令行参数数量，字符指针数组 argv表示命令行参数列表。为了防止调用错误要检查参数数量是否合规（此处为2）。

   3. 输出错误时用到了`fprintf(2, "Usage: sleep times\n")`，2是一个文件描述符，代表标准错误流（stderr），后面是需要输出的错误信息。

   4. `exit(0)`用于退出程序，而不是写return 0。

      

### pingpong (easy)

1. 实验目的：模拟管道的用法。编写程序来“pingpong” 一对管道上的两个进程之间的字节，父进程向子进程发送一个字节; 子进程打印“<pid>：received ping”，<pid> 是其进程 ID， 将管道上的字节写给父进程、退出; 父进程应从子进程读取字节， 打印“<pid>：received pong”、并退出。
2. 实验步骤：
   1. 编写程序。用pipe来创建管道，fork来创建子进程，read(src,dst,lenth)来读，write(dst,src,lenth)来写。getpid来获取进程id。
   2. 编译。运行结果：![image-20230710235357561](assets/image-20230710235357561.png)
   3. 运行测试程序：![image-20230710234102423](assets/image-20230710234102423.png)
3. 实验中遇到的问题与解决方法：暂无
4. 实验心得：学会了如何使用管道，以及它是如何在进程间通信、传递信息的。在程序编写时要注意程序读写的前后顺序，以免出现错误值。

### primes (moderate)/(hard)

### find (moderate)

### xargs (moderate)

## Lab: system calls

### System call tracing (moderate)

### Sysinfo (moderate)

## Lab: page tables

### Speed up system calls (easy)

### Print a page table (easy)

### Detecting which pages have been accessed (hard)

## Lab: traps

### RISC-V assembly (easy)

### Backtrace (moderate)

### Alarm (hard)

## Lab: Copy-on-Write Fork for xv6

### Implement copy-on write(hard)

## Lab: Multithreading

### Uthread: switching between threads (moderate)

### Using threads (moderate)

### Barrier(moderate)

## Lab: networking

## Lab: locks

### Memory allocator (moderate)

### Buffer cache (hard)

## Lab: file system

### Large files (moderate)

### Symbolic links (moderate)

## Lab: mmap (hard)