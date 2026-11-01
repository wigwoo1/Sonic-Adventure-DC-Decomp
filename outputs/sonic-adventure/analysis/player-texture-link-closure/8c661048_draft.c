/* Ghidra draft only; not buildable/verified source. */

int FUN_8c661048(int param_1,uint param_2,undefined4 param_3)

{
  int iVar1;
  uint local_1c;
  undefined4 local_18;
  undefined4 local_14;
  uint local_10;
  undefined4 local_c;
  
  local_10 = param_2;
  local_c = param_3;
  iVar1 = FUN_8c660f44(&local_18,&local_14,&local_1c,param_3);
  if (iVar1 < 0) {
    return iVar1;
  }
  if ((local_1c == (int)DAT_8c661136) && ((local_10 & DAT_8c66113c) != 0)) {
    local_1c = local_1c & 0xff | (int)DAT_8c661138;
  }
  iVar1 = (*(code *)PTR_FUN_8c661140)(param_1 + 4,local_18,local_14,local_1c);
  if (iVar1 != 0) {
    *DAT_8c661144 = 6;
    return -6;
  }
  *(uint *)(param_1 + 4) =
       *(uint *)(param_1 + 4) | local_1c << 0x10 & DAT_8c661148 | *(uint *)(param_1 + 4) & 0xffff;
  return 1;
}

