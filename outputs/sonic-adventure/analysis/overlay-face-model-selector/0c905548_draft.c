/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0c905548(undefined4 param_1,int param_2)

{
  byte bVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined8 uVar4;
  char in_FPSCR_SZ;
  
  puVar2 = PTR_DAT_0c9055ec;
  puVar3 = _DAT_0c9055e8;
  bVar1 = *(byte *)(*(int *)(param_2 + 0xec) + 0x34);
  if (in_FPSCR_SZ == '\0') {
    uVar4 = CONCAT44(_DAT_0c9055e8,param_1);
    *(undefined **)(*(int *)(param_2 + 0x2d4) + 0x20) = _DAT_0c9055e8;
    puVar2 = puVar3;
  }
  else {
    uVar4 = CONCAT44(PTR_DAT_0c9055ec,_DAT_0c9055e8);
    *(undefined8 *)(*(int *)(param_2 + 0x2d4) + 0x20) = uVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined **)(*(int *)(param_2 + 0x2d4) + 0x24) = puVar2;
  }
  else {
    *(undefined8 *)(*(int *)(param_2 + 0x2d4) + 0x24) = uVar4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined **)(*(int *)(param_2 + 0x2d4) + 0x28) = puVar2;
  }
  else {
    *(undefined8 *)(*(int *)(param_2 + 0x2d4) + 0x28) = uVar4;
  }
  if ((bVar1 & 0x10) != 0) {
    *(undefined4 *)(*(int *)(param_2 + 0x2d4) + 4) = *(undefined4 *)(PTR_DAT_0c9055ec + 4);
    return;
  }
  if ((bVar1 & 8) != 0) {
    *(undefined4 *)(*(int *)(param_2 + 0x2d4) + 4) = *(undefined4 *)(PTR_DAT_0c9055f0 + 4);
    return;
  }
  if ((bVar1 & 4) == 0) {
    if ((bVar1 & 2) == 0) {
      if ((bVar1 & 1) != 0) {
        *(undefined4 *)(*(int *)(param_2 + 0x2d4) + 4) = *(undefined4 *)(PTR_DAT_0c905740 + 4);
      }
      return;
    }
    *(undefined4 *)(*(int *)(param_2 + 0x2d4) + 4) = *(undefined4 *)(PTR_DAT_0c9055f8 + 4);
    return;
  }
  *(undefined4 *)(*(int *)(param_2 + 0x2d4) + 4) = *(undefined4 *)(PTR_DAT_0c9055f4 + 4);
  return;
}

