/* Ghidra draft only; not buildable/verified source. */

void FUN_8c1087d2(int param_1)

{
  undefined *puVar1;
  undefined1 uVar2;
  
  uVar2 = (*(code *)PTR_FUN_8c1088ec)(*(undefined4 *)(param_1 + 0x630));
  puVar1 = PTR_FUN_8c1088f0;
  *(undefined1 *)(param_1 + 0x600) = uVar2;
  uVar2 = (*(code *)puVar1)(*(undefined4 *)(param_1 + 0x630));
  *(undefined1 *)(param_1 + 0x601) = uVar2;
  return;
}

