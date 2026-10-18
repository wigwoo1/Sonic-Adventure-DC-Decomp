/* Ghidra draft only; not buildable/verified source. */

void FUN_8c037872(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  char *pcVar4;
  uint *puVar5;
  int iVar6;
  undefined4 in_fr1;
  double dVar7;
  float fVar9;
  double dVar8;
  double in_xd0;
  int in_GBR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if ((*(uint *)(in_GBR + 0x2c) & DAT_8c0378e4) != 0) {
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)*(byte *)(param_2 + 3),in_fr1);
    }
    else {
      dVar7 = (double)*(byte *)(param_2 + 3);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)PTR_DAT_8c037964);
    }
    else {
      in_xd0 = *(double *)PTR_DAT_8c037964;
    }
    if (in_FPSCR_PR == '\0') {
      dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) * SUB84(dVar7,0),SUB84(dVar7,0));
    }
    else {
      dVar7 = dVar7 * dVar7;
    }
    iVar2 = *(int *)(in_GBR + 0x28);
    if (iVar2 != 0) {
      if (0 < iVar2) {
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)CONCAT44(*(undefined4 *)DAT_8c037968,SUB84(dVar7,0));
        }
        else {
          dVar7 = *DAT_8c037968;
        }
      }
      if (iVar2 < 0) {
        if (in_FPSCR_SZ == '\0') {
          dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)DAT_8c037968);
        }
        else {
          in_xd0 = *DAT_8c037968;
        }
        if (in_FPSCR_PR == '\0') {
          fVar9 = (float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0);
        }
        else {
          fVar9 = (float)((ulonglong)(dVar7 + dVar7) >> 0x20);
        }
        dVar8 = (double)CONCAT44(fVar9,0x3f800000);
        if ((in_FPSCR_PR == '\0' && 1.0 < fVar9) && (dVar8 = in_xd0, in_FPSCR_SZ == '\0')) {
          dVar8 = 0.0078125;
        }
        dVar7 = (double)((ulonglong)dVar8 & 0xffffffff00000000);
        if ((in_FPSCR_PR != '\0' || (float)((ulonglong)dVar8 >> 0x20) <= 0.0) &&
           (dVar7 = in_xd0, in_FPSCR_SZ == '\0')) {
          dVar7 = 0.0;
        }
      }
    }
    if (in_FPSCR_SZ == '\0') {
      dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)PTR_DAT_8c03796c);
    }
    if (in_FPSCR_PR == '\0') {
      iVar2 = (int)((float)((ulonglong)dVar7 >> 0x20) * SUB84(dVar7,0));
    }
    else {
      iVar2 = (int)(dVar7 * dVar7);
    }
    uVar3 = iVar2 << 0x18 | DAT_8c037970;
    iVar2 = *(int *)(param_1 + 8);
    puVar5 = (uint *)(*(int *)(in_GBR + 0x3c) + 0xc);
    if (uVar3 != *puVar5) {
      do {
        *puVar5 = uVar3;
        puVar5[4] = uVar3;
        puVar5[8] = uVar3;
        puVar5[0xc] = uVar3;
        puVar5 = puVar5 + 0x10;
        iVar2 = iVar2 + -4;
      } while (0 < iVar2);
    }
    return;
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)*(byte *)(param_2 + 3),in_fr1);
  }
  else {
    dVar7 = (double)*(byte *)(param_2 + 3);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)PTR_DAT_8c0378e8);
  }
  else {
    in_xd0 = *(double *)PTR_DAT_8c0378e8;
  }
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44((float)((ulonglong)dVar7 >> 0x20) * SUB84(dVar7,0),SUB84(dVar7,0));
  }
  else {
    dVar7 = dVar7 * dVar7;
  }
  iVar2 = *(int *)(in_GBR + 0x28);
  if (iVar2 != 0) {
    if (0 < iVar2) {
      if (in_FPSCR_SZ == '\0') {
        dVar7 = (double)CONCAT44(*(undefined4 *)DAT_8c0378ec,SUB84(dVar7,0));
      }
      else {
        dVar7 = *DAT_8c0378ec;
      }
    }
    if (iVar2 < 0) {
      if (in_FPSCR_SZ == '\0') {
        dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)DAT_8c0378ec);
      }
      else {
        in_xd0 = *DAT_8c0378ec;
      }
      if (in_FPSCR_PR == '\0') {
        fVar9 = (float)((ulonglong)dVar7 >> 0x20) + SUB84(dVar7,0);
      }
      else {
        fVar9 = (float)((ulonglong)(dVar7 + dVar7) >> 0x20);
      }
      dVar8 = (double)CONCAT44(fVar9,0x3f800000);
      if ((in_FPSCR_PR == '\0' && 1.0 < fVar9) && (dVar8 = in_xd0, in_FPSCR_SZ == '\0')) {
        dVar8 = 0.0078125;
      }
      dVar7 = (double)((ulonglong)dVar8 & 0xffffffff00000000);
      if ((in_FPSCR_PR != '\0' || (float)((ulonglong)dVar8 >> 0x20) <= 0.0) &&
         (dVar7 = in_xd0, in_FPSCR_SZ == '\0')) {
        dVar7 = 0.0;
      }
    }
  }
  if (in_FPSCR_SZ == '\0') {
    dVar7 = (double)CONCAT44((int)((ulonglong)dVar7 >> 0x20),*(undefined4 *)PTR_DAT_8c0378f0);
  }
  if (in_FPSCR_PR == '\0') {
    iVar2 = (int)((float)((ulonglong)dVar7 >> 0x20) * SUB84(dVar7,0));
  }
  else {
    iVar2 = (int)(dVar7 * dVar7);
  }
  iVar6 = *(int *)(param_1 + 8);
  pcVar4 = (char *)(*(int *)(in_GBR + 0x3c) + 0xf);
  if (iVar2 != *pcVar4) {
    do {
      cVar1 = (char)iVar2;
      *pcVar4 = cVar1;
      pcVar4[0x10] = cVar1;
      pcVar4[0x20] = cVar1;
      pcVar4[0x30] = cVar1;
      pcVar4 = pcVar4 + 0x40;
      iVar6 = iVar6 + -4;
    } while (0 < iVar6);
  }
  return;
}

