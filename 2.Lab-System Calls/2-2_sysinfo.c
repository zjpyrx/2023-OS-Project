//kernel / sysproc.c
uint64
sys_sysinfo(void)
{
    struct sysinfo info;    // addr代表的是userspace的内存地址，info处于kernel中，需要复制到user中
    uint64 addr;
    if (argaddr(0, &addr) < 0)  //argaddr函数用于获取地址,此处将索引为0的参数的地址保存在addr变量中。
        return -1;
    info.nproc = proc_getnum();  //获取当前进程数，将结果保存在info.nproc中。
    info.freemem = getfreememSize();  //获取系统的空闲可用内存大小
    if (copyout(myproc()->pagetable, addr, (char*)&info, sizeof(info)) < 0) {  //copyout，将info结构体从kernel复制到user空间的地址addr处。
        return -1;
    }
    else {
        return 0;
    }
    return 0;
}



// kernel / sysinfo.h
struct sysinfo {
	uint64 freemem;   // amount of free memory (bytes) == 遍历freeelist==>得到可用内存大小
	uint64 nproc;     // number of process  == myproc()->nextpid
};

//kernel/kalloc.c
int
getfreememSize()
{
    struct run* r;
    int memSize = 0;
    acquire(&kmem.lock);//获取锁
    for (r = kmem.freelist; r != 0; r = r->next)
    {
        memSize += PGSIZE;
    }
    release(&kmem.lock);  //释放锁
    return memSize;
}

//kernel/proc.c
int
proc_getnum(void) {
    struct proc* p;
    uint64 count = 0;
    for (p = proc; p < &proc[NPROC]; p++) {  //遍历所有进程
        acquire(&p->lock);  //获取锁
        if (p->state != UNUSED) {
            count++;
        }
        release(&p->lock);  //释放锁
    }
    return count;
}



