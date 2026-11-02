/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09cd6e(undefined4 param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 auStack_2c [36];
  
  iVar2 = (*(code *)PTR_FUN_8c09ce24)();
  uVar3 = *(undefined4 *)(PTR_PTR_8c09ce28 + iVar2 * 4);
  (*(code *)PTR_FUN_8c09ce30)(auStack_2c,PTR_DAT_8c09ce3c);
  puVar1 = PTR_FUN_8c09ce34;
  (*(code *)PTR_FUN_8c09ce34)(auStack_2c,param_1);
  (*(code *)puVar1)(auStack_2c,uVar3);
  (*(code *)puVar1)(auStack_2c,PTR_DAT_8c09ce38);
  iVar2 = FUN_8c09cc80(auStack_2c,param_2);
  if (iVar2 == -1) {
    uVar3 = *(undefined4 *)PTR_PTR_8c09ce28;
    (*(code *)PTR_FUN_8c09ce30)(auStack_2c,PTR_DAT_8c09ce3c);
    (*(code *)puVar1)(auStack_2c,param_1);
    (*(code *)puVar1)(auStack_2c,uVar3);
    (*(code *)puVar1)(auStack_2c,PTR_DAT_8c09ce38);
    FUN_8c09cc80(auStack_2c,param_2);
  }
  return;
}

