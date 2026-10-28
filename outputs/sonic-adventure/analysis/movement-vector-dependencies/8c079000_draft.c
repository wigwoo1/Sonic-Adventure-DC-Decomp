/* Ghidra draft only; not buildable/verified source. */

void FUN_8c079000(undefined8 *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined8 *puVar1;
  undefined1 *puVar2;
  undefined4 uVar4;
  double dVar3;
  double in_dr2;
  undefined8 uVar5;
  float in_fr15;
  undefined4 in_fr14;
  undefined4 in_PR;
  int iVar6;
  int iVar7;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined1 local_10 [4];
  undefined1 local_c [4];
  
  if (in_FPSCR_SZ == '\0') {
    puVar2 = local_c;
  }
  else {
    puVar2 = local_10;
  }
  *(undefined4 *)(puVar2 + -4) = in_PR;
  *(undefined4 *)(puVar2 + -0x1c) = param_2;
  *(undefined4 *)(puVar2 + -0x18) = param_3;
  puVar1 = *(undefined8 **)(puVar2 + -0x1c);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(puVar2 + -0x10) = *(undefined4 *)puVar1;
    *(undefined4 *)(puVar2 + -0xc) = *(undefined4 *)((int)puVar1 + 4);
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),*(undefined4 *)(puVar1 + 1));
    *(undefined4 *)(puVar2 + -8) = *(undefined4 *)(puVar1 + 1);
    uVar5 = CONCAT44(*(undefined4 *)param_1,*(undefined4 *)((int)param_1 + 4));
    in_fr15 = DAT_8c0791c0;
  }
  else {
    *(undefined8 *)(puVar2 + -0x10) = *puVar1;
    *(undefined8 *)(puVar2 + -0xc) = puVar1[1];
    *(undefined8 *)(puVar2 + -8) = puVar1[2];
    uVar5 = *param_1;
  }
  uVar4 = (*(code *)PTR_FUN_8c0791c4)(uVar5);
  if (in_FPSCR_SZ == '\0') {
    in_dr2 = (double)CONCAT44((int)((ulonglong)in_dr2 >> 0x20),uVar4);
  }
  if (in_FPSCR_PR == '\0') {
    iVar6 = (int)(SUB84(in_dr2,0) * in_fr15);
  }
  else {
    iVar6 = (int)(in_dr2 * (double)CONCAT44(in_fr14,in_fr15));
  }
  if (in_FPSCR_SZ == '\0') {
    uVar5 = CONCAT44(*(undefined4 *)(param_1 + 1),(int)uVar5);
  }
  else {
    uVar5 = param_1[1];
  }
  dVar3 = (double)(*(code *)PTR_FUN_8c0791c8)(uVar5);
  if (in_FPSCR_PR == '\0') {
    iVar7 = (int)((float)((ulonglong)dVar3 >> 0x20) * in_fr15);
  }
  else {
    iVar7 = (int)(dVar3 * (double)CONCAT44(in_fr14,in_fr15));
  }
  (*(code *)PTR_FUN_8c0791cc)();
  if (-iVar6 != 0) {
    (*(code *)PTR_FUN_8c0791d0)(0,-iVar6);
  }
  if (iVar7 != 0) {
    (*(code *)PTR_FUN_8c0791d4)(0,iVar7);
  }
  iVar6 = -*(int *)(puVar2 + -0x18);
  *(int *)(puVar2 + -0x14) = iVar6;
  if (iVar6 != 0) {
    (*(code *)PTR_FUN_8c0791d8)(0,iVar6);
  }
  (*(code *)PTR_FUN_8c0791dc)(0,puVar2 + -0x10,*(undefined4 *)(puVar2 + -0x1c));
  (*(code *)PTR_FUN_8c0791e0)(1);
  return;
}

