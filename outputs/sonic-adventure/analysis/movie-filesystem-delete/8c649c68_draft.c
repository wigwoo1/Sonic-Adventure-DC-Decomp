/* Ghidra draft only; not buildable/verified source. */

int FUN_8c649c68(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  undefined1 auStack_48 [12];
  uint local_3c;
  int local_20;
  
  iVar2 = (*(code *)PTR_FUN_8c649d00)(param_1,auStack_48);
  if (iVar2 < 0) {
    iVar2 = (int)DAT_8c649cf4;
  }
  else {
    uVar3 = (*(code *)PTR_FUN_8c649d04)(param_1,iVar2);
    iVar4 = (*(code *)PTR_FUN_8c649d08)(param_1,param_2);
    uVar1 = DAT_8c649d0c;
    if (iVar4 == 0) {
      uVar6 = local_3c;
      for (iVar4 = 0; iVar4 < local_20; iVar4 = iVar4 + 1) {
        uVar5 = (*(code *)PTR_FUN_8c649d10)(param_1,uVar6);
        (*(code *)PTR_FUN_8c649d14)(param_1,uVar6,uVar1);
        uVar6 = uVar5 & 0xffff;
      }
    }
    (*(code *)PTR_FUN_8c649d18)(uVar3,0,0x20);
    (*(code *)PTR_FUN_8c649d1c)(param_1,iVar2);
    iVar2 = 0;
  }
  return iVar2;
}

