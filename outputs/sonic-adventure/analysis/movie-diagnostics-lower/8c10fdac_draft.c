/* Ghidra draft only; not buildable/verified source. */

void FUN_8c10fdac(undefined4 param_1,char *param_2,int param_3)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  char cVar4;
  int iVar5;
  char *pcVar6;
  int iVar7;
  int iVar8;
  int local_4c;
  int local_48;
  int local_44;
  undefined *local_38;
  int local_34;
  undefined1 auStack_28 [8];
  undefined1 local_20 [4];
  
  puVar1 = PTR_FUN_8c10fec4;
  local_34 = 1;
  pcVar6 = param_2 + param_3;
  *pcVar6 = '\0';
  (*(code *)puVar1)(auStack_28,param_1,8);
  local_48 = param_3 + -1;
  local_38 = PTR_DAT_8c10fec8 + local_48 * 8;
  do {
    if (local_48 < 0) {
      return;
    }
    iVar2 = (*(code *)PTR_FUN_8c10fecc)(param_3);
    iVar2 = (iVar2 + 1) * -2;
    iVar8 = iVar2 + 8;
    iVar7 = 8 - iVar8;
    (*(code *)PTR_FUN_8c10fec4)(auStack_28 + iVar2,local_38 + iVar8,iVar7);
    iVar5 = 8;
    local_44 = 0;
    local_4c = 0;
    do {
      iVar3 = (*(code *)PTR_FUN_8c10fed0)(auStack_28 + iVar8,auStack_28 + iVar2,iVar7);
      if (-1 < iVar3) {
        local_34 = (*(code *)PTR_FUN_8c10fed4)(0,auStack_28 + iVar8,auStack_28 + iVar2,iVar7);
        local_44 = local_44 + iVar5;
      }
      iVar5 = iVar5 >> 1;
      if (iVar5 == 0) break;
      (*(code *)PTR_FUN_8c10fed8)(auStack_28 + iVar2,iVar7,1);
      local_4c = local_4c + 1;
    } while (local_4c < 4);
    cVar4 = (char)local_44 + '0';
    if (local_34 == 0) {
      pcVar6 = pcVar6 + -1;
      *pcVar6 = cVar4;
      while (pcVar6 = pcVar6 + -1, param_2 <= pcVar6) {
        *pcVar6 = '0';
      }
      return;
    }
    pcVar6 = pcVar6 + -1;
    *pcVar6 = cVar4;
    local_48 = local_48 + -1;
    local_38 = local_38 + -8;
    param_3 = param_3 + -1;
  } while( true );
}

