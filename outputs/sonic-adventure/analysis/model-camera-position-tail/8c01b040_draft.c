/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01b040(short param_1,byte param_2)

{
  undefined *puVar1;
  int *piVar2;
  int *piVar3;
  undefined *puVar4;
  int iVar5;
  
  piVar2 = DAT_8c01b204;
  puVar1 = PTR_DAT_8c01b200;
  iVar5 = *(int *)PTR_DAT_8c01b200;
  if ('\x04' < *(char *)(iVar5 + 8)) {
    return;
  }
  if (*(char *)(iVar5 + 8) != '\x04') {
    *(int *)(iVar5 + 0x28) = (int)*(char *)(iVar5 + 8);
    *(undefined2 *)(*(int *)puVar1 + 0x38) = *(undefined2 *)(*(int *)puVar1 + 4);
    *(undefined1 *)(*(int *)puVar1 + 0x40) = *(undefined1 *)(*(int *)puVar1 + 0xb);
    *(undefined1 *)(*(int *)puVar1 + 0x78) = *(undefined1 *)(*(int *)puVar1 + 6);
    *(undefined4 *)(*(int *)puVar1 + 0x54) = *(undefined4 *)(*(int *)puVar1 + 0xc);
    if (*(short *)(*(int *)puVar1 + 4) == -1) {
      *(undefined1 *)(*(int *)puVar1 + 0x7e) = 0x15;
    }
    else {
      *(undefined1 *)(*(int *)puVar1 + 0x7e) =
           *(undefined1 *)(*(short *)(*(int *)puVar1 + 4) * 0x40 + *DAT_8c01b208 + 2);
    }
    *(undefined4 *)(*(int *)puVar1 + 0x6c) =
         *(undefined4 *)(*(char *)(*(int *)puVar1 + 0x7e) * 8 + *piVar2 + 4);
    *(undefined1 *)(*(int *)puVar1 + 8) = 4;
  }
  piVar3 = DAT_8c01b20c;
  *(undefined2 *)(*(int *)puVar1 + 4) = 0xffff;
  *(char *)(*(int *)puVar1 + 6) = (char)param_1;
  *(undefined4 *)(*(int *)puVar1 + 0xc) = *(undefined4 *)(param_1 * 0x10 + *piVar3);
  *(byte *)(*(int *)puVar1 + 9) = param_2;
  puVar4 = PTR_DAT_8c01b210;
  *(undefined4 *)(*(int *)puVar1 + 0x10) = *(undefined4 *)((uint)param_2 * 8 + *piVar2 + 4);
  *(undefined4 *)(*(int *)puVar4 + 0x14) = 0;
  *(undefined1 *)(*(int *)puVar1 + 0xb) = 2;
  if (*(int *)(*(char *)(*(int *)puVar1 + 9) * 8 + *piVar2) != 0) {
    *(byte *)(*(int *)puVar1 + 0xb) = *(byte *)(*(int *)puVar1 + 0xb) | 1;
  }
  *(undefined1 *)(*(int *)puVar1 + 7) =
       *(undefined1 *)(*(char *)(*(int *)puVar1 + 6) * 0x10 + *piVar3 + 5);
  puVar4 = PTR_LAB_8c01b214;
  **(undefined4 **)puVar1 = 1;
  (*(code *)PTR_FUN_8c01b218)(8,6,puVar4);
  return;
}

