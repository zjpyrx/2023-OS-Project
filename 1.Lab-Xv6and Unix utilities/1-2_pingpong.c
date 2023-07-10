#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    int fd[2];//��ţ�fd[0]��ȡ���ݣ�fd[1]д������

    //����pipe
    if (pipe(fd) == -1) {
        fprintf(2, "Error: pipe(fd) error.\n");
    }
    else {
        if (fork() != 0) {  //�жϵ�ǰ�Ǹ����̣�����Ǹ����̻᷵�ؽ���id���ӽ��̷���0
            char buffer[1];
            buffer[0] = 'a';
            write(fd[1], buffer, 1);
            close(fd[1]);
            read(fd[0], buffer, 1);
            fprintf(0, "%d: received pong\n", getpid());
        }
        else {  //��ǰ���ӽ���
            char buffer[1]; //�洢�ӹܵ��ж�ȡ��д�������
            read(fd[0], buffer, 1);
            close(fd[0]);
            fprintf(0, "%d: received ping\n", getpid());
            write(fd[1], buffer, 1); // ������д��ܵ�
        }
    }
    exit(0);
}