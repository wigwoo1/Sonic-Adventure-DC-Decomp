/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c64573e(int param_1,int param_2,int param_3,int param_4,char *param_5)

{
  char cVar1;
  bool bVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  char *pcVar6;
  char *pcVar7;
  uint uVar8;
  byte *pbVar9;
  byte *pbVar10;
  byte *pbVar11;
  uint local_44;
  ushort local_3c [2];
  int local_38;
  int local_34;
  uint local_30;
  uint local_2c;
  int local_28;
  uint local_24;
  
  bVar2 = true;
  local_44 = 0;
  pcVar6 = param_5;
  while (cVar1 = *pcVar6, pcVar6 = pcVar6 + 1, cVar1 != '\0') {
    local_44 = local_44 + 1;
  }
  pcVar6 = (char *)(param_4 + 1);
  uVar8 = 1;
  local_24 = 1;
  local_34 = 0;
  pbVar9 = (byte *)(param_1 + DAT_8c645846);
  local_30 = 0;
  pbVar11 = pbVar9;
  local_38 = param_2;
  local_28 = param_3;
  do {
    if ((pbVar9 <= pbVar11) ||
       (pbVar10 = pbVar11, pbVar9 <= pbVar11 + (uint)*pbVar11 + (*pbVar11 & 1) + 8)) {
      pbVar10 = pbVar11 + -(int)DAT_8c645846;
      FUN_8c64535c(pbVar11,pbVar10);
      iVar4 = (*(code *)PTR_FUN_8c645848)(local_38,1,param_1);
      if (iVar4 != 0) {
        return 0;
      }
      local_38 = local_38 + 1;
    }
    FUN_8c64535c(pbVar10 + 6,local_3c,2);
    uVar3 = local_24;
    if ((local_3c[0] == local_24) && (uVar3 = uVar8, uVar8 != 1)) {
      if (bVar2) {
        local_2c = local_44;
        pcVar7 = param_5;
      }
      else {
        local_2c = local_30;
        pcVar7 = pcVar6;
      }
      uVar3 = local_24;
      if ((*pbVar10 == local_2c) &&
         (iVar4 = FUN_8c645374(pbVar10 + 8,pcVar7,local_2c), uVar3 = local_24, iVar4 != 0)) {
        if (!bVar2) {
          pcVar6 = pcVar6 + local_30;
        }
        if (*pcVar6 == '\0') {
          uVar5 = (*(code *)PTR_FUN_8c64584c)(pbVar10 + 2);
          return uVar5;
        }
        if (bVar2) {
          bVar2 = false;
        }
        else if (uVar8 != 1) {
          pcVar6 = pcVar6 + 1;
        }
        local_30 = FUN_8c64541c(pcVar6,PTR_DAT_8c645894);
        uVar3 = uVar8;
      }
    }
    local_24 = uVar3;
    uVar8 = uVar8 + 1;
    iVar4 = (*pbVar10 & 1) + (uint)*pbVar10 + 8;
    local_34 = local_34 + iVar4;
    pbVar11 = pbVar10 + iVar4;
    if (local_28 <= local_34) {
      return 0;
    }
  } while( true );
}

