//kernel / sysproc.c
uint64
sys_sysinfo(void)
{
    struct sysinfo info;    // addr�������userspace���ڴ��ַ��info����kernel�У���Ҫ���Ƶ�user��
    uint64 addr;
    if (argaddr(0, &addr) < 0)  //argaddr�������ڻ�ȡ��ַ,�˴�������Ϊ0�Ĳ����ĵ�ַ������addr�����С�
        return -1;
    info.nproc = proc_getnum();  //��ȡ��ǰ�������������������info.nproc�С�
    info.freemem = getfreememSize();  //��ȡϵͳ�Ŀ��п����ڴ��С
    if (copyout(myproc()->pagetable, addr, (char*)&info, sizeof(info)) < 0) {  //copyout����info�ṹ���kernel���Ƶ�user�ռ�ĵ�ַaddr����
        return -1;
    }
    else {
        return 0;
    }
    return 0;
}



// kernel / sysinfo.h
struct sysinfo {
	uint64 freemem;   // amount of free memory (bytes) == ����freeelist==>�õ������ڴ��С
	uint64 nproc;     // number of process  == myproc()->nextpid
};

//kernel/kalloc.c
int
getfreememSize()
{
    struct run* r;
    int memSize = 0;
    acquire(&kmem.lock);//��ȡ��
    for (r = kmem.freelist; r != 0; r = r->next)
    {
        memSize += PGSIZE;
    }
    release(&kmem.lock);  //�ͷ���
    return memSize;
}

//kernel/proc.c
int
proc_getnum(void) {
    struct proc* p;
    uint64 count = 0;
    for (p = proc; p < &proc[NPROC]; p++) {  //�������н���
        acquire(&p->lock);  //��ȡ��
        if (p->state != UNUSED) {
            count++;
        }
        release(&p->lock);  //�ͷ���
    }
    return count;
}



