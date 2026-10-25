/* Ghidra draft only; not buildable/verified source. */

void FUN_8c092fa0(void)

{
  undefined *puVar1;
  undefined4 *puVar2;
  uint uVar3;
  uint uVar4;
  
  puVar1 = PTR_FUN_8c0930e4;
  *(undefined4 *)PTR_DAT_8c0930e0 = 0;
  (*(code *)puVar1)();
  FUN_8c093c74();
  FUN_8c092fe8();
  (*(code *)PTR_FUN_8c0930e8)();
  uVar3 = DAT_8c0930f4;
  puVar2 = DAT_8c0930f0;
  uVar4 = DAT_8c0930f4 + 0x7b8;
  *DAT_8c0930ec = 0;
  *puVar2 = 0x20;
  for (; uVar3 < uVar4; uVar3 = uVar3 + 0x68) {
    *(undefined4 *)(uVar3 + 4) = 0;
    *(undefined4 *)(uVar3 + 0x38) = 0;
  }
  FUN_8c09306c();
  return;
}

