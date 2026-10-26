/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c665d18) */

void FUN_8c665cc0(int param_1,int param_2,int param_3)

{
  short sVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint local_1c [4];
  undefined4 local_c;
  undefined4 local_8;
  
  local_1c[0] = *(uint *)PTR_DAT_8c665d68;
  local_1c[1] = *(undefined4 *)(PTR_DAT_8c665d68 + 4);
  local_1c[2] = *(undefined4 *)(PTR_DAT_8c665d68 + 8);
  local_1c[3] = *(undefined4 *)(PTR_DAT_8c665d68 + 0xc);
  local_c = *(undefined4 *)(PTR_DAT_8c665d68 + 0x10);
  local_8 = *(undefined4 *)(PTR_DAT_8c665d68 + 0x14);
  sVar1 = DAT_8c665d60;
  if ((*DAT_8c665d6c != 1) && ((*DAT_8c665d70 & 0x10) == 0)) {
    sVar1 = DAT_8c665d62;
  }
  if (param_2 == 1) {
    uVar4 = param_3 + 0x1fU & 0xffffffe0;
    *DAT_8c665d78 = *DAT_8c665d78 & DAT_8c665d74 | 0x400000 | (param_3 + 0x1fU >> 5 & 0x1f) << 0x10;
    goto LAB_8c665d90;
  }
  if (*DAT_8c665d6c != 1) {
    if (((*DAT_8c665d70 & 1) != 1) && ((*DAT_8c665d70 & 0x2c) == 0x20)) {
      uVar4 = (uint)DAT_8c665d64;
      goto LAB_8c665d90;
    }
    if ((*DAT_8c665de8 & 0x20) == 0) {
      uVar4 = (uint)DAT_8c665de2;
      goto LAB_8c665d90;
    }
  }
  uVar4 = (uint)DAT_8c665de0;
LAB_8c665d90:
  iVar2 = DAT_8c665dec;
  iVar3 = (int)DAT_8c665de4;
  *(int *)(DAT_8c665dec + 0x48) = (int)sVar1;
  *(uint *)(iVar2 + 0x4c) = uVar4;
  if (param_1 == iVar3) {
    param_1 = *DAT_8c665df0;
  }
  *(uint *)(iVar2 + 0x50) = local_1c[param_1] >> 3;
  *(int *)(iVar2 + 0x60) = *(int *)(iVar2 + 0x48) * *(int *)(iVar2 + 0x4c) * *(int *)(iVar2 + 0x50);
  return;
}

