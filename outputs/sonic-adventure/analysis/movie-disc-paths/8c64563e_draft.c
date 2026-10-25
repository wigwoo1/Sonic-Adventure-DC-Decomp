/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64563e(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  byte *pbVar6;
  byte *pbVar7;
  byte *pbVar8;
  ushort local_34 [2];
  int local_30;
  int local_2c;
  int local_28;
  uint local_24;
  
  pbVar6 = (byte *)(DAT_8c645766 + param_1);
  uVar3 = 0;
  pcVar5 = (char *)(param_4 + 1);
  local_24 = 1;
  uVar4 = 1;
  local_2c = 0;
  pbVar8 = pbVar6;
  local_30 = param_2;
  local_28 = param_3;
  do {
    if ((pbVar6 <= pbVar8) ||
       (pbVar7 = pbVar8, pbVar6 <= pbVar8 + (uint)*pbVar8 + (*pbVar8 & 1) + 8)) {
      pbVar7 = pbVar8 + -(int)DAT_8c645766;
      FUN_8c64535c(pbVar8,pbVar7);
      iVar1 = (*(code *)PTR_FUN_8c645768)(local_30,1,param_1);
      if (iVar1 != 0) {
        return 0;
      }
      local_30 = local_30 + 1;
    }
    FUN_8c64535c(pbVar7 + 6,local_34,2);
    if ((local_34[0] == local_24) &&
       ((uVar4 == 1 ||
        ((*pbVar7 == uVar3 && (iVar1 = FUN_8c645374(pbVar7 + 8,pcVar5,uVar3), iVar1 != 0)))))) {
      pcVar5 = pcVar5 + uVar3;
      local_24 = uVar4;
      if (*pcVar5 == '\0') {
        uVar2 = (*(code *)PTR_FUN_8c64576c)(pbVar7 + 2);
        return uVar2;
      }
      if (uVar4 != 1) {
        pcVar5 = pcVar5 + 1;
      }
      uVar3 = FUN_8c64541c(pcVar5,PTR_DAT_8c645770);
    }
    uVar4 = uVar4 + 1;
    iVar1 = (*pbVar7 & 1) + (uint)*pbVar7 + 8;
    local_2c = local_2c + iVar1;
    pbVar8 = pbVar7 + iVar1;
  } while (local_2c < local_28);
  return 0;
}

