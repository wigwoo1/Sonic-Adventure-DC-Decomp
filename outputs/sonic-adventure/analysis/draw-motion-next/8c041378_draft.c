/* Ghidra draft only; not buildable/verified source. */

void FUN_8c041378(undefined4 param_1,uint *param_2)

{
  short sVar1;
  ushort *puVar2;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined8 uVar8;
  char in_FPSCR_SZ;
  int local_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 auStack_3c [4];
  undefined1 auStack_2c [20];
  
  iVar3 = DAT_8c0415d0;
  puVar2 = DAT_8c0415c4;
  puVar4 = (undefined4 *)param_2[1];
  if ((puVar4 != (undefined4 *)0x0) && ((*param_2 & 8) == 0)) {
    local_48 = 0;
    iVar7 = *(int *)(param_2[1] + 8);
    iVar5 = (uint)*DAT_8c0415c4 * iVar7 * 0x18 + *DAT_8c0415c8;
    iVar6 = (iVar7 * 3 + (uint)*DAT_8c0415c4 * iVar7 * 6) * 4 + *DAT_8c0415c8;
    *(undefined4 *)(DAT_8c0415cc + (uint)*DAT_8c0415c4 * 4) = *puVar4;
    *(undefined4 *)(DAT_8c0415d4 + (uint)*puVar2 * 4) = puVar4[1];
    sVar1 = *(short *)((uint)*puVar2 * 2 + iVar3);
    if (sVar1 == 1) {
      FUN_8c0410d0(&local_48,&uStack_40,auStack_2c,iVar5);
    }
    else if (sVar1 == 2) {
      FUN_8c041152(&local_48,&uStack_44,auStack_3c,iVar6);
    }
    else if (sVar1 == 3) {
      FUN_8c0411d4(&local_48,&uStack_40,auStack_2c,iVar5);
      iVar3 = (int)(char)((char)*puVar2 * '\x1c');
      *(int *)(DAT_8c0415d8 + iVar3) = *(int *)(DAT_8c0415d8 + iVar3) + 1;
      FUN_8c04124e(&local_48,&uStack_44,auStack_3c,iVar6);
    }
    if (local_48 == 1) {
      if (in_FPSCR_SZ == '\0') {
        uVar8 = CONCAT44(uStack_40,param_1);
      }
      else {
        uVar8 = CONCAT44(auStack_3c[0],uStack_40);
      }
      FUN_8c0412c8(uVar8,auStack_2c,iVar5,iVar7);
    }
    else {
      if (local_48 != 2) {
        if (local_48 != 3) {
          return;
        }
        if (in_FPSCR_SZ == '\0') {
          uVar8 = CONCAT44(uStack_40,param_1);
        }
        else {
          uVar8 = CONCAT44(auStack_3c[0],uStack_40);
        }
        FUN_8c0412c8(uVar8,auStack_2c,iVar5,iVar7);
        param_1 = (undefined4)uVar8;
      }
      if (in_FPSCR_SZ == '\0') {
        uVar8 = CONCAT44(uStack_44,param_1);
      }
      else {
        uVar8 = CONCAT44(uStack_40,uStack_44);
      }
      FUN_8c041340(uVar8,auStack_3c,iVar6,iVar7);
    }
  }
  return;
}

