/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c600494) */

int FUN_8c600464(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  
  if (*(short *)(param_1 + 0x4c) == 0) {
    iVar1 = (int)DAT_8c600594;
    *(undefined4 *)(param_1 + 4) = param_2;
    *(undefined4 *)(param_1 + 8) = param_3;
    *(int *)(param_1 + 0xc) = param_4;
    *(int *)(param_1 + 0x10) = param_4 + iVar1 >> 0xb;
    *(undefined4 *)(param_1 + 0x14) = 0;
    *(undefined4 *)(param_1 + 0x18) = 0;
    *(undefined4 *)(param_1 + 0x1c) = 0;
    *(undefined4 *)(param_1 + 0x20) = 0;
    *(undefined4 *)(param_1 + 0x24) = 0;
    *(undefined4 *)(param_1 + 0x2c) = 0;
    *(undefined4 *)(param_1 + 0x34) = 0;
    *(undefined4 *)(param_1 + 0x3c) = 0;
    *(undefined4 *)(param_1 + 0x40) = 0;
    *(undefined4 *)(param_1 + 0x44) = 0;
    *(undefined2 *)(param_1 + 0x48) = 0;
    *(undefined2 *)(param_1 + 0x4a) = 0;
    *(undefined2 *)(param_1 + 0x4c) = 1;
    *(undefined2 *)(param_1 + 0x4e) = *(undefined2 *)PTR_DAT_8c600598;
    *(undefined2 *)(param_1 + 0x50) = 0;
    *(undefined2 *)(param_1 + 0x52) = 0;
    return param_1;
  }
  return 0;
}

