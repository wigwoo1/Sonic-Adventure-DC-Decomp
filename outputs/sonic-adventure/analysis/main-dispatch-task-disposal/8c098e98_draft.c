/* Ghidra draft only; not buildable/verified source. */

void FUN_8c098e98(int param_1)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = (int *)*(int *)(param_1 + 0xc);
  while (piVar1 != (int *)0x0) {
    iVar2 = *piVar1;
    (*(code *)piVar1[4])(piVar1);
    piVar1 = (int *)iVar2;
  }
  return;
}

