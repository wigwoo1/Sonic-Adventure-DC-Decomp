/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64a7ba(int param_1)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = param_1 * DAT_8c64a862 + DAT_8c64a864;
  iVar3 = 0;
  do {
    if (**(int **)(iVar4 + 0x1c) + 1 <= iVar3) {
      return 0;
    }
    uVar1 = FUN_8c64b1c0(param_1,iVar3);
    uVar1 = uVar1 & 0xffff;
    if (((uVar1 != DAT_8c64a868) && (uVar1 != DAT_8c64a86c)) && (uVar1 != DAT_8c64a870)) {
      if (iVar3 < *(int *)(*(int *)(iVar4 + 0x1c) + 0x30)) {
        iVar2 = *(int *)(*(int *)(iVar4 + 0x1c) + 0x30);
      }
      else {
        iVar2 = **(int **)(iVar4 + 0x1c) + 1;
      }
      if (iVar2 <= (int)uVar1) {
        return 0xffffffff;
      }
    }
    iVar3 = iVar3 + 1;
  } while( true );
}

