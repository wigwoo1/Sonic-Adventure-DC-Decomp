/* Ghidra draft only; not buildable/verified source. */

void FUN_8c610cf8(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined8 *puVar1;
  double *pdVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  double dVar5;
  undefined8 in_xd4;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  puVar4 = DAT_8c610d70;
  puVar3 = DAT_8c610d6c;
  pdVar2 = DAT_8c610d68;
  puVar1 = DAT_8c610d64;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)DAT_8c610d64 = param_1;
    *(undefined4 *)((int)puVar1 + 4) = param_2;
    *(undefined4 *)(puVar1 + 1) = param_3;
    dVar5 = (double)CONCAT44(*(undefined4 *)pdVar2,*puVar3);
  }
  else {
    *DAT_8c610d64 = CONCAT44(param_1,param_2);
    *(undefined8 *)((int)puVar1 + 4) = in_xd4;
    puVar1[1] = CONCAT44(param_3,param_4);
    dVar5 = *pdVar2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * SUB84(dVar5,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)((int)puVar1 + 0x24) = (int)((ulonglong)dVar5 >> 0x20);
    dVar5 = (double)CONCAT44(*(undefined4 *)pdVar2,*puVar4);
  }
  else {
    *(double *)((int)puVar1 + 0x24) = dVar5;
    dVar5 = *pdVar2;
  }
  if (in_FPSCR_PR == '\0') {
    dVar5 = (double)CONCAT44((float)((ulonglong)dVar5 >> 0x20) * SUB84(dVar5,0),SUB84(dVar5,0));
  }
  else {
    dVar5 = dVar5 * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(puVar1 + 5) = (int)((ulonglong)dVar5 >> 0x20);
    *(undefined4 *)((int)puVar1 + 0x2c) = *(undefined4 *)((int)pdVar2 + 0xc);
    *(undefined4 *)(puVar1 + 6) = *(undefined4 *)(pdVar2 + 2);
  }
  else {
    puVar1[5] = dVar5;
    *(undefined8 *)((int)puVar1 + 0x2c) = *(undefined8 *)((int)pdVar2 + 0xc);
    puVar1[6] = pdVar2[2];
  }
  return;
}

