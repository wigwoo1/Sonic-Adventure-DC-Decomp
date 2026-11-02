/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c01a12c) */

void FUN_8c01a100(double param_1,undefined4 param_2,int param_3)

{
  undefined *puVar1;
  uint uVar2;
  int iVar3;
  float extraout_fr3;
  float fVar4;
  undefined4 extraout_fr2;
  float fVar5;
  undefined8 uVar6;
  double dVar7;
  ulonglong uVar8;
  double dVar9;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  
  if (*DAT_8c01a200 == 0) {
    (*(code *)PTR_FUN_8c01a21c)(DAT_8c01a218,*(undefined4 *)(param_3 + 0x1c));
    (*(code *)PTR_FUN_8c01a220)(DAT_8c01a218,*(undefined4 *)(param_3 + 0x14));
    (*(code *)PTR_FUN_8c01a224)(DAT_8c01a218,*(undefined4 *)(param_3 + 0x18));
    puVar1 = PTR_DAT_8c01a228;
    iVar3 = *(int *)PTR_DAT_8c01a228;
    if (in_FPSCR_SZ == '\0') {
      fVar4 = *(float *)(iVar3 + 0x9c);
      dVar9 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x28),param_2);
    }
    else {
      dVar9 = *(double *)(param_3 + 0x28);
      fVar4 = extraout_fr3;
    }
    if (in_FPSCR_PR == '\0') {
      dVar9 = (double)CONCAT44((float)((ulonglong)dVar9 >> 0x20) + fVar4,SUB84(dVar9,0));
    }
    else {
      dVar9 = dVar9 + (double)CONCAT44(extraout_fr2,fVar4);
    }
    if (in_FPSCR_SZ == '\0') {
      fVar4 = *(float *)(iVar3 + 0x98);
      param_1 = (double)CONCAT44((int)((ulonglong)param_1 >> 0x20),*(undefined4 *)(param_3 + 0x24));
    }
    if (in_FPSCR_PR == '\0') {
      fVar5 = SUB84(param_1,0) + fVar4;
    }
    else {
      fVar5 = SUB84(param_1 + (double)CONCAT44(extraout_fr2,fVar4),0);
    }
    if (in_FPSCR_SZ == '\0') {
      fVar4 = *(float *)(iVar3 + 0x94);
      dVar7 = (double)CONCAT44(*(undefined4 *)(param_3 + 0x20),fVar5);
    }
    else {
      dVar7 = *(double *)(param_3 + 0x20);
    }
    if (in_FPSCR_PR == '\0') {
      uVar2 = SUB84(dVar7,0);
    }
    else {
      uVar2 = SUB84(dVar7 + (double)CONCAT44(extraout_fr2,fVar4),0);
    }
    (*(code *)PTR_FUN_8c01a22c)(dVar9,DAT_8c01a218);
    (*(code *)PTR_FUN_8c01a230)(DAT_8c01a218);
    uVar8 = (ulonglong)uVar2;
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar1 + 0x94) = 0;
    }
    else {
      *(ulonglong *)(*(int *)puVar1 + 0x94) = uVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar1 + 0x98) = 0;
    }
    else {
      *(ulonglong *)(*(int *)puVar1 + 0x98) = uVar8;
    }
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)(*(int *)puVar1 + 0x9c) = 0;
    }
    else {
      *(ulonglong *)(*(int *)puVar1 + 0x9c) = uVar8;
    }
  }
  else {
    if (in_FPSCR_SZ == '\0') {
      uVar6 = CONCAT44(*(undefined4 *)DAT_8c01a204,SUB84(param_1,0));
    }
    else {
      uVar6 = *DAT_8c01a204;
    }
    (*(code *)PTR_FUN_8c01a20c)(uVar6,*DAT_8c01a208);
    uVar2 = (*(code *)PTR_FUN_8c01a210)();
    if ((uVar2 & 0xffff) >> 8 == 0x20) {
      *(undefined4 *)(*(int *)PTR_DAT_8c01a214 + 0x18) = 0;
    }
  }
                    /* WARNING: Could not recover jumptable at 0x8c01a1a8. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*DAT_8c01a234)();
  return;
}

