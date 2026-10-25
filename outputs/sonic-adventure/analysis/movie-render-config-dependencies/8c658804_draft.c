/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c658958) */
/* WARNING: Removing unreachable block (ram,0x8c658a12) */

void FUN_8c658804(void)

{
  int *piVar1;
  int iVar2;
  undefined4 local_8;
  
  piVar1 = DAT_8c658ac8;
  iVar2 = (int)DAT_8c658864;
  *(int *)(&stack0x00000000 + iVar2) = (int)DAT_8c658866;
  if (((*DAT_8c658884 & 1) == 1) || ((*DAT_8c658884 & 0x2c) == 0)) {
    *(int *)(&stack0x0000000c + iVar2) = (int)DAT_8c65886a;
  }
  else {
    *(int *)(&stack0x0000000c + iVar2) = (int)DAT_8c658868;
  }
  *(undefined4 *)((int)&local_8 + iVar2) = 0x7e;
  *(int *)(&stack0xfffffffc + iVar2) = (int)DAT_8c65886c;
  if (((*DAT_8c658884 & 1) == 1) || ((*DAT_8c658884 & 0x2c) == 0)) {
    *(undefined4 *)(&stack0x00000004 + iVar2) = 0x12;
    *(int *)(&stack0x00000008 + iVar2) = (int)DAT_8c65886e;
  }
  else {
    *(undefined4 *)(&stack0x00000004 + iVar2) = 0x24;
    *(int *)(&stack0x00000008 + iVar2) = (int)DAT_8c658870;
  }
  *(undefined4 *)(&stack0x00000010 + iVar2) = 0x3f;
  if ((*DAT_8c658904 & 0x20) == 0) {
    *(int *)(&stack0x0000006c + iVar2) = (int)DAT_8c65886e;
  }
  else {
    *(int *)(&stack0x0000006c + iVar2) = (int)DAT_8c658900;
  }
  *(undefined4 *)(&stack0x00000070 + iVar2) = 0x15;
  if (((*DAT_8c658904 & 1) == 1) || ((*DAT_8c658904 & 0x2c) == 0)) {
    *(undefined4 *)(&stack0x00000014 + iVar2) = 3;
  }
  else {
    *(undefined4 *)(&stack0x00000014 + iVar2) = 6;
  }
  if (((*DAT_8c658904 & 1) == 1) || ((*DAT_8c658904 & 0x2c) == 0)) {
    *(int *)(&stack0x00000018 + iVar2) = (int)DAT_8c658980;
  }
  else {
    *(int *)(&stack0x00000018 + iVar2) = (int)DAT_8c658902;
  }
  if (((*DAT_8c658984 & 1) == 1) || ((*DAT_8c658984 & 0x2c) == 0)) {
    *(undefined4 *)(&stack0x0000001c + iVar2) = 0xf;
  }
  else {
    *(undefined4 *)(&stack0x0000001c + iVar2) = 0x1f;
  }
  *(undefined4 *)(&stack0x00000020 + iVar2) = 1;
  *(undefined4 *)(&stack0x00000024 + iVar2) = 0;
  if (((*DAT_8c658984 & 1) == 1) || ((*DAT_8c658984 & 0x2c) == 0)) {
    *(undefined4 *)(&stack0x00000028 + iVar2) = 0;
  }
  else {
    *(undefined4 *)(&stack0x00000028 + iVar2) = 1;
  }
  *(undefined4 *)(&stack0x0000002c + iVar2) = 0;
  *(undefined4 *)(&stack0x00000030 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000034 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000038 + iVar2) = 0;
  *(undefined4 *)(&stack0x0000003c + iVar2) = 0;
  if (*DAT_8c658a24 == 6) {
    *(undefined4 *)(&stack0x00000068 + iVar2) = 0x16;
  }
  else {
    *(undefined4 *)(&stack0x00000068 + iVar2) = 0x1f;
  }
  if ((*DAT_8c658a28 & 0x10) == 0) {
    *(undefined4 *)(&stack0x00000064 + iVar2) = 1;
  }
  else {
    *(undefined4 *)(&stack0x00000064 + iVar2) = 0;
  }
  *(undefined4 *)(&stack0x00000040 + iVar2) = 1;
  *(undefined4 *)(&stack0x00000044 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000048 + iVar2) = 0;
  *(undefined4 *)(&stack0x0000004c + iVar2) = 0;
  *(undefined4 *)(&stack0x00000050 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000054 + iVar2) = 0;
  *(undefined4 *)(&stack0x00000060 + iVar2) = 0;
  *(int *)(&stack0x0000004c + DAT_8c658a20 + iVar2) = (int)DAT_8c658a20;
  *(int *)(&stack0x0000005c + iVar2) = (int)DAT_8c658a22;
  *(undefined4 *)(&stack0x00000058 + iVar2) = 0;
  if (((*DAT_8c658a28 & 1) == 1) || ((*DAT_8c658ac4 & 0x2c) == 0)) {
    *(undefined4 *)((int)&local_8 + DAT_8c658ac0 + iVar2) = 0x11;
  }
  else {
    *(undefined4 *)((int)&local_8 + DAT_8c658ac0 + iVar2) = 0x12;
  }
  *(undefined4 *)(&stack0xfffffffc + DAT_8c658ac0 + iVar2) = 0x12;
  *DAT_8c658ac8 = (int)DAT_8c658ac0;
  *DAT_8c658acc =
       ((int)(*(int *)(&stack0x00000004 + iVar2) + (uint)(*(int *)(&stack0x00000004 + iVar2) < 0))
       >> 1) + *piVar1;
  *DAT_8c658ad0 = *(undefined4 *)(&stack0x00000004 + iVar2);
  (*(code *)PTR_FUN_8c658ad4)();
  FUN_8c6582e0();
  return;
}

