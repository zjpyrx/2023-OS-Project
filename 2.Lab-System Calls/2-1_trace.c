/******以下为trace.c中内容******/
#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    int i;
    char* child_argv[MAXARG];

    if (argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')) {
        fprintf(2, "Usage: %s <mask> <command>\n", argv[0]);
        exit(1);
    }

    if (trace(atoi(argv[1])) < 0) { //trace函数返回值小于0，表示跟踪设置失败
        fprintf(2, "%s: trace failed\n", argv[0]);
        exit(1);
    }

    for (i = 2; i < argc && i < MAXARG; i++) {
        child_argv[i - 2] = argv[i];
    }
    exec(child_argv[0], child_argv);  //执行
    exit(0);
}


/******以下是在修改系统调用文件时加入的一些函数

//1.kernel/sysproc.c添加sys_trace函数。
uint64
sys_trace(void)
{
    int mask;
    argint(0, &mask);  //argint函数获取系统调用函数的整型参数
    if (mask < 0)  //检查mask的值是否小于0,小于0表示传入的掩码值无效
        return -1;
    myproc()->mask = mask;  //myproc()函数用于获取当前进程的指针。将mask值赋给当前进程的mask成员变量
    return 0;
}


//2.kernel/syscall.c修改syscall函数
void
syscall(void)
{
    int num;
    struct proc* p = myproc();

    num = p->trapframe->a7;
    if (num > 0 && num < NELEM(syscalls) && syscalls[num]) {
        p->trapframe->a0 = syscalls[num]();

        //检查当前进程的掩码是否设置了对应系统调用的trace。如果设置了，函数就会打印信息
        if ((p->mask & (1 << num)) != 0) {
            char* syscall_name = syscall_names[num];
            printf("pid=%d syscallname=%s -> %d\n", p->pid, syscall_name, p->trapframe->a0);
        }
    }
    else {
        printf("%d %s: unknown sys call %d\n",
            p->pid, p->name, num);
        p->trapframe->a0 = -1;
    }
}
******/