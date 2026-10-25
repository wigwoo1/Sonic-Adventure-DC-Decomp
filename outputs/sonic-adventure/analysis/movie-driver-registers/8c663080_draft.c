/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c663080(void)

{
  undefined *puVar1;
  int iVar2;
  undefined2 *puVar3;
  undefined2 *puVar4;
  
  puVar1 = PTR_DAT_8c663120;
  iVar2 = *(int *)(PTR_DAT_8c663120 + 8);
  while (iVar2 != 0) {
    iVar2 = *(int *)(iVar2 + 8);
    (*(code *)PTR_FUN_8c663124)();
  }
  iVar2 = *(int *)puVar1;
  while (iVar2 != 0) {
    iVar2 = *(int *)(iVar2 + 8);
    (*(code *)PTR_FUN_8c663124)();
  }
  iVar2 = *(int *)(puVar1 + 0x1c);
  while (iVar2 != 0) {
    iVar2 = *(int *)(iVar2 + 8);
    (*(code *)PTR_FUN_8c663124)();
  }
  iVar2 = *(int *)(puVar1 + 0x14);
  puVar3 = DAT_8c663128;
  while (DAT_8c663128 = puVar3, iVar2 != 0) {
    iVar2 = *(int *)(iVar2 + 8);
    (*(code *)PTR_FUN_8c663124)();
    puVar3 = DAT_8c663128;
  }
  puVar4 = (undefined2 *)(DAT_8c66312c + (int)puVar3);
  do {
    *puVar3 = 0;
    puVar3 = puVar3 + 0xc;
  } while (puVar3 < puVar4);
  *(undefined4 *)puVar1 = 0;
  *(undefined4 *)(puVar1 + 4) = 0;
  *(undefined4 *)(puVar1 + 8) = 0;
  *(undefined4 *)(puVar1 + 0xc) = 0;
  *(undefined4 *)(puVar1 + 0x10) = 0;
  *(undefined4 *)(puVar1 + 0x14) = 0;
  *(undefined4 *)(puVar1 + 0x18) = 0;
  *(undefined4 *)(puVar1 + 0x1c) = 0;
  *(undefined4 *)(puVar1 + 0x20) = 0;
  *(undefined4 *)(puVar1 + 0x24) = 0;
  iVar2 = *(int *)PTR_DAT_8c663130;
  while (iVar2 != 0) {
    iVar2 = *(int *)(iVar2 + 4);
    (*(code *)PTR_FUN_8c663134)();
  }
  *(undefined4 *)PTR_DAT_8c663130 = 0;
  return 0;
}

