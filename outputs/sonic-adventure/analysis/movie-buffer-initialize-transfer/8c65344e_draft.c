/* Ghidra draft only; not buildable/verified source. */

void FUN_8c65344e(uint param_1,int param_2)

{
  undefined *puVar1;
  uint uVar2;
  
  puVar1 = PTR_FUN_8c6534c4;
  uVar2 = param_1 + param_2;
  for (; param_1 <= uVar2; param_1 = param_1 + 0x20) {
    (*(code *)puVar1)(param_1);
  }
  return;
}

