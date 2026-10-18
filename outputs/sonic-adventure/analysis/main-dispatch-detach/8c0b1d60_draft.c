/* Ghidra draft only; not buildable/verified source. */

void FUN_8c0b1d60(undefined4 param_1,int param_2,int param_3)

{
  undefined *puVar1;
  uint uVar2;
  int iVar3;
  undefined8 in_xd2;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  undefined4 local_18;
  undefined4 uStack_14;
  undefined4 local_10;
  undefined4 local_c;
  
  if ((*(ushort *)(param_2 + 4) & 0x800) != 0) {
    iVar3 = *(int *)(*(int *)(param_3 + 0x68) + 0x20);
    if (iVar3 != 0) {
      *(ushort *)(iVar3 + 4) = *(ushort *)(iVar3 + 4) & 0xefff;
    }
    puVar1 = PTR_FUN_8c0b1ee0;
    *(ushort *)(param_2 + 4) = *(ushort *)(param_2 + 4) & 0xf7ff;
    local_c = in_PR;
    uVar2 = (*(code *)puVar1)(param_2);
    if ((uVar2 & 0x200) != 0) {
      if (in_FPSCR_SZ == '\0') {
        uStack_14 = 0x3f800000;
        local_10 = 0;
      }
      else {
        local_10 = (undefined4)in_xd2;
        local_c = (undefined4)((ulonglong)in_xd2 >> 0x20);
        uStack_14 = param_1;
      }
      local_18 = uStack_14;
      (*(code *)PTR_FUN_8c0b1ee4)(param_2,&local_18);
      iVar3 = *(int *)(*(int *)(param_3 + 0x68) + 0x24);
      *(undefined4 *)(iVar3 + 4) = local_18;
      *(undefined4 *)(iVar3 + 8) = uStack_14;
      *(undefined4 *)(iVar3 + 0xc) = local_10;
    }
    *(undefined4 *)(param_3 + 0x68) = 0;
  }
  return;
}

