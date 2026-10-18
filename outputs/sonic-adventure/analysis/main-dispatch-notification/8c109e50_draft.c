/* Ghidra draft only; not buildable/verified source. */

void FUN_8c109e50(undefined4 param_1,undefined1 param_2,undefined1 param_3,undefined1 param_4,
                 undefined1 param_5)

{
  int iVar1;
  undefined1 *puVar2;
  
  iVar1 = (*(code *)PTR_FUN_8c109f4c)(8,0,PTR_LAB_8c109f48);
  if (iVar1 != 0) {
    puVar2 = (undefined1 *)(*(code *)PTR_FUN_8c109f50)(8);
    if (puVar2 == (undefined1 *)0x0) {
      (*(code *)PTR_FUN_8c109f44)(iVar1);
      return;
    }
    *(undefined1 **)(iVar1 + 0x28) = puVar2;
    **(undefined4 **)(iVar1 + 0x2c) = param_1;
    *puVar2 = 1;
    puVar2[1] = param_2;
    puVar2[2] = param_3;
    puVar2[3] = param_4;
    puVar2[4] = param_5;
  }
  return;
}

