/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Restarted to delay deadcode elimination for space: register */

void FUN_8c038c24(void)

{
  int iVar1;
  undefined8 *puVar2;
  undefined8 *puVar3;
  int iVar4;
  float fVar5;
  undefined8 uVar7;
  float fVar8;
  float fVar10;
  undefined8 uVar9;
  undefined8 in_xd0;
  undefined8 in_xd2;
  float in_xf4;
  float in_xf5;
  float in_xf6;
  float in_xf7;
  float in_xf8;
  float in_xf9;
  float in_xf10;
  float in_xf11;
  float in_xf12;
  float in_xf13;
  float in_xf14;
  float in_xf15;
  int in_GBR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar6;
  
  puVar2 = DAT_8c038c64;
  iVar1 = *(int *)PTR_DAT_8c038c68;
  *(undefined4 *)PTR_DAT_8c038c68 = 0;
  *(int *)(in_GBR + 0x8c) = iVar1;
  if (iVar1 != 0) {
    iVar4 = DAT_8c038c6c;
    if (0x10 < iVar1) {
      iVar1 = 0x10;
    }
    do {
      if (in_FPSCR_SZ == '\0') {
        uVar7 = CONCAT44(*(undefined4 *)puVar2,*(undefined4 *)((int)puVar2 + 4));
        fVar10 = *(float *)(puVar2 + 1);
        puVar2 = (undefined8 *)((int)puVar2 + 0xc);
      }
      else {
        uVar7 = *puVar2;
        in_xd0 = puVar2[1];
        fVar10 = (float)((ulonglong)puVar2[2] >> 0x20);
        puVar2 = puVar2 + 3;
      }
      uVar9 = CONCAT44(fVar10,0x3f800000);
                    /* WARNING: Ignoring partial resolution of indirect */
      if (in_FPSCR_PR == '\0') {
        fVar6 = (float)uVar7;
        fVar8 = (float)((ulonglong)uVar7 >> 0x20);
        fVar5 = (float)in_xd0 * fVar6 + in_xf4 * fVar8 + in_xf8 * 1.0 + in_xf12 * fVar10;
        fVar5 = (float)((ulonglong)in_xd0 >> 0x20) * fVar6 + in_xf5 * fVar8 + in_xf9 * 1.0 +
                in_xf13 * fVar10;
        fVar5 = (float)in_xd2 * fVar6 + in_xf6 * fVar8 + in_xf10 * 1.0 + in_xf14 * fVar10;
        fVar10 = (float)((ulonglong)in_xd2 >> 0x20) * fVar6 + in_xf7 * fVar8 + in_xf11 * 1.0 +
                 in_xf15 * fVar10;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar9 = CONCAT44((int)((ulonglong)uVar9 >> 0x20),*(undefined4 *)puVar2);
        puVar3 = (undefined8 *)((int)puVar2 + 4);
      }
      else {
        in_xd2 = *puVar2;
        puVar3 = puVar2 + 1;
      }
      puVar2 = puVar3 + 1;
      *(undefined4 *)(iVar4 + -4) = *(undefined4 *)puVar3;
      if (in_FPSCR_SZ == '\0') {
        *(int *)(iVar4 + -8) = (int)uVar9;
        *(int *)(iVar4 + -0xc) = (int)((ulonglong)uVar9 >> 0x20);
        *(int *)(iVar4 + -0x10) = (int)uVar7;
        puVar3 = (undefined8 *)(iVar4 + -0x14);
        *(int *)puVar3 = (int)((ulonglong)uVar7 >> 0x20);
      }
      else {
        *(undefined8 *)(iVar4 + -0xc) = in_xd2;
        *(undefined8 *)(iVar4 + -0x14) = uVar9;
        *(undefined8 *)(iVar4 + -0x1c) = in_xd0;
        puVar3 = (undefined8 *)(iVar4 + -0x24);
        *puVar3 = uVar7;
      }
      iVar1 = iVar1 + -1;
      iVar4 = (int)puVar3 + -4;
    } while (iVar1 != 0);
  }
  return;
}

