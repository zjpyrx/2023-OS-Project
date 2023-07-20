#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[])
{
    char* command = argv[1];  //�洢Ҫ���������ݵĲ�����ָ��ӽ��̣�
    char rec[MAXARG];
    memset(rec, 0, sizeof(rec));   //�洢��ȡ���ַ�
    int index = 0;  //����
    char ch;
    while (read(0, &ch, sizeof(char))){
        if (ch == '\n'){  //����       
            char* child_argv[4];
            rec[index] = 0;  //���һ��Ԫ������Ϊ0����ʾ�ַ�������
            //�������Ĳ��ִ����ӽ���
            child_argv[0] = command;
            child_argv[1] = argv[2];
            child_argv[2] = rec;
            child_argv[3] = 0;
            int pid = fork();  //�����ӽ���
            if (pid != 0){            
                wait(0);
            }
            else{           
                exec(child_argv[0], child_argv);  //ִ��ָ��               
            }
            index = 0;  // ���������Ա���һ�ζ�ȡ
        }
        else{
            rec[index++] = ch;
        }
    }
    exit(0);
}