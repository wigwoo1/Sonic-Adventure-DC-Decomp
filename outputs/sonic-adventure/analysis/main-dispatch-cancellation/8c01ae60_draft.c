/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01ae60(void)

{
  undefined *puVar1;
  undefined *puVar2;
  int iVar3;
  
  puVar1 = PTR_DAT_8c01b000;
  iVar3 = *(int *)PTR_DAT_8c01b000;
  if (*(char *)(iVar3 + 8) == '\x02') {
    *(undefined2 *)(iVar3 + 4) = *(undefined2 *)(iVar3 + 0x34);
    *(undefined1 *)(*(int *)puVar1 + 0xb) = *(undefined1 *)(*(int *)puVar1 + 0x3e);
    *(undefined1 *)(*(int *)puVar1 + 6) = *(undefined1 *)(*(int *)puVar1 + 0x76);
    *(undefined4 *)(*(int *)puVar1 + 0xc) = *(undefined4 *)(*(int *)puVar1 + 0x4c);
    *(undefined1 *)(*(int *)puVar1 + 9) = *(undefined1 *)(*(int *)puVar1 + 0x7c);
    *(undefined4 *)(*(int *)puVar1 + 0x10) = *(undefined4 *)(*(int *)puVar1 + 100);
    *(char *)(*(int *)puVar1 + 8) = (char)*(undefined4 *)(*(int *)puVar1 + 0x20);
    puVar2 = PTR_LAB_8c01b00c;
    *(undefined1 *)(*(int *)puVar1 + 7) =
         *(undefined1 *)(*(char *)(*(int *)puVar1 + 6) * 0x10 + *DAT_8c01b004 + 5);
    **(undefined4 **)puVar1 = 1;
    *(undefined4 *)(*(int *)puVar1 + 0x14) = 0;
    *(undefined4 *)(*(int *)PTR_DAT_8c01b008 + 0x14) = 0;
    (*(code *)PTR_FUN_8c01b010)(8,6,puVar2);
    if ((*(char *)(*(int *)puVar1 + 8) == '\0') && (*DAT_8c01b014 != -1)) {
      FUN_8c01aae0(*DAT_8c01b014,*DAT_8c01b018);
    }
  }
  *DAT_8c01b01c = 0;
  return;
}

