/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c06cd5c) */

undefined4 FUN_8c06cd22(int param_1,undefined4 *param_2,undefined8 *param_3)

{
  short sVar1;
  int iVar2;
  int iVar3;
  longlong lVar4;
  undefined8 in_xd2;
  char in_FPSCR_SZ;
  
  iVar3 = DAT_8c06ce18;
  if ((*(ushort *)(param_1 + 4) & 0x4000) == 0) {
    iVar2 = (uint)*(byte *)(param_1 + 8) * 8 + DAT_8c06ce18;
    if (in_FPSCR_SZ == '\0') {
      lVar4 = (ulonglong)*(uint *)(iVar2 + 4) << 0x20;
    }
    else {
      lVar4 = *(longlong *)(iVar2 + 4);
    }
    if ((float)((ulonglong)lVar4 >> 0x20) == (float)lVar4) {
      if (*(char *)(param_1 + 9) != '\0') {
        return 0;
      }
      if ((*(ushort *)(param_1 + 4) & 0x100) == 0) {
        return 0;
      }
      sVar1 = (*(code *)PTR_FUN_8c06ce20)(0x28);
      if ((sVar1 != 0x902) && (sVar1 = (*(code *)PTR_FUN_8c06ce20)(), sVar1 != 0x903)) {
        return 0;
      }
      if (param_2 != (undefined4 *)0x0) {
        *param_2 = *(undefined4 *)(param_1 + 0x18);
      }
      if (param_3 != (undefined8 *)0x0) {
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)param_3 = 0;
        }
        else {
          *param_3 = in_xd2;
        }
      }
    }
    else {
      if (((*(ushort *)(param_1 + 4) & 0x2000) == 0) &&
         (*(short *)(*(int *)((uint)*(byte *)(param_1 + 8) * 4 + DAT_8c06ce1c) + 10) != 0)) {
        return 0;
      }
      if (param_2 != (undefined4 *)0x0) {
        *param_2 = *(undefined4 *)(DAT_8c06ce18 + (uint)*(byte *)(param_1 + 8) * 8);
      }
      if (param_3 != (undefined8 *)0x0) {
        iVar3 = iVar3 + (uint)*(byte *)(param_1 + 8) * 8;
        if (in_FPSCR_SZ == '\0') {
          *(undefined4 *)param_3 = *(undefined4 *)(iVar3 + 4);
        }
        else {
          *param_3 = *(undefined8 *)(iVar3 + 4);
        }
      }
    }
  }
  else if (*(char *)(*(int *)(param_1 + 0x3c) + 0x24) == '\0') {
    return 0;
  }
  return 1;
}

