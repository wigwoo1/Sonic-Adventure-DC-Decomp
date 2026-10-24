/* Ghidra draft only; not buildable/verified source. */

int FUN_8c600fac(int *param_1,int param_2)

{
  int iVar1;
  int iVar2;
  byte in_IMASK;
  byte in_S;
  
  if ((param_1 == (int *)0x0) || ((short)param_1[0x13] == 0)) {
    return -10;
  }
  if (*(int *)(*param_1 + 0x1c) == 0) {
    iVar2 = param_1[5] + param_1[2];
    iVar1 = param_1[2] + param_1[4];
    if (iVar2 < iVar1) {
      if (iVar1 < param_2 + iVar2) {
        param_2 = iVar1 - iVar2;
      }
      if (param_2 != 0) {
        iVar1 = (**(code **)(*(int *)(*param_1 + 0xc) + 4))
                          (iVar2,param_2,0,(((uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4);
        if (iVar1 == 0) {
          *(undefined2 *)(param_1 + 0x14) = 1;
          return 0;
        }
        param_1[0xf] = iVar1;
        param_1[7] = param_2;
        *(undefined2 *)(param_1 + 0x14) = 2;
        *(undefined2 *)((int)param_1 + 0x4a) = 1;
        *(undefined2 *)(param_1 + 0x12) = 1;
        param_1[6] = 0;
        *(int **)(*param_1 + 0x1c) = param_1;
        (**(code **)(*(int *)(*param_1 + 0xc) + 0x40))();
        return param_2;
      }
    }
    param_1[6] = 0;
    *(undefined2 *)(param_1 + 0x14) = 1;
  }
  return 0;
}

