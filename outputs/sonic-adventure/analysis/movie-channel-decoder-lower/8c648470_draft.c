/* Ghidra draft only; not buildable/verified source. */

int FUN_8c648470(int param_1)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  puVar1 = PTR_DAT_8c6485b8;
  iVar4 = param_1 * DAT_8c648490 + DAT_8c648494;
  if (*(char *)(iVar4 + 2) == '\0') {
    return (int)DAT_8c648492;
  }
  *(undefined4 *)(iVar4 + 0x30) = 1;
  iVar2 = DAT_8c6485bc;
  *(undefined4 *)(iVar4 + 0x34) = *(undefined4 *)(puVar1 + *(int *)(iVar4 + 4) * 0x50);
  puVar1 = PTR_FUN_8c6485c0;
  iVar3 = *(int *)(iVar4 + 0x30);
  *(int *)(iVar4 + 0x30) = iVar3 + 1;
  (*(code *)puVar1)(param_1,*(undefined4 *)(param_1 * 0x80 + iVar2 + 8),iVar3,
                    *(undefined4 *)(iVar4 + 0x34));
  iVar4 = (*(code *)PTR_FUN_8c6485c4)(param_1);
  return iVar4;
}

