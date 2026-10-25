/* Ghidra draft only; not buildable/verified source. */

void FUN_8c658ae0(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  undefined4 local_8;
  
  piVar1 = DAT_8c658bf8;
  iVar2 = (int)DAT_8c658be0;
  *(int *)(&stack0x00000000 + iVar2) = (int)DAT_8c658be2;
  *(int *)(&stack0x0000000c + iVar2) = (int)DAT_8c658be4;
  *(undefined4 *)((int)&local_8 + iVar2) = 0x7e;
  *(int *)(&stack0xfffffffc + iVar2) = (int)DAT_8c658be6;
  *(undefined4 *)(&stack0x00000004 + iVar2) = 0x28;
  *(int *)(&stack0x00000008 + iVar2) = (int)DAT_8c658be8;
  *(undefined4 *)(&stack0x00000010 + iVar2) = 0x3f;
  *(undefined4 *)(&stack0x00000014 + iVar2) = 3;
  *(int *)(&stack0x00000018 + iVar2) = (int)DAT_8c658bea;
  *(undefined4 *)(&stack0x0000001c + iVar2) = 0xf;
  *(int *)(&stack0x0000006c + iVar2) = (int)DAT_8c658be8;
  *(undefined4 *)(&stack0x00000070 + iVar2) = 0x15;
  *(undefined4 *)(&stack0x0000002c + iVar2) = 0;
  *(undefined4 *)(&stack0x00000030 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000034 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000038 + iVar2) = 0;
  *(undefined4 *)(&stack0x0000003c + iVar2) = 0;
  if (*DAT_8c658bf4 == 6) {
    *(undefined4 *)(&stack0x00000068 + iVar2) = 0x16;
  }
  else {
    *(undefined4 *)(&stack0x00000068 + iVar2) = 0x1f;
  }
  *(undefined4 *)(&stack0x00000064 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000020 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000024 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000028 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000040 + iVar2) = 1;
  *(undefined4 *)(&stack0x00000044 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000048 + iVar2) = 0;
  *(undefined4 *)(&stack0x0000004c + iVar2) = 0;
  *(undefined4 *)(&stack0x00000050 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000054 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000060 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000058 + iVar2) = 0;
  iVar3 = (int)DAT_8c658bee;
  *(int *)(&stack0x0000005c + iVar2) = (int)DAT_8c658bec;
  *(int *)(&stack0x00000060 + iVar3 + iVar2) = iVar3;
  *(undefined4 *)(&stack0x00000058 + iVar3 + iVar2) = 0x23;
  *(undefined4 *)(&stack0x0000005c + iVar3 + iVar2) = 0x23;
  *(int *)(&stack0x00000060 + iVar3 + iVar2) = iVar3 + 0x18;
  *(undefined4 *)(&stack0x00000058 + iVar3 + iVar2) = 0x28;
  *(undefined4 *)(&stack0x0000005c + iVar3 + iVar2) = 0x28;
  *DAT_8c658bf8 = (int)DAT_8c658bec;
  *DAT_8c658bfc = *(int *)(&stack0x00000004 + iVar2) + *piVar1;
  *DAT_8c658c00 = *(undefined4 *)(&stack0x00000004 + iVar2);
  (*(code *)PTR_FUN_8c658c04)();
  FUN_8c6582e0();
  return;
}

