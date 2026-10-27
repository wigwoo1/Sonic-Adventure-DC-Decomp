/* Ghidra draft only; not buildable/verified source. */

int FUN_8c6482a0(int param_1)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  puVar1 = PTR_DAT_8c648374;
  iVar4 = param_1 * DAT_8c648360 + DAT_8c648368;
  if (*(int *)(*DAT_8c648370 + 8) == 1) {
    iVar4 = FUN_8c648470(param_1);
    return iVar4;
  }
  if (*(char *)(iVar4 + 2) != '\0') {
    *(undefined4 *)(iVar4 + 0x30) = 1;
    iVar2 = DAT_8c648378;
    *(undefined4 *)(iVar4 + 0x34) = *(undefined4 *)(puVar1 + *(int *)(iVar4 + 4) * 0x50);
    puVar1 = PTR_FUN_8c64837c;
    iVar3 = *(int *)(iVar4 + 0x30);
    *(int *)(iVar4 + 0x30) = iVar3 + 1;
    (*(code *)puVar1)(param_1,*(undefined4 *)(param_1 * 0x80 + iVar2 + 8),iVar3,
                      *(undefined4 *)(iVar4 + 0x34));
    iVar2 = (*(code *)PTR_FUN_8c648380)(param_1,*(undefined4 *)(iVar4 + 0x1c));
    if (-1 < iVar2) {
      iVar2 = (*(code *)PTR_FUN_8c648384)(iVar4);
      if (iVar2 < 0) {
        return -0x7d;
      }
      *(undefined4 *)(*(int *)(iVar4 + 0x1c) + 0x30) =
           *(undefined4 *)(*(int *)(iVar4 + 0x1c) + 0x1c);
      *(undefined1 *)(iVar4 + 9) = 1;
      *(undefined1 *)(iVar4 + 8) = 1;
      *(undefined1 *)(iVar4 + 10) = 0;
      *(undefined1 *)(iVar4 + 0xb) = 0;
      *(undefined1 *)(iVar4 + 0xc) = 0;
      iVar4 = (*(code *)PTR_FUN_8c648388)(param_1);
      return iVar4;
    }
    *(undefined1 *)(iVar4 + 9) = 0;
    *(undefined1 *)(iVar4 + 8) = 0;
  }
  return (int)DAT_8c648362;
}

