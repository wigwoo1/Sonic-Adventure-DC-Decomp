/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c666758) */
/* WARNING: Removing unreachable block (ram,0x8c666764) */

void FUN_8c666706(uint param_1)

{
  uint *puVar1;
  uint *puVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  bool bVar11;
  
  *DAT_8c66689c = param_1;
  puVar1 = DAT_8c666898;
  bVar11 = (param_1 & 1) == 0;
  *DAT_8c666898 = (DAT_8c6668a0 & param_1) >> 0x10;
  puVar2 = DAT_8c6668a8;
  *DAT_8c6668a8 = (DAT_8c6668a4 & param_1) >> 0x11;
  puVar1[2] = (DAT_8c6668ac & param_1) >> 0x13;
  puVar2[2] = (DAT_8c666890 & param_1) >> 0x15;
  puVar1[4] = (DAT_8c6668b0 & param_1) >> 0x17;
  puVar2[4] = (param_1 & DAT_8c6668b4) >> 0x19;
  puVar1[-0x18] = (puVar1[-0x15] >> 2) * 3;
  iVar4 = FUN_8c665e00(puVar2[-0x15] * *DAT_8c6668b8,0);
  iVar6 = 0;
  iVar9 = 0;
  iVar7 = 0;
  iVar8 = 0;
  do {
    iVar10 = iVar9 * 0x20;
    *(int *)(DAT_8c6668c0 + 4 + iVar10) = *(int *)(DAT_8c6668c0 + iVar10) + *DAT_8c6668bc;
    iVar3 = DAT_8c6668cc;
    if ((*DAT_8c6668c4 & 0x20) == 0) {
      *(int *)(DAT_8c6668cc + iVar10) =
           *(int *)(DAT_8c6668cc + iVar10) + *DAT_8c6668d0 * *(int *)(DAT_8c6668cc + -0x10) * -0x18;
    }
    else {
      if (*DAT_8c6668c8 == 0) {
        iVar5 = *(int *)(DAT_8c6668cc + iVar10) +
                *DAT_8c6668d0 * *(int *)(DAT_8c6668cc + -0x10) * -0x30;
      }
      else {
        *(int *)(DAT_8c6668cc + iVar10) = *(int *)(DAT_8c6668cc + iVar10) + -0x44;
        iVar5 = *(int *)(iVar3 + iVar10) + *DAT_8c6668d0 * *DAT_8c6668b8 * -0x60;
      }
      *(int *)(iVar3 + iVar10) = iVar5;
    }
    iVar3 = DAT_8c6669a4;
    if (*DAT_8c666898 == 0) {
      *(uint *)(DAT_8c6668d8 + iVar6) = *(int *)(DAT_8c6668c0 + iVar6) + *DAT_8c6668d4 & 0xffffffe0;
      iVar3 = DAT_8c6668cc;
      *(uint *)(DAT_8c6668cc + 0xc + iVar6) = *(int *)(DAT_8c6668cc + iVar6) - iVar4 & 0xffffff80;
      *(int *)(iVar3 + 0x10 + iVar6) = *(int *)(iVar3 + 8 + iVar6) + iVar4;
    }
    else {
      *(uint *)(DAT_8c6669a4 + 8 + iVar7) = *(int *)(DAT_8c6669a4 + iVar7) - iVar4 & 0xffffff80;
      *(uint *)(iVar3 + 0xc + iVar7) = *(int *)(iVar3 + -4 + iVar7) + *DAT_8c6669a8 & 0xffffffe0;
      *(undefined4 *)(iVar3 + 0x10 + iVar7) = *(undefined4 *)(iVar3 + 8 + iVar7);
    }
    iVar3 = DAT_8c6669ac;
    *(undefined4 *)(DAT_8c6669ac + 0x18 + iVar8) = *(undefined4 *)(DAT_8c6669ac + iVar8);
    *(int *)(iVar3 + 0x1c + iVar8) = *(int *)(iVar3 + iVar8) + *DAT_8c6669a8 + (int)DAT_8c6669a0;
    iVar9 = iVar9 + 1;
    iVar7 = iVar7 + 0x20;
    iVar8 = iVar8 + 0x20;
    iVar6 = iVar6 + 0x20;
  } while (iVar9 < 2);
  FUN_8c665fa8(*DAT_8c6669b4,*DAT_8c6669b0,0,1,0,bVar11);
  FUN_8c665fa8(*DAT_8c6669b4,*DAT_8c6669b0,0,1,1,bVar11);
  return;
}

