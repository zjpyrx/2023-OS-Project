#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//��������fmtname�����ڸ�ʽ���ļ��������Ὣ�ļ���������һ���̶���С�Ļ������У������ļ������Ȳ���ʱ���пո���䣬ȷ��������ļ�������һ�¡�
char*
fmtname(char* path)
{
    static char buf[DIRSIZ + 1];
    char* p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

char* remove_space(char* str) {
    int len = strlen(str);
    while (len > 0 && str[len - 1] == ' ') {
        str[--len] = '\0';
    }
    return str;
}



void find(char* path, char* name) {
    char buf[512], * p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
    case T_FILE:  //������ļ�
        if (strcmp(name, fmtname(path)) == 0) {
            printf("%s\n", path);
        }
        break;

    case T_DIR:  //�����Ŀ¼
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("find: path too long\n");
            break;
        }
        //�������Ŀ¼
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
                      
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0) {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            //Ϊ�˲�������ǰ���ϼ�Ŀ¼�����ܺ��пո�����Ҫȥ��
            if (strcmp(remove_space(de.name), ".") == 0 || strcmp(remove_space(de.name), "..") == 0)
                continue;
            if (st.type == T_DEVICE || st.type == T_FILE) {
                if (strcmp(name, remove_space(de.name)) == 0) {
                    printf("%s\n", buf);
                }
            }
            else if (st.type == T_DIR) {  //�ݹ���ã������Ŀ¼
                find(buf, name);
            }
        }
        
        break;
    }
    close(fd);
}


int
main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(2, "usage: find <path> <target>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}