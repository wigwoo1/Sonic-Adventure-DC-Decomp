/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c051ce8) */

void FUN_8c051cc0(void)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  undefined *puVar5;
  undefined2 uVar6;
  int iVar7;
  
  puVar2 = PTR_DAT_8c051dc4;
  puVar1 = PTR_DAT_8c051dc0;
  if (((char *)*DAT_8c051dc8 != (char *)0x0) && (*(char *)*DAT_8c051dc8 == '\0')) {
    uVar6 = (*(code *)PTR_FUN_8c051dcc)();
    *puVar2 = (byte)((ushort)uVar6 >> 8) & (byte)((uint)DAT_8c051dd0 >> 8);
    *puVar1 = (char)uVar6;
  }
  puVar5 = PTR_DAT_8c051de0;
  puVar4 = PTR_DAT_8c051dd8;
  puVar3 = PTR_DAT_8c051dd4;
  *(undefined4 *)PTR_DAT_8c051dd4 = 1;
  *(undefined4 *)puVar4 = 1;
  iVar7 = *(int *)(puVar5 + (char)*puVar1 * 4 + (char)*puVar2 * 0x20);
  *DAT_8c051ddc = iVar7;
  if (iVar7 != 0) {
    (*(code *)PTR_FUN_8c051de4)();
    FUN_8c051a52((int)(char)*puVar2,(int)(char)*puVar1);
    return;
  }
  *(undefined4 *)puVar3 = 0;
  *(undefined4 *)puVar4 = 0;
  return;
}

