/* Ghidra draft only; not buildable/verified source. */

int FUN_8c643904(undefined4 param_1,undefined4 param_2,char param_3)

{
  int iVar1;
  int local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  char cStack_c;
  
  local_14 = param_1;
  local_10 = param_2;
  cStack_c = param_3;
  iVar1 = (*(code *)PTR_FUN_8c64395c)(param_2,(int)param_3,&local_1c,&local_18);
  if (((iVar1 == 0) && (iVar1 = DAT_8c643960, local_1c != 0)) && (0 < local_18)) {
    iVar1 = (*(code *)PTR_FUN_8c643964)(local_14,local_1c);
  }
  if (iVar1 != 0) {
    (*(code *)PTR_FUN_8c64396c)(PTR_FUN_8c643968,iVar1);
  }
  return iVar1;
}

