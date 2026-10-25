/* Ghidra draft only; not buildable/verified source. */

void FUN_8c665a00(void)

{
  undefined *puVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  undefined4 uVar5;
  
  puVar1 = PTR_FUN_8c665a6c;
  uVar5 = 0;
  (*(code *)PTR_FUN_8c665a6c)((int)DAT_8c665a60,DAT_8c665a70,0);
  (*(code *)puVar1)((int)DAT_8c665a62,DAT_8c665a74);
  (*(code *)puVar1)((int)DAT_8c665a64,DAT_8c665a74);
  uVar4 = (uint)DAT_8c665a6a;
  uVar3 = (uint)DAT_8c665a68;
  iVar2 = (int)DAT_8c665a66;
  do {
    (*(code *)puVar1)(iVar2,(uVar4 & 0xff) << 8 | uVar3 & 0xff,uVar5);
    uVar4 = uVar4 - 1;
    iVar2 = iVar2 + 4;
    uVar3 = uVar3 - 1;
  } while (0x7f < (int)uVar4);
  return;
}

