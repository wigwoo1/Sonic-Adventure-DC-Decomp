/* Ghidra draft only; not buildable/verified source. */

void FUN_8c60edb0(uint param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined8 *puVar3;
  char in_FPSCR_SZ;
  
  puVar2 = DAT_8c60ee38;
  puVar1 = DAT_8c60ee30;
  *DAT_8c60ee2c = param_2;
  *puVar1 = param_3;
  puVar1 = DAT_8c60ee3c;
  *DAT_8c60ee34 = 0;
  *puVar2 = 0xffffffff;
  *puVar1 = 0;
  puVar3 = DAT_8c60ee40;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)DAT_8c60ee40 = (undefined4)DAT_8c60ee44;
    *(undefined4 *)((int)puVar3 + 4) = 0;
    *(undefined4 *)(puVar3 + 1) = 0;
    *(undefined4 *)((int)puVar3 + 0xc) = 0;
  }
  else {
    *DAT_8c60ee40 = CONCAT44(DAT_8c60ee44._4_4_,(undefined4)DAT_8c60ee44);
    *(ulonglong *)((int)puVar3 + 4) = (ulonglong)param_1;
    puVar3[1] = (ulonglong)param_1;
    *(ulonglong *)((int)puVar3 + 0xc) = (ulonglong)param_1;
  }
  return;
}

