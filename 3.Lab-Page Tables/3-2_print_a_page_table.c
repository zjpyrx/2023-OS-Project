//kernel/vm.c
// ���ڴ�ӡָ��ҳ���е�����ҳ����
void vmprint(pagetable_t pagetable)
{ 
    printf("page table %p\n", pagetable);
    for (int i = 0; i < 512; i++) {
        // ��ȡ��ǰҳ����
        pte_t pte = pagetable[i];

        if (pte & PTE_V) {        //��Ч
            printf("..%d: pte %p pa %p\n", i, pte, PTE2PA(pte));

            // ��ȡ����ҳ��ĵ�ַ
            pagetable_t second = (pagetable_t)PTE2PA(pte);
            for (int j = 0; j < 512; j++) {               
                pte = second[j];
                if (pte & PTE_V) {
                    printf(".. ..%d: pte %p pa %p\n", j, pte, PTE2PA(pte));
                   
                    // ��ȡ����ҳ��ĵ�ַ
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