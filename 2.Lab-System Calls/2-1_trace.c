/******����Ϊtrace.c������******/
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

    if (trace(atoi(argv[1])) < 0) { //trace��������ֵС��0����ʾ��������ʧ��
        fprintf(2, "%s: trace failed\n", argv[0]);
        exit(1);
    }

    for (i = 2; i < argc && i < MAXARG; i++) {
        child_argv[i - 2] = argv[i];
    }
    exec(child_argv[0], child_argv);  //ִ��
    exit(0);
}


/******���������޸�ϵͳ�����ļ�ʱ�����һЩ����

//1.kernel/sysproc.c���sys_trace������
uint64
sys_trace(void)
{
    int mask;
    argint(0, &mask);  //argint������ȡϵͳ���ú��������Ͳ���
    if (mask < 0)  //���mask��ֵ�Ƿ�С��0,С��0��ʾ���������ֵ��Ч
        return -1;
    myproc()->mask = mask;  //myproc()�������ڻ�ȡ��ǰ���̵�ָ�롣��maskֵ������ǰ���̵�mask��Ա����
    return 0;
}


//2.kernel/syscall.c�޸�syscall����
void
syscall(void)
{
    int num;
    struct proc* p = myproc();

    num = p->trapframe->a7;
    if (num > 0 && num < NELEM(syscalls) && syscalls[num]) {
        p->trapframe->a0 = syscalls[num]();

        //��鵱ǰ���̵������Ƿ������˶�Ӧϵͳ���õ�trace����������ˣ������ͻ��ӡ��Ϣ
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