/* Ghidra draft only; not buildable/verified source. */

void FUN_8c09844a(void)

{
  int iVar1;
  int *piVar2;
  undefined4 *puVar3;
  int *piVar4;
  int *piVar5;
  int *piVar6;
  undefined4 *puVar7;
  
  iVar1 = FUN_8c098a80(DAT_8c0985a4,&DAT_8c19de24);
  piVar6 = DAT_8c0985ac;
  *DAT_8c0985a8 = iVar1;
  *piVar6 = iVar1;
  if (iVar1 != 0) {
    puVar7 = DAT_8c0985b0 + 8;
    for (puVar3 = DAT_8c0985b0; puVar3 < puVar7; puVar3 = puVar3 + 2) {
      *puVar3 = 0;
      puVar3[1] = 0;
    }
    piVar6 = (int *)*piVar6;
    piVar2 = (int *)0x0;
    iVar1 = 0;
    do {
      piVar4 = piVar6 + 0xc;
      if (0xffe < iVar1) {
        piVar4 = (int *)0x0;
      }
      *piVar6 = (int)piVar4;
      piVar6[1] = (int)piVar2;
      piVar6[7] = 0;
      piVar6[8] = 0;
      piVar6[9] = 0;
      piVar6[10] = 0;
      piVar6[0xb] = 0;
      piVar6[3] = 0;
      piVar6[2] = 0;
      piVar6[4] = 0;
      piVar6[5] = 0;
      piVar6[6] = 0;
      piVar5 = piVar4 + 0xc;
      if (0xffe < iVar1 + 1) {
        piVar5 = (int *)0x0;
      }
      *piVar4 = (int)piVar5;
      piVar4[1] = (int)piVar6;
      piVar4[7] = 0;
      piVar4[8] = 0;
      piVar4[9] = 0;
      piVar4[10] = 0;
      piVar4[0xb] = 0;
      piVar4[3] = 0;
      piVar4[2] = 0;
      piVar4[4] = 0;
      piVar4[5] = 0;
      piVar4[6] = 0;
      piVar2 = piVar5 + 0xc;
      if (0xffe < iVar1 + 2) {
        piVar2 = (int *)0x0;
      }
      *piVar5 = (int)piVar2;
      piVar5[1] = (int)piVar4;
      piVar5[7] = 0;
      piVar5[8] = 0;
      piVar5[9] = 0;
      piVar5[10] = 0;
      piVar5[0xb] = 0;
      piVar5[3] = 0;
      piVar5[2] = 0;
      piVar5[4] = 0;
      piVar5[5] = 0;
      piVar5[6] = 0;
      piVar6 = piVar2 + 0xc;
      if (0xffe < iVar1 + 3) {
        piVar6 = (int *)0x0;
      }
      iVar1 = iVar1 + 4;
      *piVar2 = (int)piVar6;
      piVar2[1] = (int)piVar5;
      piVar2[7] = 0;
      piVar2[8] = 0;
      piVar2[9] = 0;
      piVar2[10] = 0;
      piVar2[0xb] = 0;
      piVar2[3] = 0;
      piVar2[2] = 0;
      piVar2[4] = 0;
      piVar2[5] = 0;
      piVar2[6] = 0;
    } while (iVar1 < 0x1000);
  }
  return;
}

