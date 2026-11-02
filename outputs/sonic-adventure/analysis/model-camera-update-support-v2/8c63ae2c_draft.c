/* Ghidra draft only; not buildable/verified source. */

void FUN_8c63ae2c(undefined8 param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 in_xd4;
  char in_FPSCR_SZ;
  
  iVar1 = (int)DAT_8c63ae7a;
  iVar2 = (int)DAT_8c63ae7e;
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar1 + param_4) = (int)((ulonglong)param_1 >> 0x20);
    *(int *)(iVar1 + 4 + param_4) = (int)param_1;
    *(undefined4 *)(iVar1 + 8 + param_4) = param_2;
  }
  else {
    *(undefined8 *)(iVar1 + param_4) = param_1;
    *(undefined8 *)(iVar1 + 4 + param_4) = in_xd4;
    *(ulonglong *)(iVar1 + 8 + param_4) = CONCAT44(param_2,param_3);
  }
  if (in_FPSCR_SZ == '\0') {
    uVar3 = CONCAT44(*(undefined4 *)(DAT_8c63ae7c + param_4),(int)param_1);
  }
  else {
    uVar3 = *(undefined8 *)(DAT_8c63ae7c + param_4);
  }
  FUN_8c63ae0c(uVar3,DAT_8c63ae80 + param_4,iVar2 + param_4);
  if (in_FPSCR_SZ == '\0') {
    uVar3 = CONCAT44(*(undefined4 *)(DAT_8c63ae82 + param_4),(int)uVar3);
  }
  else {
    uVar3 = *(undefined8 *)(DAT_8c63ae82 + param_4);
  }
  FUN_8c63ae0c(uVar3,DAT_8c63ae84 + param_4,DAT_8c63ae80 + param_4);
  if (in_FPSCR_SZ == '\0') {
    uVar3 = CONCAT44(*(undefined4 *)(DAT_8c63ae86 + param_4),(int)uVar3);
  }
  else {
    uVar3 = *(undefined8 *)(DAT_8c63ae86 + param_4);
  }
  FUN_8c63ae0c(uVar3,DAT_8c63ae88 + param_4,DAT_8c63ae80 + param_4);
  if (in_FPSCR_SZ == '\0') {
    uVar3 = CONCAT44(*(undefined4 *)(DAT_8c63ae8a + param_4),(int)uVar3);
  }
  else {
    uVar3 = *(undefined8 *)(DAT_8c63ae8a + param_4);
  }
  FUN_8c63ae0c(uVar3,DAT_8c63ae8c + param_4,DAT_8c63ae80 + param_4);
  return;
}

