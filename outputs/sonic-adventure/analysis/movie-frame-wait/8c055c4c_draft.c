/* Ghidra draft only; not buildable/verified source. */

void FUN_8c055c4c(void)

{
  int *piVar1;
  undefined *puVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  undefined1 auStack_4c [48];
  
  uVar3 = DAT_8c055d14;
  puVar2 = PTR_FUN_8c055d10;
  piVar1 = DAT_8c055d08;
  if (*DAT_8c055d08 != 0) {
    iVar4 = 0;
    iVar5 = *DAT_8c055d0c;
    if (0 < *DAT_8c055d08) {
      do {
        if (*(int *)(iVar5 + 0x28) != 0) {
          (*(code *)PTR_FUN_8c055d04)(iVar5 + 0x28);
        }
        (*(code *)PTR_FUN_8c055d18)
                  (auStack_4c,*(undefined4 *)(iVar5 + 0x20),0x900,*(undefined2 *)(iVar5 + 10),
                   *(undefined2 *)(iVar5 + 0xc));
        (*(code *)PTR_FUN_8c055d1c)(iVar5 + 0x30,auStack_4c,*(undefined4 *)(iVar5 + 0x24),uVar3);
        *(int *)(iVar5 + 0x28) = iVar5 + 0x30;
        *(undefined4 *)(iVar5 + 0x2c) = 1;
        (*(code *)puVar2)(iVar5 + 0x28);
        iVar4 = iVar4 + 1;
        iVar5 = iVar5 + 0x3c;
      } while (iVar4 < *piVar1);
    }
    *piVar1 = 0;
  }
  return;
}

