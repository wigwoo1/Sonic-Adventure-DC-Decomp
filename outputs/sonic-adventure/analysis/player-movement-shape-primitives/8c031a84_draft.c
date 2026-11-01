/* Ghidra draft only; not buildable/verified source. */

void FUN_8c031a84(double *param_1,undefined4 *param_2,int param_3,undefined8 *param_4)

{
  double dVar1;
  undefined4 uVar2;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 local_8;
  
  if (in_FPSCR_SZ == '\0') {
    dVar1 = (double)CONCAT44(*(undefined4 *)param_1,*param_2);
  }
  else {
    dVar1 = *param_1;
  }
  if (in_FPSCR_PR == '\0') {
    dVar1 = (double)CONCAT44((float)((ulonglong)dVar1 >> 0x20) - SUB84(dVar1,0),SUB84(dVar1,0));
  }
  else {
    dVar1 = dVar1 - dVar1;
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_14 = (undefined4)((ulonglong)dVar1 >> 0x20);
    dVar1 = (double)CONCAT44(*(undefined4 *)((int)param_1 + 4),param_2[1]);
  }
  else {
    uStack_14 = SUB84(dVar1,0);
    dVar1 = *(double *)((int)param_1 + 4);
  }
  if (in_FPSCR_PR == '\0') {
    dVar1 = (double)CONCAT44((float)((ulonglong)dVar1 >> 0x20) - SUB84(dVar1,0),SUB84(dVar1,0));
  }
  else {
    dVar1 = dVar1 - dVar1;
  }
  if (in_FPSCR_SZ == '\0') {
    uStack_10 = (undefined4)((ulonglong)dVar1 >> 0x20);
    dVar1 = (double)CONCAT44(*(undefined4 *)(param_1 + 1),param_2[2]);
  }
  else {
    uStack_10 = SUB84(dVar1,0);
    dVar1 = param_1[1];
  }
  if (in_FPSCR_PR == '\0') {
    dVar1 = (double)CONCAT44((float)((ulonglong)dVar1 >> 0x20) - SUB84(dVar1,0),SUB84(dVar1,0));
  }
  else {
    dVar1 = dVar1 - dVar1;
  }
  uVar2 = (undefined4)((ulonglong)dVar1 >> 0x20);
  uStack_c = uVar2;
  local_8 = in_PR;
  if (in_FPSCR_SZ != '\0') {
    uStack_c = SUB84(dVar1,0);
    local_8 = uVar2;
  }
  (*(code *)PTR_FUN_8c031b04)(PTR_DAT_8c031b00);
  if (param_3 != 0) {
    (*(code *)PTR_FUN_8c031b08)(0,param_3);
  }
  (*(code *)PTR_FUN_8c031b18)(0,&uStack_14,&uStack_20);
  (*(code *)PTR_FUN_8c031b10)(1);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)param_4 = uStack_20;
    *(undefined4 *)((int)param_4 + 4) = uStack_1c;
    *(undefined4 *)(param_4 + 1) = uStack_18;
  }
  else {
    *param_4 = CONCAT44(uStack_1c,uStack_20);
    *(ulonglong *)((int)param_4 + 4) = CONCAT44(uStack_18,uStack_1c);
    param_4[1] = CONCAT44(uStack_14,uStack_18);
  }
  return;
}

