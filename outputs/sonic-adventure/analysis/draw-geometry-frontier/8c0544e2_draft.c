/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_8c0544e2(undefined4 param_1)

{
  int iVar1;
  undefined *puVar2;
  float fVar3;
  double in_dr2;
  undefined4 uVar4;
  double dVar5;
  undefined8 uVar6;
  undefined8 in_xd2;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 local_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 local_4;
  
  local_4 = in_PR;
  (*(code *)PTR_FUN_8c054620)(DAT_8c05461c,0x80,0);
  puVar2 = PTR_FUN_8c054634;
  iVar1 = DAT_8c054624;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(DAT_8c054624 + 4) = (undefined4)DAT_8c054628;
    *(undefined4 *)(iVar1 + 8) = DAT_8c054628._4_4_;
    dVar5 = (double)CONCAT44(DAT_8c054630,param_1);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar1 + 4));
  }
  else {
    *(ulonglong *)(DAT_8c054624 + 4) = CONCAT44(DAT_8c054628._4_4_,(undefined4)DAT_8c054628);
    *(ulonglong *)(iVar1 + 8) = CONCAT44(DAT_8c054630,DAT_8c054628._4_4_);
    dVar5 = (double)CONCAT44(PTR_FUN_8c054634,DAT_8c054630);
    in_xd2 = *(undefined8 *)(iVar1 + 4);
  }
  fVar3 = (float)((ulonglong)dVar5 >> 0x20);
  if (in_FPSCR_PR == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),SUB84(in_dr2,0) * fVar3);
  }
  else {
    in_dr2 = in_dr2 * dVar5;
  }
  if (in_FPSCR_SZ == '\0') {
    *(int *)(iVar1 + 0xc) = SUB84(in_dr2,0);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(iVar1 + 8));
  }
  else {
    *(undefined8 *)(iVar1 + 0xc) = in_xd2;
    in_xd2 = *(undefined8 *)(iVar1 + 8);
  }
  if (in_FPSCR_PR == '\0') {
    fVar3 = SUB84(in_dr2,0) * fVar3;
  }
  else {
    fVar3 = SUB84(in_dr2 * dVar5,0);
  }
  if (in_FPSCR_SZ == '\0') {
    *(float *)(iVar1 + 0x10) = fVar3;
  }
  else {
    *(undefined8 *)(iVar1 + 0x10) = in_xd2;
  }
  (*(code *)puVar2)();
  (*(code *)PTR_FUN_8c054638)(0x31c7);
  (*(code *)PTR_FUN_8c054640)(DAT_8c05463c);
  (*(code *)PTR_FUN_8c054644)(DAT_8c05463c);
  (*(code *)PTR_FUN_8c05464c)(DAT_8c054648,0x800);
  (*(code *)PTR_FUN_8c054650)(0,0);
  (*(code *)PTR_FUN_8c054654)(1);
  (*(code *)PTR_FUN_8c054658)(0);
  (*(code *)PTR_FUN_8c05465c)(0);
  (*(code *)PTR_FUN_8c054660)(0);
  (*(code *)PTR_FUN_8c054664)(0x36c);
  (*(code *)PTR_FUN_8c054668)(0xffffffff,0x2000);
  uVar4 = local_4;
  uStack_c = (undefined4)in_xd2;
  local_4 = (undefined4)((ulonglong)in_xd2 >> 0x20);
  local_14 = uStack_c;
  uStack_c = local_4;
  if (in_FPSCR_SZ == '\0') {
    uStack_c = 0;
    local_14 = 0;
    local_4 = uVar4;
  }
  uStack_10 = uStack_c;
  uStack_8 = uStack_c;
  (*(code *)PTR_FUN_8c05466c)(&local_14);
  if (in_FPSCR_SZ == '\0') {
    uVar6 = CONCAT44(DAT_8c054670._4_4_,(undefined4)DAT_8c054670);
  }
  else {
    uVar6 = CONCAT44(PTR_FUN_8c054678,DAT_8c054670._4_4_);
  }
  (*(code *)PTR_FUN_8c054678)(uVar6);
  if (in_FPSCR_SZ == '\0') {
    uVar6 = CONCAT44(DAT_8c054680,DAT_8c05467c);
  }
  else {
    uVar6 = CONCAT44(PTR_FUN_8c054684,DAT_8c054680);
  }
  (*(code *)PTR_FUN_8c054684)(uVar6);
  uVar4 = (undefined4)uVar6;
  (*(code *)PTR_FUN_8c054688)(0xffffffff);
  (*(code *)PTR_thunk_FUN_8c652e38_8c054690)(DAT_8c05468c);
  (*(code *)PTR_FUN_8c054698)(DAT_8c054694);
  (*(code *)PTR_thunk_FUN_8c652dd0_8c05469c)(DAT_8c054694);
  (*(code *)PTR_FUN_8c0546a0)();
  (*(code *)PTR_FUN_8c0546a4)(1);
  if (in_FPSCR_SZ == '\0') {
    uVar6 = CONCAT44(_DAT_8c0546a8,uVar4);
  }
  else {
    uVar6 = CONCAT44(PTR_thunk_FUN_8c652d3a_8c0546ac,_DAT_8c0546a8);
  }
  (*(code *)PTR_thunk_FUN_8c652d3a_8c0546ac)(uVar6);
  (*(code *)PTR_FUN_8c0546b4)(DAT_8c0546b0);
  (*(code *)PTR_FUN_8c0546b8)(0x10);
  (*(code *)PTR_FUN_8c0546bc)();
  return;
}

