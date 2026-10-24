/* Ghidra draft only; not buildable/verified source. */

void FUN_8c665e3c(int param_1,int param_2,int param_3)

{
  undefined *puVar1;
  uint uVar2;
  int iVar3;
  
  puVar1 = PTR_FUN_8c665ef8;
  iVar3 = param_3 * 0x20;
  (*(code *)PTR_FUN_8c665ef8)((int)DAT_8c665ee0,*(undefined4 *)(DAT_8c665efc + iVar3),param_3);
  (*(code *)puVar1)((int)DAT_8c665ee2,*(undefined4 *)(DAT_8c665f00 + iVar3));
  (*(code *)puVar1)((int)DAT_8c665ee4,*(undefined4 *)(DAT_8c665f04 + iVar3));
  (*(code *)puVar1)((int)DAT_8c665ee6,*(undefined4 *)(DAT_8c665f08 + iVar3));
  (*(code *)puVar1)((int)DAT_8c665ee8,(param_2 + -1) * 0x10000 + param_1 + -1);
  uVar2 = *DAT_8c665ef0 & 3 | (*DAT_8c665ef4 & 3) << 4 | (*DAT_8c665f0c & 3) << 8 |
          (*DAT_8c665f10 & 3) << 0xc;
  if (*DAT_8c665f14 == 6) {
    uVar2 = uVar2 | (*DAT_8c665f18 & 1) << 0x14 | (*DAT_8c665f1c & 3) << 0x10;
  }
  else {
    uVar2 = uVar2 | (*DAT_8c665fe8 & 1) << 0x10;
  }
  (*(code *)puVar1)((int)DAT_8c665fe0,uVar2);
  (*(code *)puVar1)((int)DAT_8c665fe2,DAT_8c665fec);
  return;
}

