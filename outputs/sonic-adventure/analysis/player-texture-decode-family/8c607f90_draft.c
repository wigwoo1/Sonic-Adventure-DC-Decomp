/* Ghidra draft only; not buildable/verified source. */

void FUN_8c607f90(void)

{
  int iVar1;
  int iVar2;
  int local_c;
  
  iVar1 = (int)DAT_8c608062;
  do {
    (*(code *)PTR_FUN_8c608070)(&stack0xfffffff8 + iVar1,4);
    iVar2 = *(int *)(&stack0xfffffff8 + iVar1);
    if ((((iVar2 == DAT_8c608074) || (iVar2 == DAT_8c608078)) || (iVar2 == DAT_8c60807c)) ||
       ((iVar2 == DAT_8c608080 || (iVar2 == DAT_8c608084)))) {
      (*(code *)PTR_FUN_8c608070)((int)&local_c + iVar1,4);
LAB_8c6080a8:
      (*(code *)PTR_FUN_8c6081cc)(*(undefined4 *)((int)&local_c + iVar1),1);
    }
    else if (iVar2 == DAT_8c608088) {
      (*(code *)PTR_FUN_8c608070)((int)&local_c + DAT_8c608064 + iVar1,0xc);
      (*(code *)PTR_FUN_8c608090)(*(undefined2 *)((int)&local_c + DAT_8c608066 + iVar1));
      if (*(short *)((int)&local_c + DAT_8c608068 + iVar1) == 6) {
        iVar2 = 4;
      }
      else {
        iVar2 = 2;
      }
      (*(code *)PTR_FUN_8c608070)
                (&stack0xfffffffc + iVar1,*(short *)((int)&local_c + DAT_8c60806a + iVar1) * iVar2);
      iVar2 = (*(int *)((int)&local_c + DAT_8c608064 + iVar1) -
              *(short *)((int)&local_c + DAT_8c60806a + iVar1) * iVar2) + -8;
      *(int *)((int)&local_c + iVar1) = iVar2;
      (*(code *)PTR_FUN_8c608094)(iVar2,1);
      (*(code *)PTR_FUN_8c608098)(*(undefined2 *)((int)&local_c + DAT_8c608066 + iVar1));
      (*(code *)PTR_FUN_8c60809c)
                ((int)*(short *)((int)&local_c + DAT_8c60806c + iVar1) +
                 *(short *)((int)&local_c + DAT_8c608068 + iVar1) * 0x10,
                 (int)*(short *)((int)&local_c + DAT_8c60806a + iVar1),&stack0xfffffffc + iVar1);
    }
    else {
      if (iVar2 != DAT_8c60808c) {
        (*(code *)PTR_FUN_8c6081c8)((int)&local_c + iVar1,4);
        goto LAB_8c6080a8;
      }
      (*(code *)PTR_FUN_8c608094)(0xfffffffc,1);
    }
    if (*(int *)(&stack0xfffffff8 + iVar1) == DAT_8c6081d0) {
      return;
    }
  } while( true );
}

