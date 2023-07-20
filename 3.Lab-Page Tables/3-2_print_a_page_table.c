//kernel/vm.c
// 用于打印指定页表中的所有页表项
void vmprint(pagetable_t pagetable)
{ 
    printf("page table %p\n", pagetable);
    for (int i = 0; i < 512; i++) {
        // 获取当前页表项
        pte_t pte = pagetable[i];

        if (pte & PTE_V) {        //有效
            printf("..%d: pte %p pa %p\n", i, pte, PTE2PA(pte));

            // 获取二级页表的地址
            pagetable_t second = (pagetable_t)PTE2PA(pte);
            for (int j = 0; j < 512; j++) {               
                pte = second[j];
                if (pte & PTE_V) {
                    printf(".. ..%d: pte %p pa %p\n", j, pte, PTE2PA(pte));
                   
                    // 获取三级页表的地址
                    pagetable_t third = (pagetable_t)PTE2PA(pte); 
                    for (int k = 0; k < 512; k++) {
                        pte = third[k];
                        if (pte & PTE_V) {
                            printf(".. .. ..%d: pte %p pa %p\n", k, pte, PTE2PA(pte));
                        }
                    }
                }
            }
        }
    }
}