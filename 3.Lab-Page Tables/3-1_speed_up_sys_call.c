//kernel/proc.c
// �ڴ��ļ����ֺ������������䣬��ʵ�ֽ�����������
//����ӳ��,����˲���
pagetable_t
proc_pagetable(struct proc* p)
{
    pagetable_t pagetable;

    // An empty page table.
    pagetable = uvmcreate();
    if (pagetable == 0)
        return 0;

    // map the trampoline code (for system call return)
    // at the highest user virtual address.
    // only the supervisor uses it, on the way
    // to/from user space, so not PTE_U.
    if (mappages(pagetable, TRAMPOLINE, PGSIZE,
        (uint64)trampoline, PTE_R | PTE_X) < 0) {
        uvmfree(pagetable, 0);
        return 0;
    }

    // map the trapframe just below TRAMPOLINE, for trampoline.S.
    if (mappages(pagetable, TRAPFRAME, PGSIZE,
        (uint64)(p->trapframe), PTE_R | PTE_W) < 0) {
        uvmunmap(pagetable, TRAMPOLINE, 1, 0);
        uvmfree(pagetable, 0);
        return 0;
    }

    /*********************��ӵĲ���*****************************/
    p->usyscall->pid = p->pid;
    if (mappages(pagetable, USYSCALL, PGSIZE, (uint64)p->usyscall, PTE_R | PTE_U) < 0) {
        uvmunmap(pagetable, USYSCALL, 1, 0);
        uvmfree(pagetable, 0);
    }
    /*************************************************************/
    return pagetable;
}

//����ͳ�ʼ��ҳ��
static struct proc*
allocproc(void)
{
    struct proc* p;

    for (p = proc; p < &proc[NPROC]; p++) {
        acquire(&p->lock);
        if (p->state == UNUSED) {
            goto found;
        }
        else {
            release(&p->lock);
        }
    }
    return 0;

found:
    p->pid = allocpid();
    p->state = USED;

    // Allocate a trapframe page.
    if ((p->trapframe = (struct trapframe*)kalloc()) == 0) {
        freeproc(p);
        release(&p->lock);
        return 0;
    }

    /*********************��ӵĲ���*****************************/
    if ((p->usyscall = (struct usyscall*)kalloc()) == 0) {
        freeproc(p);
        release(&p->lock);
        return 0;
    }
    /*************************************************************/

    // An empty user page table.
    p->pagetable = proc_pagetable(p);
    if (p->pagetable == 0) {
        freeproc(p);
        release(&p->lock);
        return 0;
    }

    // Set up new context to start executing at forkret,
    // which returns to user space.
    memset(&p->context, 0, sizeof(p->context));
    p->context.ra = (uint64)forkret;
    p->context.sp = p->kstack + PGSIZE;

    return p;
}

//��ӳ��
pagetable_t
proc_pagetable(struct proc* p)
{
    pagetable_t pagetable;

    // An empty page table.
    pagetable = uvmcreate();
    if (pagetable == 0)
        return 0;

    // map the trampoline code (for system call return)
    // at the highest user virtual address.
    // only the supervisor uses it, on the way
    // to/from user space, so not PTE_U.
    if (mappages(pagetable, TRAMPOLINE, PGSIZE,
        (uint64)trampoline, PTE_R | PTE_X) < 0) {
        uvmfree(pagetable, 0);
        return 0;
    }

    // map the trapframe just below TRAMPOLINE, for trampoline.S.
    if (mappages(pagetable, TRAPFRAME, PGSIZE,
        (uint64)(p->trapframe), PTE_R | PTE_W) < 0) {
        uvmunmap(pagetable, TRAMPOLINE, 1, 0);
        uvmfree(pagetable, 0);
        return 0;
    }

    /*********************��ӵĲ���*****************************/
    p->usyscall->pid = p->pid;
    if (mappages(pagetable, USYSCALL, PGSIZE, (uint64)p->usyscall, PTE_R | PTE_U) < 0) {
        uvmunmap(pagetable, USYSCALL, 1, 0);
        uvmfree(pagetable, 0);
    }
    /*************************************************************/

    return pagetable;
}

//�ͷ������ڲ��ڴ�
static void
freeproc(struct proc* p)
{
    if (p->trapframe)
        kfree((void*)p->trapframe);
    p->trapframe = 0;
    /*********************��ӵĲ���*****************************/
    if (p->usyscall)
        kfree((void*)p->usyscall);
    p->usyscall = 0;

    if (p->pagetable)
        proc_freepagetable(p->pagetable, p->sz);
    p->pagetable = 0;
    p->sz = 0;
    p->pid = 0;
    p->parent = 0;
    p->name[0] = 0;
    p->chan = 0;
    p->killed = 0;
    p->xstate = 0;
    p->state = UNUSED;
}
