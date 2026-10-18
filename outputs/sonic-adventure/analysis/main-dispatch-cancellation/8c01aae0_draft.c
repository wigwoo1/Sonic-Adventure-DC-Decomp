/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01aae0(short param_1,byte param_2)

{
  int *piVar1;
  undefined *puVar2;
  undefined *puVar3;
  int *piVar4;
  int iVar5;
  
  puVar3 = PTR_DAT_8c01ac70;
  iVar5 = *(int *)PTR_DAT_8c01ac70;
  if (*(char *)(iVar5 + 8) == '\0') {
    *(int *)(iVar5 + 0x18) = (int)*(char *)(iVar5 + 8);
    *(undefined2 *)(*(int *)puVar3 + 0x30) = *(undefined2 *)(*(int *)puVar3 + 4);
    *(undefined1 *)(*(int *)puVar3 + 0x3c) = *(undefined1 *)(*(int *)puVar3 + 0xb);
    *(undefined1 *)(*(int *)puVar3 + 0x74) = *(undefined1 *)(*(int *)puVar3 + 6);
    *(undefined4 *)(*(int *)puVar3 + 0x44) = *(undefined4 *)(*(int *)puVar3 + 0xc);
    iVar5 = *(int *)puVar3;
    if (*(short *)(iVar5 + 4) == -1) {
      *(undefined1 *)(iVar5 + 0x7a) = 0x15;
    }
    else {
      *(undefined1 *)(iVar5 + 0x7a) =
           *(undefined1 *)(*(short *)(iVar5 + 4) * 0x40 + *DAT_8c01ac74 + 2);
    }
    piVar4 = DAT_8c01ac78;
    piVar1 = DAT_8c01ac60;
    *(undefined4 *)(*(int *)puVar3 + 0x5c) =
         *(undefined4 *)(*(char *)(*(int *)puVar3 + 0x7a) * 8 + *DAT_8c01ac78 + 4);
    *(undefined2 *)(*(int *)puVar3 + 4) = 0xffff;
    *(undefined1 *)(*(int *)puVar3 + 8) = 0;
    *(char *)(*(int *)puVar3 + 6) = (char)param_1;
    *(undefined4 *)(*(int *)puVar3 + 0xc) = *(undefined4 *)(param_1 * 0x10 + *piVar1);
    *(byte *)(*(int *)puVar3 + 9) = param_2;
    puVar2 = PTR_DAT_8c01ac64;
    *(undefined4 *)(*(int *)puVar3 + 0x10) = *(undefined4 *)((uint)param_2 * 8 + *piVar4 + 4);
    *(undefined4 *)(*(int *)puVar2 + 0x14) = 0;
    if (*(int *)(*(char *)(*(int *)puVar3 + 9) * 8 + *piVar4) != 0) {
      *(byte *)(*(int *)puVar3 + 0xb) = *(byte *)(*(int *)puVar3 + 0xb) | 1;
    }
    *(undefined1 *)(*(int *)puVar3 + 7) =
         *(undefined1 *)(*(char *)(*(int *)puVar3 + 6) * 0x10 + *DAT_8c01ac60 + 5);
    puVar2 = PTR_LAB_8c01ac68;
    **(undefined4 **)puVar3 = 1;
    (*(code *)PTR_FUN_8c01ac6c)(8,6,puVar2);
    return;
  }
  return;
}

