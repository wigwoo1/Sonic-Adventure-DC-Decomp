/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64bb4c(int param_1,uint param_2)

{
  uint uVar1;
  undefined4 uVar2;
  uint uVar3;
  
  uVar3 = 0;
  uVar2 = *(undefined4 *)(param_1 + 0x28);
  *(undefined4 *)(param_1 + 0x40) = *(undefined4 *)(param_1 + 0x38);
  if (param_2 != 0) {
    do {
      if (*(int *)(param_1 + 0x40) == DAT_8c64bc50) {
        if (*(int *)(param_1 + 0x34) != 0) {
          return DAT_8c64bc54;
        }
        return 0;
      }
      if (*(int *)(param_1 + 0x34) == 0) {
        return DAT_8c64bc54;
      }
      uVar1 = FUN_8c64b1c0(uVar2,*(undefined4 *)(param_1 + 0x40));
      uVar3 = uVar3 + 1;
      *(uint *)(param_1 + 0x40) = uVar1 & 0xffff;
      *(int *)(param_1 + 0x34) = *(int *)(param_1 + 0x34) + -1;
    } while (uVar3 < param_2);
  }
  return *(undefined4 *)(param_1 + 0x34);
}

