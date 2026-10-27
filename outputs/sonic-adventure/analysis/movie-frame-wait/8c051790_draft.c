/* Ghidra draft only; not buildable/verified source. */

void FUN_8c051790(void)

{
  undefined *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  
  puVar1 = PTR_DAT_8c05189c;
  do {
  } while (0 < *DAT_8c051884);
  *DAT_8c051884 = *DAT_8c051880;
  if (*(int *)puVar1 == 1) {
    (*(code *)PTR_FUN_8c0518a0)();
  }
  else {
    (*(code *)PTR_FUN_8c0518a4)();
    uVar4 = (*(code *)PTR_FUN_8c0518a8)();
    puVar1 = PTR_FUN_8c0518b0;
    *DAT_8c0518ac = uVar4;
    (*(code *)puVar1)();
    puVar3 = DAT_8c0518bc;
    puVar2 = DAT_8c0518b8;
    *DAT_8c0518b4 = 0;
    *puVar2 = 0;
    *puVar3 = 0;
    *DAT_8c0518c0 = *DAT_8c0518c0 + 1;
    (*(code *)PTR_FUN_8c0518c4)();
  }
  (*(code *)PTR_FUN_8c0518c8)();
  uVar4 = (*(code *)PTR_FUN_8c0518a8)();
  *DAT_8c0518cc = uVar4;
  return;
}

