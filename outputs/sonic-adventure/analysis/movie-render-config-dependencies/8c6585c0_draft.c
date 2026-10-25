/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c65869a) */
/* WARNING: Removing unreachable block (ram,0x8c658768) */

void FUN_8c6585c0(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  int local_8;
  
  piVar1 = DAT_8c658874;
  iVar2 = (int)DAT_8c658660;
  *(int *)(&stack0x00000000 + iVar2) = (int)DAT_8c658662;
  if (((*DAT_8c658674 & 1) == 1) || ((*DAT_8c658674 & 0x2c) == 0)) {
    *(int *)(&stack0x0000000c + iVar2) = (int)DAT_8c658666;
  }
  else {
    *(int *)(&stack0x0000000c + iVar2) = (int)DAT_8c658664;
  }
  *(int *)((int)&local_8 + iVar2) = (int)DAT_8c658668;
  *(int *)(&stack0xfffffffc + iVar2) = (int)DAT_8c65866a;
  *(undefined4 *)(&stack0x00000004 + iVar2) = 0x2c;
  *(int *)(&stack0x00000008 + iVar2) = (int)DAT_8c65866c;
  *(undefined4 *)(&stack0x00000010 + iVar2) = 0x3f;
  *(undefined4 *)(&stack0x00000014 + iVar2) = 5;
  *(int *)(&stack0x0000006c + iVar2) = (int)DAT_8c65866e;
  *(undefined4 *)(&stack0x00000070 + iVar2) = 0x15;
  if (((*DAT_8c658674 & 1) == 1) || ((*DAT_8c658674 & 0x2c) == 0)) {
    *(int *)(&stack0x00000018 + iVar2) = (int)DAT_8c658672;
  }
  else {
    *(int *)(&stack0x00000018 + iVar2) = (int)DAT_8c658670;
  }
  if (((*DAT_8c658674 & 1) == 1) || ((*DAT_8c658674 & 0x2c) == 0)) {
    *(undefined4 *)(&stack0x00000028 + iVar2) = 0;
  }
  else {
    *(undefined4 *)(&stack0x00000028 + iVar2) = 1;
  }
  *(undefined4 *)(&stack0x0000002c + iVar2) = 0;
  *(undefined4 *)(&stack0x00000030 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000034 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000038 + iVar2) = 0;
  if (((*DAT_8c658700 & 1) == 1) || ((*DAT_8c658700 & 0x2c) == 0)) {
    *(undefined4 *)(&stack0x0000003c + iVar2) = 2;
  }
  else {
    *(undefined4 *)(&stack0x0000003c + iVar2) = 0;
  }
  if (*DAT_8c658704 == 6) {
    *(undefined4 *)(&stack0x00000068 + iVar2) = 0x16;
  }
  else {
    *(undefined4 *)(&stack0x00000068 + iVar2) = 0x3f;
  }
  if ((*DAT_8c658700 & 0x10) == 0) {
    *(undefined4 *)(&stack0x00000064 + iVar2) = 1;
  }
  else {
    *(undefined4 *)(&stack0x00000064 + iVar2) = 0;
  }
  *(undefined4 *)(&stack0x0000001c + iVar2) = 0x1f;
  *(undefined4 *)(&stack0x00000020 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000024 + iVar2) = 1;
  *(undefined4 *)(&stack0x00000040 + iVar2) = 1;
  *(undefined4 *)(&stack0x00000044 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000048 + iVar2) = 0;
  *(undefined4 *)(&stack0x0000004c + iVar2) = 0;
  *(undefined4 *)(&stack0x00000050 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000054 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000060 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000058 + iVar2) = 0;
  *(int *)(&stack0x0000005c + iVar2) = (int)DAT_8c6587a0;
  *(int *)(&stack0x00000042 + DAT_8c6587a2 + iVar2) = (int)DAT_8c6587a2;
  if (((*DAT_8c6587a4 & 1) == 1) || ((*DAT_8c6587a4 & 0x2c) == 0)) {
    iVar3 = (int)DAT_8c658860;
    *(undefined4 *)((int)&local_8 + iVar3 + iVar2) = 0x2e;
  }
  else {
    iVar3 = (int)DAT_8c6587a0;
    *(undefined4 *)((int)&local_8 + iVar3 + iVar2) = 0x2d;
  }
  *(undefined4 *)(&stack0xfffffffc + iVar3 + iVar2) = 0x2e;
  *DAT_8c658874 = (int)DAT_8c6587a0;
  *DAT_8c658878 =
       ((int)(*(int *)(&stack0x00000004 + iVar2) + (uint)(*(int *)(&stack0x00000004 + iVar2) < 0))
       >> 1) + *piVar1;
  *DAT_8c65887c = *(undefined4 *)(&stack0x00000004 + iVar2);
  (*(code *)PTR_FUN_8c658880)();
  FUN_8c6582e0();
  return;
}

