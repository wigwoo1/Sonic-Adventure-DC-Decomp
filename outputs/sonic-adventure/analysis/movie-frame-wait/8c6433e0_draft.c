/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6433e0(void)

{
  undefined *puVar1;
  uint uVar2;
  int iVar3;
  
  puVar1 = PTR_FUN_8c643494;
  if (*DAT_8c64348c == 1) {
    uVar2 = *DAT_8c643490;
    (*(code *)PTR_FUN_8c643494)(8,1);
    (*(code *)puVar1)(8,0);
    iVar3 = (uVar2 & 1) * 0x20;
    (*(code *)puVar1)((int)DAT_8c643480,*(undefined4 *)(DAT_8c643498 + iVar3));
    (*(code *)puVar1)((int)DAT_8c643482,*(undefined4 *)(DAT_8c64349c + iVar3));
    (*(code *)puVar1)((int)DAT_8c643484,*(undefined4 *)(DAT_8c6434a0 + iVar3));
    (*(code *)puVar1)((int)DAT_8c643486,*(undefined4 *)(DAT_8c6434a4 + iVar3));
    (*(code *)puVar1)((int)DAT_8c643488,*(undefined4 *)(DAT_8c6434a8 + iVar3));
    (*(code *)puVar1)((int)DAT_8c64348a,DAT_8c6434ac);
    (*(code *)PTR_FUN_8c6434b0)((int)DAT_8c64348a);
    *DAT_8c6434b4 = 0;
    *DAT_8c6434bc = *DAT_8c6434b8;
  }
  (*(code *)PTR_FUN_8c6434c4)(*(undefined4 *)PTR_DAT_8c6434c0);
  return;
}

