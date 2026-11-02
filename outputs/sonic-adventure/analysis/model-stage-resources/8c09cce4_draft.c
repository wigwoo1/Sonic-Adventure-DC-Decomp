/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09cce4(undefined4 param_1,undefined4 param_2)

{
  undefined *puVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 auStack_30 [36];
  undefined1 *local_c;
  
  local_c = &DAT_0c900000;
  if (*(int *)PTR_DAT_8c09ce1c == 0) {
    iVar2 = (*(code *)PTR_FUN_8c09ce24)();
    uVar3 = *(undefined4 *)(PTR_PTR_8c09ce28 + iVar2 * 4);
  }
  else {
    uVar3 = *(undefined4 *)PTR_PTR_8c09ce20;
  }
  (*(code *)PTR_FUN_8c09ce30)(auStack_30,PTR_DAT_8c09ce2c);
  puVar1 = PTR_FUN_8c09ce34;
  (*(code *)PTR_FUN_8c09ce34)(auStack_30,param_1);
  (*(code *)puVar1)(auStack_30,uVar3);
  (*(code *)puVar1)(auStack_30,PTR_DAT_8c09ce38);
  iVar2 = FUN_8c09cc80(auStack_30,param_2);
  if (iVar2 == -1) {
    uVar3 = *(undefined4 *)PTR_PTR_8c09ce28;
    (*(code *)PTR_FUN_8c09ce30)(auStack_30,PTR_DAT_8c09ce2c);
    (*(code *)puVar1)(auStack_30,param_1);
    (*(code *)puVar1)(auStack_30,uVar3);
    (*(code *)puVar1)(auStack_30,PTR_DAT_8c09ce38);
    FUN_8c09cc80(auStack_30,param_2);
  }
  return;
}

