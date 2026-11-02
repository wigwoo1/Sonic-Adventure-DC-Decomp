/* Ghidra draft only; not buildable/verified source. */

void FUN_8c0528ae(uint param_1,int param_2,int param_3)

{
  short sVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  float fVar5;
  double in_dr2;
  double dVar6;
  float fVar7;
  double dVar8;
  float fVar10;
  double in_xd2;
  bool bVar11;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  float fVar9;
  
  iVar3 = DAT_8c0529f4;
  psVar2 = DAT_8c0529e4;
  if (*DAT_8c0529e4 < 0x100) {
    iVar4 = *(int *)(param_3 + 4);
    if (in_FPSCR_SZ == '\0') {
      fVar9 = *(float *)(iVar4 + 0x18);
      fVar7 = *(float *)(iVar4 + 0x1c);
      dVar8 = (double)((ulonglong)(uint)fVar9 << 0x20);
      in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),fVar7);
      dVar6 = (double)CONCAT44(fVar9,fVar7);
    }
    else {
      dVar8 = *(double *)(iVar4 + 0x18);
      in_xd2 = *(double *)(iVar4 + 0x1c);
      fVar9 = (float)((ulonglong)dVar8 >> 0x20);
      fVar7 = SUB84(dVar8,0);
      dVar6 = dVar8;
    }
    if (in_FPSCR_PR == '\0') {
      fVar5 = SUB84(in_dr2,0) * fVar7;
    }
    else {
      fVar5 = SUB84(in_dr2 * dVar6,0);
    }
    if (in_FPSCR_SZ == '\0') {
      fVar10 = *(float *)(iVar4 + 0x20);
      dVar6 = (double)CONCAT44(DAT_8c0529e8,fVar5);
    }
    else {
      fVar10 = (float)((ulonglong)*(undefined8 *)(iVar4 + 0x20) >> 0x20);
      dVar6 = (double)CONCAT44(DAT_8c0529ec,DAT_8c0529e8);
    }
    if (in_FPSCR_PR == '\0') {
      dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),
                               fVar10 * fVar10 +
                               (float)((ulonglong)dVar8 >> 0x20) * fVar9 + SUB84(dVar6,0));
    }
    if (in_FPSCR_SZ == '\0') {
      in_xd2 = (double)CONCAT44(SUB84(dVar6,0),fVar7);
    }
    if (in_FPSCR_PR == '\0') {
      bVar11 = (float)((ulonglong)in_xd2 >> 0x20) < (float)((ulonglong)dVar6 >> 0x20);
    }
    else {
      bVar11 = in_xd2 < dVar6;
    }
    if ((!bVar11) &&
       (((*(uint *)(param_3 + 0x14) | *(uint *)(param_3 + 0x18) | *(uint *)(param_3 + 0x1c)) &
        DAT_8c0529ec) != 0)) {
      param_1 = param_1 | DAT_8c0529f0;
    }
    *(uint *)(*DAT_8c0529e4 * 0xc + DAT_8c0529f4) = param_1;
    *(int *)(*psVar2 * 0xc + iVar3 + 4) = param_3;
    sVar1 = *psVar2;
    *psVar2 = sVar1 + 1;
    *(int *)(iVar3 + sVar1 * 0xc + 8) = param_2;
    *(ushort *)(*(int *)(param_2 + 0x20) + 4) = *(ushort *)(*(int *)(param_2 + 0x20) + 4) | 0x100;
  }
  return;
}

