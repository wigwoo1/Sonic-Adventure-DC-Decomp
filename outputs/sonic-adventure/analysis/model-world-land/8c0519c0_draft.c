/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void original_entry_8c0519c0(undefined8 param_1,uint param_2,int param_3)

{
  char cVar1;
  undefined *puVar2;
  char *pcVar3;
  undefined4 *puVar4;
  ulonglong uVar5;
  uint uVar6;
  undefined4 uVar7;
  undefined8 in_xd6;
  ulonglong in_xd14;
  undefined4 in_PR;
  char in_FPSCR_SZ;
  undefined4 uStack_c;
  
  uVar6 = (uint)param_1;
  pcVar3 = *(char **)(param_3 + 0x20);
  cVar1 = *pcVar3;
  uStack_c = in_PR;
  if (cVar1 == '\0') {
    FUN_8c051a16(param_3);
    puVar2 = PTR_FUN_8c051b04;
    *DAT_8c051b00 = pcVar3;
    (*(code *)puVar2)();
    puVar2 = PTR_FUN_8c051b0c;
    *(undefined **)(param_3 + 0x14) = PTR_LAB_8c051b08;
    *(undefined **)(param_3 + 0x18) = puVar2;
    if (in_FPSCR_SZ == '\0') {
      param_1 = CONCAT44(_DAT_8c051b10,uVar6);
      *(undefined4 *)PTR_DAT_8c051b14 = _DAT_8c051b10;
    }
    else {
      param_1 = CONCAT44(PTR_DAT_8c051b14,_DAT_8c051b10);
      *(undefined8 *)PTR_DAT_8c051b14 = param_1;
    }
  }
  else if (cVar1 != '\x02') {
    if (cVar1 != '\x01') {
      *pcVar3 = '\0';
      return;
    }
    goto LAB_8c051a00;
  }
  uVar6 = (uint)param_1;
  FUN_8c051cc0();
  *pcVar3 = '\x01';
LAB_8c051a00:
  if (in_FPSCR_SZ == '\0') {
    puVar4 = (undefined4 *)&stack0xfffffff8;
  }
  else {
    puVar4 = &uStack_c;
  }
  *(undefined4 *)((int)puVar4 + -4) = uStack_c;
  if (in_FPSCR_SZ == '\0') {
    uVar6 = *DAT_8c051de8;
    param_2 = uVar6;
  }
  else {
    in_xd6 = *(undefined8 *)DAT_8c051de8;
  }
  uVar7 = 0;
  uVar5 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    in_xd6 = CONCAT44(param_2,param_2);
    uVar5 = (ulonglong)uVar6;
  }
  (*(code *)PTR_FUN_8c051dec)(uVar5,in_xd6,param_3);
  (*(code *)PTR_FUN_8c051df0)();
  (*(code *)PTR_FUN_8c051df4)();
  if (*(int *)PTR_DAT_8c051dd4 == 1) {
    if (*DAT_8c051df8 == 0) {
      FUN_8c0520c8();
      FUN_8c051e56();
      FUN_8c052048();
      FUN_8c051f64();
    }
    else {
      FUN_8c052048();
    }
  }
  (*(code *)PTR_FUN_8c051dfc)();
  if (in_FPSCR_SZ == '\0') {
    in_xd14 = CONCAT44(uVar7,uVar7);
  }
  (*(code *)PTR_FUN_8c051dec)(in_xd14,in_xd14);
  return;
}

