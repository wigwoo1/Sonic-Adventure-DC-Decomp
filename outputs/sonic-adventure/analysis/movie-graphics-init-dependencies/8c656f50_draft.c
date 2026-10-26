/* Ghidra draft only; not buildable/verified source. */

char FUN_8c656f50(int param_1,int param_2,int param_3,int param_4,int param_5)

{
  uint *puVar1;
  int *piVar2;
  char cVar3;
  int local_28 [5];
  
  puVar1 = DAT_8c657128;
  for (piVar2 = local_28; piVar2 < &stack0xffffffec; piVar2 = piVar2 + 1) {
    *piVar2 = 0;
  }
  if (*DAT_8c657120 == 1) {
    if (*DAT_8c657124 == 0x20) {
      local_28[0] = 1;
    }
    if (*DAT_8c657124 == 0x21) {
      local_28[1] = 1;
    }
    if (*DAT_8c657124 == 0x22) {
      local_28[2] = 1;
    }
    if (*DAT_8c657124 == 0x23) {
      local_28[3] = 1;
    }
    if (*DAT_8c657124 == 0x24) {
      local_28[4] = 1;
    }
  }
  cVar3 = param_1 + local_28[0] != 0;
  *DAT_8c657128 = 0;
  if ((bool)cVar3) {
    *puVar1 = *puVar1 | 1;
  }
  if (param_2 + local_28[1] != 0) {
    *DAT_8c657128 = *DAT_8c657128 | 2;
    cVar3 = cVar3 + '\x01';
  }
  if (param_3 + local_28[2] != 0) {
    *DAT_8c657128 = *DAT_8c657128 | 4;
    cVar3 = cVar3 + '\x01';
  }
  if (param_4 + local_28[3] != 0) {
    *DAT_8c657128 = *DAT_8c657128 | 8;
    cVar3 = cVar3 + '\x01';
  }
  if (param_5 + local_28[4] != 0) {
    *DAT_8c657128 = *DAT_8c657128 | 0x10;
    cVar3 = cVar3 + '\x01';
  }
  return cVar3;
}

