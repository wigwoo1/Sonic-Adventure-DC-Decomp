/* Ghidra draft only; not buildable/verified source. */

undefined4
FUN_8c0940da(uint param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5)

{
  int iVar1;
  undefined4 *puVar2;
  char in_FPSCR_SZ;
  
  iVar1 = FUN_8c093f02(param_4,param_2);
  if (-1 < iVar1) {
    puVar2 = (undefined4 *)(iVar1 * 0x34 + DAT_8c094224);
    puVar2[1] = 0x78;
    *puVar2 = 0xffffffff;
    puVar2[2] = param_3;
    puVar2[3] = 0x11;
    puVar2[4] = param_2;
    puVar2[5] = 0;
    puVar2[6] = param_5;
    puVar2[7] = 0;
    puVar2[8] = 0;
    if (in_FPSCR_SZ == '\0') {
      puVar2[9] = 0;
      puVar2[10] = 0;
    }
    else {
      *(ulonglong *)(puVar2 + 9) = (ulonglong)param_1;
      *(ulonglong *)(puVar2 + 10) = (ulonglong)param_1;
    }
    if (in_FPSCR_SZ == '\0') {
      puVar2[0xb] = 0;
    }
    else {
      *(ulonglong *)(puVar2 + 0xb) = (ulonglong)param_1;
    }
    return 0x2c;
  }
  return 0xffffffff;
}

