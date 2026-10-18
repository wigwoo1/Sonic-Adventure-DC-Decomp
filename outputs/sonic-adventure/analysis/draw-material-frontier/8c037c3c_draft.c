/* Ghidra draft only; not buildable/verified source. */

int FUN_8c037c3c(void)

{
  ushort uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  undefined8 *puVar6;
  undefined8 *puVar7;
  int in_r2;
  ushort *puVar8;
  int unaff_r14;
  undefined8 uVar9;
  undefined8 uVar10;
  undefined8 in_xd0;
  undefined8 in_xd2;
  int in_GBR;
  char in_FPSCR_SZ;
  
  puVar8 = *(ushort **)(in_r2 + 4);
  iVar4 = *(int *)(in_GBR + 0x3c);
  *(int *)(in_GBR + 0x44) = (int)*(short *)(in_r2 + 2);
  do {
    uVar2 = DAT_8c037cc4;
    iVar5 = *(int *)(in_GBR + 0x54);
    if (iVar5 == 0) {
      uVar1 = *puVar8;
      iVar5 = (int)(short)uVar1;
      puVar8 = puVar8 + 1;
      if (iVar5 < 0) {
        iVar5 = (int)(short)(uVar1 & DAT_8c037cc0);
      }
    }
    uVar1 = *puVar8;
    puVar8 = puVar8 + 1;
    puVar6 = (undefined8 *)((short)uVar1 * 0x10 + iVar4);
    if (in_FPSCR_SZ == '\0') {
      uVar9 = CONCAT44(*(undefined4 *)puVar6,*(undefined4 *)((int)puVar6 + 4));
      uVar10 = CONCAT44(*(undefined4 *)(puVar6 + 1),*(undefined4 *)((int)puVar6 + 0xc));
    }
    else {
      uVar9 = *puVar6;
      in_xd0 = puVar6[1];
      uVar10 = puVar6[2];
      in_xd2 = puVar6[3];
    }
    iVar5 = iVar5 + -1;
    do {
      if (in_FPSCR_SZ == '\0') {
        *(int *)(unaff_r14 + 0x18) = (int)uVar10;
        *(int *)(unaff_r14 + 0xc) = (int)((ulonglong)uVar10 >> 0x20);
        *(int *)(unaff_r14 + 8) = (int)uVar9;
        puVar6 = (undefined8 *)(unaff_r14 + 4);
        *(int *)puVar6 = (int)((ulonglong)uVar9 >> 0x20);
      }
      else {
        *(undefined8 *)(unaff_r14 + 0x14) = in_xd2;
        *(undefined8 *)(unaff_r14 + 4) = uVar10;
        *(undefined8 *)(unaff_r14 + -4) = in_xd0;
        puVar6 = (undefined8 *)(unaff_r14 + -0xc);
        *puVar6 = uVar9;
      }
      *(undefined4 *)((int)puVar6 + -4) = uVar2;
      uVar3 = DAT_8c037cc8;
      unaff_r14 = (int)puVar6 + 0x1c;
      uVar1 = *puVar8;
      puVar8 = puVar8 + 1;
      puVar7 = (undefined8 *)((short)uVar1 * 0x10 + iVar4);
      if (in_FPSCR_SZ == '\0') {
        uVar9 = CONCAT44(*(undefined4 *)puVar7,*(undefined4 *)((int)puVar7 + 4));
        uVar10 = CONCAT44(*(undefined4 *)(puVar7 + 1),*(undefined4 *)((int)puVar7 + 0xc));
      }
      else {
        uVar9 = *puVar7;
        in_xd0 = puVar7[1];
        uVar10 = puVar7[2];
        in_xd2 = puVar7[3];
      }
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar6 + 0x34) = (int)uVar10;
      *(int *)(puVar6 + 5) = (int)((ulonglong)uVar10 >> 0x20);
      *(int *)((int)puVar6 + 0x24) = (int)uVar9;
      puVar6 = puVar6 + 4;
      *(int *)puVar6 = (int)((ulonglong)uVar9 >> 0x20);
    }
    else {
      puVar6[6] = in_xd2;
      puVar6[4] = uVar10;
      puVar6[3] = in_xd0;
      puVar6 = puVar6 + 2;
      *puVar6 = uVar9;
    }
    *(undefined4 *)((int)puVar6 + -4) = uVar3;
    unaff_r14 = (int)puVar6 + 0x1c;
    iVar5 = *(int *)(in_GBR + 0x44) + -1;
    *(int *)(in_GBR + 0x44) = iVar5;
  } while (iVar5 != 0);
  return unaff_r14;
}

