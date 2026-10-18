/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09878c(int param_1)

{
  undefined *puVar1;
  int *piVar2;
  code *pcVar3;
  undefined4 uVar4;
  
  puVar1 = PTR_DAT_8c0987ec;
  uVar4 = 0;
  if (param_1 == 8) {
    param_1 = 7;
  }
  piVar2 = *(int **)(DAT_8c0987e0 + param_1 * 4);
  while (piVar2 != (int *)0x0) {
    pcVar3 = (code *)piVar2[4];
    piVar2 = (int *)*piVar2;
    *(code **)puVar1 = pcVar3;
    if (pcVar3 != (code *)0x0) {
      (*pcVar3)(uVar4);
    }
    *(undefined4 *)puVar1 = 0;
  }
  return;
}

