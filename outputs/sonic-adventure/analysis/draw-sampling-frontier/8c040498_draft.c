/* Ghidra draft only; not buildable/verified source. */

void FUN_8c040498(undefined4 param_1,uint *param_2)

{
  short sVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  undefined8 uVar4;
  char in_FPSCR_SZ;
  int local_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined1 auStack_24 [8];
  undefined1 auStack_1c [12];
  
  puVar3 = (undefined4 *)param_2[1];
  if ((puVar3 != (undefined4 *)0x0) && ((*param_2 & 8) == 0)) {
    local_28 = *puVar3;
    local_2c = puVar3[1];
    uVar2 = puVar3[2];
    local_38 = 0;
    sVar1 = *DAT_8c040668;
    if (sVar1 == 1) {
      FUN_8c0402d0(&local_38,&uStack_30,auStack_1c);
    }
    else if (sVar1 == 2) {
      FUN_8c04031e(&local_38,&uStack_34,auStack_24);
    }
    else if (sVar1 == 3) {
      FUN_8c0402d0(&local_38,&uStack_30,auStack_1c);
      *DAT_8c04066c = *DAT_8c04066c + 1;
      FUN_8c04031e(&local_38,&uStack_34,auStack_24);
    }
    if (local_38 == 1) {
      if (in_FPSCR_SZ == '\0') {
        uVar4 = CONCAT44(uStack_30,param_1);
      }
      else {
        uVar4 = CONCAT44(local_2c,uStack_30);
      }
      FUN_8c0403c0(uVar4,puVar3,uVar2,auStack_1c);
    }
    else if (local_38 == 2) {
      if (in_FPSCR_SZ == '\0') {
        uVar4 = CONCAT44(uStack_34,param_1);
      }
      else {
        uVar4 = CONCAT44(uStack_30,uStack_34);
      }
      FUN_8c040404(uVar4,puVar3,uVar2,auStack_24);
    }
    else if (local_38 == 3) {
      if (in_FPSCR_SZ == '\0') {
        uVar4 = CONCAT44(uStack_30,param_1);
      }
      else {
        uVar4 = CONCAT44(local_2c,uStack_30);
      }
      FUN_8c0403c0(uVar4,puVar3,uVar2,auStack_1c);
      if (in_FPSCR_SZ == '\0') {
        uVar4 = CONCAT44(uStack_34,(int)uVar4);
      }
      else {
        uVar4 = CONCAT44(uStack_30,uStack_34);
      }
      FUN_8c040448(uVar4,puVar3,uVar2,auStack_24);
    }
    (*(code *)PTR_FUN_8c040670)(puVar3);
    *puVar3 = local_28;
    puVar3[1] = local_2c;
  }
  return;
}

