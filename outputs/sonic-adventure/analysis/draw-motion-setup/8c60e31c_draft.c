/* Ghidra draft only; not buildable/verified source. */

void FUN_8c60e31c(ulonglong param_1,uint *param_2)

{
  int iVar1;
  undefined8 uVar2;
  uint uVar3;
  undefined8 uVar4;
  char in_FPSCR_SZ;
  undefined1 *puVar5;
  
  puVar5 = &DAT_8c88fe78;
  while( true ) {
    (*(code *)PTR_FUN_8c60e520)(0,puVar5);
    iVar1 = DAT_8c60e528;
    if ((*param_2 & 0x40) == 0) {
      (*(code *)*DAT_8c60e524)(param_2);
    }
    else {
      if (in_FPSCR_SZ == '\0') {
        uVar4 = CONCAT44(param_2[4],(int)param_1);
        uVar2 = CONCAT44(param_2[2],param_2[3]);
      }
      else {
        uVar4 = *(undefined8 *)(param_2 + 4);
        uVar2 = *(undefined8 *)(param_2 + 2);
      }
      (*(code *)PTR_FUN_8c60e52c)(uVar2,uVar4,*(undefined4 *)(DAT_8c60e528 + 0x18));
      uVar3 = (uint)uVar4;
      if ((*param_2 & 0x20) == 0) {
        (*(code *)PTR_FUN_8c60e534)(*(undefined4 *)(iVar1 + 0x18),param_2[5],param_2[6],param_2[7]);
      }
      else {
        (*(code *)PTR_FUN_8c60e530)(param_2[5],param_2[6],param_2[7]);
      }
      if (in_FPSCR_SZ == '\0') {
        param_1 = (ulonglong)uVar3;
      }
      else {
        param_1 = *(ulonglong *)(param_2 + 10);
      }
      (*(code *)PTR_FUN_8c60e538)(*(undefined4 *)(iVar1 + 0x18));
    }
    if ((code *)*DAT_8c60e53c != (code *)0x0) {
      (*(code *)*DAT_8c60e53c)(param_2);
    }
    if ((param_2[1] != 0) && ((*param_2 & 8) == 0)) {
      (*(code *)PTR_FUN_8c60e540)(param_2[1]);
    }
    if (param_2[0xb] != 0) {
      FUN_8c60e31c(param_2[0xb]);
    }
    (*(code *)PTR_FUN_8c60e544)(1);
    if (param_2[0xc] == 0) break;
    param_2 = (uint *)param_2[0xc];
  }
  return;
}

