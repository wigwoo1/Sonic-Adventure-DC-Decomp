/* Ghidra draft only; not buildable/verified source. */

int FUN_8c604148(int param_1)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  
  puVar1 = PTR_DAT_8c604320;
  if (*(int *)PTR_DAT_8c604320 == 0) {
    iVar3 = -0xb;
  }
  else if ((param_1 == 0) || (param_1 == 1)) {
    iVar3 = (*(code *)PTR_FUN_8c604324)(*(undefined4 *)(*(int *)PTR_DAT_8c604320 + 4),FUN_8c5ff31e);
    if (iVar3 == 0) {
      iVar3 = *(int *)puVar1;
      thunk_FUN_8c603c2e(PTR_s_KATANA_FLASH_____8c604328,iVar3 + 0x34,0x10);
      *(short *)(*(int *)puVar1 + 2) = (short)param_1;
      *(char *)(iVar3 + 0x45) = (char)param_1;
      puVar2 = PTR_FUN_8c60432c;
      *(undefined1 *)(iVar3 + 0x44) = **(undefined1 **)puVar1;
      iVar3 = (*(code *)puVar2)(*(undefined4 *)(*(int *)puVar1 + 4),iVar3 + 0x34,0x12);
      if (iVar3 == 0x12) {
        FUN_8c603c38(*(undefined4 *)(*(int *)puVar1 + 0xc),*(undefined4 *)(*(int *)puVar1 + 8));
        iVar3 = FUN_8c603d22(*(undefined4 *)puVar1);
        if (iVar3 == 0) {
          iVar3 = 0;
        }
      }
      else {
        iVar3 = -2;
      }
    }
    else {
      iVar3 = -3;
    }
  }
  else {
    iVar3 = -6;
  }
  return iVar3;
}

