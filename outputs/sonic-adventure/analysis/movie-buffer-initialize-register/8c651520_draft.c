/* Ghidra draft only; not buildable/verified source. */

void FUN_8c651520(void)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  uint uVar5;
  
  (*(code *)PTR_FUN_8c651644)(PTR_callback_8c652210_8c651640);
  *DAT_8c651648 = 0;
  *DAT_8c65164c = 0;
  *DAT_8c651650 = 0;
  *DAT_8c651654 = 0;
  puVar4 = DAT_8c651660;
  puVar3 = DAT_8c65165c;
  puVar2 = DAT_8c651658;
  uVar5 = 0;
  *DAT_8c651658 = 0;
  puVar2[1] = 0;
  puVar2[2] = 0;
  puVar2[3] = 0;
  puVar2[4] = 0;
  puVar2[5] = 0;
  puVar2[6] = 0;
  puVar2[7] = 0;
  puVar2[8] = 0;
  *puVar3 = 0;
  *puVar4 = 0;
  do {
    iVar1 = uVar5 + 0x2c;
    uVar5 = uVar5 + 4;
    *(undefined4 *)((int)puVar2 + iVar1) = 0;
  } while (uVar5 < 0x14);
  return;
}

