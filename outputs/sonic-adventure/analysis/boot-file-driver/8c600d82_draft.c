/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c600da2) */

int * FUN_8c600d82(int *param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  
  if ((param_1 != (int *)0x0) && (*(int **)(*param_1 + 0x1c) != param_1)) {
    param_1[1] = param_2;
    iVar1 = (int)DAT_8c600de2;
    param_1[2] = param_3;
    param_1[3] = param_4;
    param_1[4] = param_4 + iVar1 >> 0xb;
    param_1[5] = 0;
    param_1[6] = 0;
    param_1[7] = 0;
    param_1[8] = 0;
    param_1[0xf] = 0;
    param_1[0x11] = 0;
    *(undefined2 *)(param_1 + 0x12) = 0;
    *(undefined2 *)((int)param_1 + 0x4a) = 0;
    *(undefined2 *)(param_1 + 0x13) = 1;
    *(undefined2 *)(param_1 + 0x14) = 0;
    *(undefined2 *)((int)param_1 + 0x52) = 0;
    return param_1;
  }
  return (int *)0x0;
}

