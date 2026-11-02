/* Ghidra draft only; not buildable/verified source. */

void FUN_8c019dc4(short param_1,char *param_2)

{
  undefined *puVar1;
  undefined *puVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = 0;
  if (*(char *)(*param_2 * 0x10 + *DAT_8c019f24 + 4) == '\x04') {
    if (param_2[3] == '\x01') {
      iVar5 = FUN_8c01a240(param_2);
    }
    piVar3 = DAT_8c019f2c;
    puVar2 = PTR_DAT_8c019f28;
    if (iVar5 != 0) {
      iVar5 = 2;
      iVar4 = *(int *)(*(int *)PTR_DAT_8c019f28 + 0x20);
      while (iVar4 != 0) {
        iVar5 = *(int *)(iVar5 * 4 + *(int *)PTR_DAT_8c019f28 + 0x18);
        iVar4 = *(int *)(iVar5 * 4 + *(int *)PTR_DAT_8c019f28 + 0x18);
      }
      iVar4 = param_1 * 0x40;
      if (*(char *)(*DAT_8c019f2c + iVar4) != '=') {
        *(undefined1 *)(*(int *)PTR_DAT_8c019f28 + 0x74 + iVar5) =
             *(undefined1 *)(*DAT_8c019f2c + iVar4);
      }
      *(undefined1 *)(*(int *)puVar2 + 0x7a + iVar5) = *(undefined1 *)(*piVar3 + iVar4 + 2);
      if (*(char *)(*piVar3 + iVar4) != '=') {
        *(undefined4 *)(*(int *)puVar2 + 0x44 + iVar5 * 4) =
             *(undefined4 *)(*param_2 * 0x10 + *DAT_8c019f24);
      }
      puVar1 = PTR_LAB_8c019f20;
      *(undefined4 *)(iVar5 * 4 + *(int *)puVar2 + 0x5c) =
           *(undefined4 *)((uint)*(byte *)(*piVar3 + iVar4 + 2) * 8 + *DAT_8c019f30 + 4);
      (*(code *)PTR_FUN_8c019f34)(8,6,puVar1);
      return;
    }
  }
  else if ((*DAT_8c019f38 != 0) &&
          (*(char *)(*(short *)(*(int *)PTR_DAT_8c019f28 + 4) * 0x40 + *DAT_8c019f2c + 1) <
           param_2[1])) {
    if (param_2[3] == '\0') {
      iVar5 = FUN_8c01a1ac(param_2);
    }
    else if (param_2[3] == '\x02') {
      iVar5 = FUN_8c01a480(param_2);
    }
    else {
      iVar5 = 0;
    }
    if (iVar5 != 0) {
      (*(code *)PTR_FUN_8c019f3c)();
      FUN_8c01a7d6((int)*param_2,(int)param_1);
      (*(code *)PTR_FUN_8c019f34)(8,6,PTR_LAB_8c019f20);
      return;
    }
  }
  return;
}

