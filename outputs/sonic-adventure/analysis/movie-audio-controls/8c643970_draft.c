/* Ghidra draft only; not buildable/verified source. */

int FUN_8c643970(int param_1)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  int local_28;
  int local_24;
  
  puVar1 = PTR_FUN_8c643a7c;
  local_28 = 0;
  local_24 = param_1;
  (*(code *)PTR_FUN_8c643a7c)(PTR_s_Enter__s_8c643a84,PTR_s_sd_multi_unit_download__c_8c643a80);
  iVar2 = DAT_8c643a8c;
  if (((*(int *)PTR_DAT_8c643a88 != 0) && (iVar2 = DAT_8c643a90, local_24 != 0)) &&
     (piVar5 = *(int **)(local_24 + 8), iVar2 = DAT_8c643a98, *piVar5 == DAT_8c643a94)) {
    (*(code *)PTR_FUN_8c643a9c)(&local_28);
    iVar4 = piVar5[2];
    piVar6 = piVar5 + 8;
    iVar2 = 0;
    if (((char)piVar5[1] < '\x01') &&
       ((*(code *)puVar1)(PTR_s_Warning_Illegal_MultiUnit_versio_8c643aa0), iVar2 = DAT_8c643aa4,
       piVar5[4] != -1)) {
      piVar6 = piVar5 + 4;
    }
    (*(code *)puVar1)(PTR_s_Block__02lX_Download_Adr__08X_8c643aa8,iVar4,piVar6);
    iVar3 = 0;
    if (0 < iVar4) {
      do {
        if (((-1 < piVar6[3]) &&
            (iVar2 = (*(code *)PTR_FUN_8c643aac)(*piVar6,piVar6[1],piVar6[2],piVar6[3]), iVar2 != 0)
            ) || ((0 < piVar6[5] &&
                  ((iVar2 = (*(code *)PTR_FUN_8c643ab0)
                                      (local_28,piVar6[4] + (int)piVar5,piVar6[5],0xffffffff,0),
                   iVar2 != 0 ||
                   (iVar2 = (*(code *)PTR_FUN_8c643ab4)(local_28,*piVar6,piVar6[1]), iVar2 != 0)))))
           ) break;
        iVar3 = iVar3 + 1;
        piVar6 = piVar6 + 8;
      } while (iVar3 < iVar4);
    }
  }
  (*(code *)puVar1)(PTR_s_Exit___s__08X__8c643ab8,PTR_s_sd_multi_unit_download__c_8c643a80,iVar2);
  if (local_28 != 0) {
    (*(code *)PTR_FUN_8c643abc)(local_28);
  }
  return iVar2;
}

