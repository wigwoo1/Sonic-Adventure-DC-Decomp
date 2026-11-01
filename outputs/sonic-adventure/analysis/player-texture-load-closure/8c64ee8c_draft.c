/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64ee8c(int param_1)

{
  short sVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  
  uVar4 = *(uint *)(param_1 + 4);
  uVar5 = uVar4 >> 0x10;
  if (uVar5 == (int)DAT_8c64eef6) {
    sVar1 = DAT_8c64eefc;
    if (*(int *)(param_1 + 0x10) == *(int *)(param_1 + 0x14)) {
      sVar1 = DAT_8c64eefa;
    }
    uVar5 = (int)sVar1 | uVar5 & 0xff;
  }
  else if (uVar5 == (int)DAT_8c64eef8) {
    uVar5 = (int)DAT_8c64eefe | uVar5 & 0xff;
  }
  iVar2 = (*(code *)PTR_FUN_8c64ef00)(param_1 + 4,uVar5);
  if (iVar2 == 0) {
    uVar3 = 1;
    *(uint *)(param_1 + 4) = *(uint *)(param_1 + 4) | uVar4;
  }
  else {
    *DAT_8c64ef04 = 6;
    uVar3 = 0xfffffffa;
  }
  return uVar3;
}

