/* Ghidra draft only; not buildable/verified source. */

void FUN_8c01b436(undefined4 param_1,int param_2,int param_3)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  undefined1 *puVar4;
  double dVar5;
  double in_dr2;
  double dVar6;
  float fVar7;
  double dVar8;
  float fVar9;
  float in_fr15;
  undefined4 in_fr14;
  bool bVar10;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_10 [4];
  undefined1 local_c [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar4 = local_c;
  }
  else {
    puVar4 = local_10;
  }
  *(undefined4 *)(puVar4 + -4) = in_PR;
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 0x20));
    dVar6 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x20),*(undefined4 *)(param_2 + 0x24));
  }
  else {
    dVar6 = *(double *)(param_2 + 0x20);
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((float)((ulonglong)dVar6 >> 0x20) - SUB84(in_dr2,0),SUB84(dVar6,0));
  }
  else {
    dVar6 = dVar6 - in_dr2;
  }
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 0x24));
  }
  if (in_FPSCR_PR == '\0') {
    dVar6 = (double)CONCAT44((int)((ulonglong)dVar6 >> 0x20),SUB84(dVar6,0) - SUB84(in_dr2,0));
  }
  else {
    dVar6 = dVar6 - in_dr2;
  }
  fVar7 = (float)((ulonglong)dVar6 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(param_3 + 0x28));
    dVar8 = (double)CONCAT44(*(undefined4 *)(param_2 + 0x28),param_1);
    dVar5 = (double)((ulonglong)(uint)fVar7 << 0x20);
  }
  else {
    dVar8 = *(double *)(param_2 + 0x28);
    dVar5 = dVar6;
  }
  if (in_FPSCR_PR == '\0') {
    fVar9 = (float)((ulonglong)dVar8 >> 0x20) - SUB84(in_dr2,0);
  }
  else {
    fVar9 = (float)((ulonglong)(dVar8 - in_dr2) >> 0x20);
  }
  if (in_FPSCR_SZ == '\0') {
    dVar8 = (double)CONCAT44(DAT_8c01b4f0,SUB84(dVar6,0));
  }
  else {
    dVar8 = (double)CONCAT44(PTR_FUN_8c01b4f4,DAT_8c01b4f0);
  }
  if (in_FPSCR_PR == '\0') {
    dVar8 = (double)CONCAT44((int)((ulonglong)dVar8 >> 0x20),
                             fVar9 * fVar9 +
                             (float)((ulonglong)dVar5 >> 0x20) * fVar7 +
                             SUB84(dVar8,0) * SUB84(dVar6,0));
  }
  else {
    dVar8 = dVar8 * dVar6;
  }
  if (in_FPSCR_SZ == '\0') {
    in_fr15 = SUB84(dVar8,0);
  }
  if (in_FPSCR_PR == '\0') {
    bVar10 = in_fr15 < (float)((ulonglong)dVar8 >> 0x20);
  }
  else {
    bVar10 = (double)CONCAT44(in_fr14,in_fr15) < dVar8;
  }
  if ((((bVar10) &&
       (iVar2 = (*(code *)PTR_FUN_8c01b4f4)
                          (*(undefined4 *)(param_2 + 0x14),*(undefined4 *)(param_3 + 0x14)),
       iVar2 < 0x5b)) &&
      (iVar2 = (*(code *)PTR_FUN_8c01b4f4)
                         (*(undefined4 *)(param_2 + 0x18),*(undefined4 *)(param_3 + 0x18)),
      iVar2 < 0x5b)) &&
     (iVar2 = (*(code *)PTR_FUN_8c01b4f4)
                        (*(undefined4 *)(param_2 + 0x1c),*(undefined4 *)(param_3 + 0x1c)),
     iVar2 < 0x5b)) {
    cVar1 = *(char *)(*(int *)PTR_DAT_8c01b4e0 + 9);
    if (cVar1 == '\x01') {
      uVar3 = 3;
    }
    else {
      if (((cVar1 != '\0') && (cVar1 != '\x02')) && ((cVar1 != '\x03' && (cVar1 != '\x04')))) {
        return;
      }
      uVar3 = 0;
    }
    FUN_8c01b3e0(uVar3);
    return;
  }
  return;
}

