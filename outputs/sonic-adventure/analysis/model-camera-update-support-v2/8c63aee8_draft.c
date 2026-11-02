/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63aee8(undefined8 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  iVar1 = (int)DAT_8c63af36;
  iVar2 = (int)DAT_8c63af3a;
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar1 + param_4) = (int)((ulonglong)param_1 >> 0x20);
    *(int *)(iVar1 + -4 + param_4) = (int)param_1;
    *(undefined4 *)(iVar1 + -8 + param_4) = param_2;
  }
  else {
    *(undefined8 *)(iVar1 + param_4) = param_1;
    *(undefined8 *)(iVar1 + -4 + param_4) = in_xd4;
    *(ulonglong *)(iVar1 + -8 + param_4) = CONCAT44(param_2,param_3);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar3 = CONCAT44(*(undefined4 *)(DAT_8c63af38 + param_4),(int)param_1);
  }
  else {
    uVar3 = *(undefined8 *)(DAT_8c63af38 + param_4);
  }
  FUN_8c63aec8(uVar3,DAT_8c63af3c + param_4,iVar2 + param_4);
  if (in_FPSCR_SZ == '\0') {
    uVar3 = CONCAT44(*(undefined4 *)(DAT_8c63af36 + param_4),(int)uVar3);
  }
  else {
    uVar3 = *(undefined8 *)(DAT_8c63af36 + param_4);
  }
  FUN_8c63aec8(uVar3,DAT_8c63af3e + param_4,DAT_8c63af3c + param_4);
  if (in_FPSCR_SZ == '\0') {
    uVar3 = CONCAT44(*(undefined4 *)(DAT_8c63af40 + param_4),(int)uVar3);
  }
  else {
    uVar3 = *(undefined8 *)(DAT_8c63af40 + param_4);
  }
  FUN_8c63aec8(uVar3,DAT_8c63af42 + param_4,DAT_8c63af3c + param_4);
  if (in_FPSCR_SZ == '\0') {
    uVar3 = CONCAT44(*(undefined4 *)(DAT_8c63af44 + param_4),(int)uVar3);
  }
  else {
    uVar3 = *(undefined8 *)(DAT_8c63af44 + param_4);
  }
  FUN_8c63aec8(uVar3,DAT_8c63af46 + param_4,DAT_8c63af3c + param_4);
  return;
}

