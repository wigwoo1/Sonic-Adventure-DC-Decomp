/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01b158(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  
  puVar1 = PTR_DAT_8c01b200;
  iVar3 = *(int *)PTR_DAT_8c01b200;
  if (*(char *)(iVar3 + 8) == '\x04') {
    *(undefined2 *)(iVar3 + 4) = *(undefined2 *)(iVar3 + 0x38);
    *(undefined1 *)(*(int *)puVar1 + 0xb) = *(undefined1 *)(*(int *)puVar1 + 0x40);
    *(undefined1 *)(*(int *)puVar1 + 6) = *(undefined1 *)(*(int *)puVar1 + 0x78);
    *(undefined4 *)(*(int *)puVar1 + 0xc) = *(undefined4 *)(*(int *)puVar1 + 0x54);
    *(undefined1 *)(*(int *)puVar1 + 9) = *(undefined1 *)(*(int *)puVar1 + 0x7e);
    *(undefined4 *)(*(int *)puVar1 + 0x10) = *(undefined4 *)(*(int *)puVar1 + 0x6c);
    *(char *)(*(int *)puVar1 + 8) = (char)*(undefined4 *)(*(int *)puVar1 + 0x28);
    puVar2 = PTR_LAB_8c01b214;
    *(undefined1 *)(*(int *)puVar1 + 7) =
         *(undefined1 *)(*(char *)(*(int *)puVar1 + 6) * 0x10 + *DAT_8c01b20c + 5);
    **(undefined4 **)puVar1 = 1;
    *(undefined4 *)(*(int *)puVar1 + 0x14) = 0;
    *(undefined4 *)(*(int *)PTR_DAT_8c01b210 + 0x14) = 0;
    (*(code *)PTR_FUN_8c01b218)(8,6,puVar2);
    if ((*(char *)(*(int *)puVar1 + 8) == '\0') && (*DAT_8c01b21c != -1)) {
      FUN_8c01aae0(*DAT_8c01b21c,*DAT_8c01b220);
      return;
    }
  }
  return;
}

