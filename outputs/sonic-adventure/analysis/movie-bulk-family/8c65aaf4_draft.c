/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c65aaf4(void)

{
  short sVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
  int iVar9;
  
  puVar2 = PTR_FUN_8c65ab6c;
  (*(code *)PTR_FUN_8c65ab6c)(PTR_s_Enter__s_8c65ab74,PTR_s_sd_drv_get_info__c_8c65ab70);
  puVar4 = PTR_FUN_8c65ab80;
  puVar3 = PTR_DAT_8c65ab78;
  iVar6 = *(int *)PTR_DAT_8c65ab78;
  uVar7 = DAT_8c65ab7c;
  if (iVar6 != 0) {
    iVar5 = (int)DAT_8c65ab64;
    iVar8 = *(int *)(DAT_8c65ab62 + iVar6);
    iVar9 = iVar8 + 0x20;
    *(int *)(DAT_8c65ab62 + -0x1c + iVar6) = iVar9;
    (*(code *)puVar4)(iVar9,*(int *)puVar3 + iVar5);
    (*(code *)puVar4)(iVar8 + 0x21,*(int *)puVar3 + (int)DAT_8c65ab66);
    if ((*(char *)(*(int *)puVar3 + (int)DAT_8c65ab64) == '\x01') &&
       (*(char *)((int)DAT_8c65ab66 + *(int *)puVar3) == '\0')) {
      iVar8 = iVar8 + 0x23;
      sVar1 = DAT_8c65ab68;
    }
    else {
      iVar8 = iVar8 + 0x22;
      sVar1 = DAT_8c65ac64;
    }
    (*(code *)puVar4)(iVar8,(int)sVar1 + *(int *)puVar3);
    iVar6 = (int)DAT_8c65ac6a;
    *(undefined1 *)((int)DAT_8c65ac66 + *(int *)puVar3) = 0;
    puVar4 = PTR_FUN_8c65ac84;
    (*(code *)PTR_FUN_8c65ac84)(iVar6 + iVar9,*(int *)puVar3 + (int)DAT_8c65ac68);
    (*(code *)puVar4)(DAT_8c65ac6e + iVar9,*(int *)puVar3 + (int)DAT_8c65ac6c);
    (*(code *)puVar4)(DAT_8c65ac72 + iVar9,*(int *)puVar3 + (int)DAT_8c65ac70);
    (*(code *)puVar4)(DAT_8c65ac76 + iVar9,*(int *)puVar3 + (int)DAT_8c65ac74);
    (*(code *)puVar4)(DAT_8c65ac7a + iVar9,*(int *)puVar3 + (int)DAT_8c65ac78);
    (*(code *)puVar4)(DAT_8c65ac7e + iVar9,*(int *)puVar3 + (int)DAT_8c65ac7c);
    (*(code *)puVar2)(PTR_s_System_Information____08X_8c65ac88,
                      *(undefined4 *)((int)DAT_8c65ac80 + *(int *)puVar3));
    (*(code *)puVar2)(PTR_s_System_Tool_Interface____08X_8c65ac8c,
                      *(undefined4 *)((int)DAT_8c65ac68 + *(int *)puVar3));
    (*(code *)puVar2)(PTR_s_System_Sound_Status____08X_8c65ac90,
                      *(undefined4 *)((int)DAT_8c65ac6c + *(int *)puVar3));
    (*(code *)puVar2)(PTR_s_System_Status____08X_8c65ac94,
                      *(undefined4 *)((int)DAT_8c65ac70 + *(int *)puVar3));
    (*(code *)puVar2)(PTR_s_System_Data_Map____08X_8c65ac98,
                      *(undefined4 *)((int)DAT_8c65ac74 + *(int *)puVar3));
    (*(code *)puVar2)(PTR_s_System_Host_Command____08X_8c65ac9c,
                      *(undefined4 *)((int)DAT_8c65ac78 + *(int *)puVar3));
    (*(code *)puVar2)(PTR_s_System_Command_History____08X_8c65aca0,
                      *(undefined4 *)((int)DAT_8c65ac7c + *(int *)puVar3));
    uVar7 = 0;
  }
  (*(code *)puVar2)(PTR_s_Exit___s_8c65aca8,PTR_s_sd_drv_get_info__c_8c65aca4);
  return uVar7;
}

