/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c609598) */

void FUN_8c60957e(float param_1,undefined4 param_2,undefined4 *param_3)

{
  ushort uVar1;
  ushort *puVar2;
  undefined4 uVar3;
  int *piVar4;
  undefined *puVar5;
  int iVar6;
  int *piVar7;
  undefined4 uVar8;
  ushort *puVar9;
  ushort *puVar10;
  int iVar11;
  float fVar12;
  undefined8 in_dr2;
  double dVar13;
  bool bVar14;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  uVar3 = DAT_8c609698;
  puVar2 = DAT_8c609690;
  uVar8 = *param_3;
  piVar7 = (int *)param_3[1];
  if (piVar7 != (int *)0x0) {
    fVar12 = (float)piVar7[1];
    dVar13 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar12);
    if (piVar7[1] < 0) {
      if (in_FPSCR_PR == '\0') {
        dVar13 = (double)CONCAT44(DAT_8c60968c,fVar12 + DAT_8c60968c);
      }
      else {
        dVar13 = (double)CONCAT44(DAT_8c60968c,fVar12) + (double)CONCAT44(DAT_8c60968c,fVar12);
      }
    }
    if (in_FPSCR_PR == '\0') {
      bVar14 = param_1 < SUB84(dVar13,0);
    }
    else {
      bVar14 = (double)CONCAT44(param_1,param_2) < dVar13;
    }
    if (bVar14) {
      *DAT_8c609690 = *(ushort *)(piVar7 + 2);
      piVar4 = DAT_8c6096a0;
      puVar10 = DAT_8c609694;
      *DAT_8c609694 = *(ushort *)((int)piVar7 + 10) & (ushort)uVar3;
      puVar5 = PTR_FUN_8c6096a4;
      puVar9 = DAT_8c60969c;
      *DAT_8c60969c = (ushort)((*(ushort *)((int)piVar7 + 10) & 0xc0) != 0);
      *piVar4 = *piVar7;
      piVar4[4] = piVar7[1];
      if (in_FPSCR_SZ == '\0') {
        piVar4[3] = (int)param_1;
      }
      else {
        *(ulonglong *)(piVar4 + 3) = CONCAT44(param_1,param_2);
      }
      piVar4[5] = 0;
      piVar4[6] = 0;
      (*(code *)puVar5)(uVar8,&DAT_8c6b706c);
      *DAT_8c6096ac = *(undefined4 *)(PTR_PTR_8c6096a8 + (uint)*puVar9 * 4);
      uVar1 = *puVar10;
      iVar11 = *piVar4;
      if (uVar1 == 1) {
        piVar4[1] = iVar11;
        piVar4[2] = iVar11 + 4;
      }
      else {
        if (uVar1 == 2) {
          piVar4[1] = iVar11;
          iVar6 = iVar11 + 8;
        }
        else if (uVar1 == 3) {
          piVar4[1] = iVar11;
          iVar6 = iVar11 + 0xc;
        }
        else if (uVar1 == 4) {
          piVar4[1] = iVar11;
          iVar6 = iVar11 + 0x10;
        }
        else {
          if (uVar1 != 5) {
            return;
          }
          iVar6 = iVar11 + 0x14;
          piVar4[1] = iVar11;
        }
        piVar4[2] = iVar6;
      }
      if ((*puVar2 & 1) != 0) {
        FUN_8c609264();
      }
      if ((*puVar2 & 2) != 0) {
        piVar4[5] = piVar4[5] + 1;
      }
      if ((*puVar2 & 4) != 0) {
        piVar4[5] = piVar4[5] + 1;
      }
      if ((*puVar2 & 0x40) == 0) {
        if ((*puVar2 & 8) != 0) {
          FUN_8c609326();
        }
      }
      else {
        FUN_8c6093d8();
      }
      if ((*puVar2 & 0x80) != 0) {
        FUN_8c60945e();
      }
      if ((*puVar2 & DAT_8c609722) != 0) {
        FUN_8c6094e0();
      }
      if ((*puVar2 & DAT_8c609724) != 0) {
        piVar4[5] = piVar4[5] + 1;
      }
      if ((*puVar2 & DAT_8c609726) != 0) {
        piVar4[5] = piVar4[5] + 1;
      }
      if (((DAT_8c609728 & *puVar2) != 0) || ((*puVar2 & DAT_8c60972a) != 0)) {
        piVar4[5] = piVar4[5] + 1;
      }
      (*(code *)PTR_FUN_8c609730)(DAT_8c60972c);
      return;
    }
  }
  return;
}

