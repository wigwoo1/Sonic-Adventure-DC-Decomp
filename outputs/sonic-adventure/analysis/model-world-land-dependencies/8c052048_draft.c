/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c052064) */

void FUN_8c052048(float param_1)

{
  int iVar1;
  double *pdVar2;
  int iVar3;
  float fVar4;
  double in_dr2;
  double dVar5;
  double dVar6;
  bool bVar8;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar7;
  
  iVar3 = *DAT_8c052104;
  if ((*(ushort *)(iVar3 + 4) & 1) != 0) {
    iVar1 = (int)*(short *)(iVar3 + 2);
    pdVar2 = *(double **)(iVar3 + 0x10);
    dVar6 = 1.58735232019473e-314;
    if ((*(ushort *)(iVar3 + 4) & 0x10) == 0) {
      while (0 < iVar1) {
        if (in_FPSCR_SZ == '\0') {
          dVar5 = (double)CONCAT44(*(undefined4 *)pdVar2,SUB84(dVar6,0));
          param_1 = *(float *)((int)pdVar2 + 4);
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(pdVar2 + 1));
        }
        else {
          dVar5 = *pdVar2;
        }
        if (in_FPSCR_PR == '\0') {
          fVar7 = (float)((ulonglong)dVar5 >> 0x20) + param_1;
          dVar6 = (double)CONCAT44(fVar7,SUB84(dVar5,0));
          fVar4 = (SUB84(in_dr2,0) - param_1) + SUB84(dVar5,0);
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar4);
          bVar8 = fVar7 < fVar4;
        }
        else {
          dVar6 = dVar5 + (double)(ulonglong)(uint)param_1;
          in_dr2 = (in_dr2 - (double)(ulonglong)(uint)param_1) + dVar6;
          bVar8 = dVar6 < in_dr2;
        }
        if (!bVar8) {
          if (in_FPSCR_SZ == '\0') {
            dVar6 = (double)((ulonglong)dVar6 & 0xffffffff);
          }
          else {
            dVar6 = (double)(ulonglong)(uint)param_1;
          }
        }
        iVar1 = iVar1 + -1;
        if (in_FPSCR_SZ == '\0') {
          *(int *)pdVar2 = (int)((ulonglong)dVar6 >> 0x20);
        }
        else {
          *pdVar2 = dVar6;
        }
        pdVar2 = pdVar2 + 3;
      }
    }
    else if (0 < iVar1) {
      do {
        if (in_FPSCR_SZ == '\0') {
          dVar5 = (double)CONCAT44(*(undefined4 *)pdVar2,SUB84(dVar6,0));
          param_1 = *(float *)((int)pdVar2 + 4);
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(pdVar2 + 1));
        }
        else {
          dVar5 = *pdVar2;
        }
        if (in_FPSCR_PR == '\0') {
          fVar7 = (float)((ulonglong)dVar5 >> 0x20) + param_1;
          dVar6 = (double)CONCAT44(fVar7,SUB84(dVar5,0));
          fVar4 = (SUB84(in_dr2,0) - param_1) + SUB84(dVar5,0);
          in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar4);
          bVar8 = fVar7 < fVar4;
        }
        else {
          dVar6 = dVar5 + (double)(ulonglong)(uint)param_1;
          in_dr2 = (in_dr2 - (double)(ulonglong)(uint)param_1) + dVar6;
          bVar8 = dVar6 < in_dr2;
        }
        if (!bVar8) {
          if (in_FPSCR_SZ == '\0') {
            dVar6 = (double)((ulonglong)dVar6 & 0xffffffff);
          }
          else {
            dVar6 = (double)(ulonglong)(uint)param_1;
          }
        }
        iVar1 = iVar1 + -1;
        if (in_FPSCR_SZ == '\0') {
          *(int *)pdVar2 = (int)((ulonglong)dVar6 >> 0x20);
        }
        else {
          *pdVar2 = dVar6;
        }
        pdVar2 = pdVar2 + 3;
      } while (0 < iVar1);
      return;
    }
  }
  return;
}

