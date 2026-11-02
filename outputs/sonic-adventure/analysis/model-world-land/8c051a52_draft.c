/* Ghidra draft only; not buildable/verified source. */

void FUN_8c051a52(int param_1,int param_2)

{
  undefined *puVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  
  psVar6 = *(short **)(PTR_DAT_8c051b48 + param_2 * 4 + param_1 * 0x20);
  if (psVar6 != (short *)0x0) {
    iVar5 = (int)*psVar6;
    if ((psVar6[2] & 0x10U) != 0) {
      if ((psVar6[3] & 0xcU) == 0) {
        iVar2 = (*(code *)PTR_FUN_8c051b4c)(*(undefined4 *)(psVar6 + 0xe),&DAT_8c7493c8);
        puVar1 = PTR_FUN_8c051b50;
        *(int *)(psVar6 + 0x10) = iVar2;
        (*(code *)puVar1)(iVar2);
        iVar3 = *(int *)(psVar6 + 6);
        for (iVar4 = iVar5; 0 < iVar4; iVar4 = iVar4 + -1) {
          *(int *)(iVar3 + 0x18) = *(int *)(iVar3 + 0x18) + iVar2;
          iVar3 = iVar3 + 0x24;
        }
        if ((psVar6[2] & 1U) != 0) {
          iVar4 = *(int *)(psVar6 + 8);
          for (; 0 < iVar5; iVar5 = iVar5 + -1) {
            *(int *)(iVar4 + 0xc) = *(int *)(iVar4 + 0xc) + iVar2;
            *(int *)(iVar4 + 0x10) = *(int *)(iVar4 + 0x10) + iVar2;
            iVar4 = iVar4 + 0x18;
          }
        }
      }
      psVar6[3] = psVar6[3] + 4;
    }
    puVar1 = PTR_FUN_8c051b54;
    if ((psVar6[2] & 2U) == 0) {
      if ((psVar6[2] & 8U) == 0) {
        if ((psVar6[2] & 1U) != 0) {
          iVar4 = *(int *)(psVar6 + 8);
          for (iVar5 = (int)psVar6[1]; 0 < iVar5; iVar5 = iVar5 + -1) {
            if (*(int *)(iVar4 + 0x14) != 0) {
              (*(code *)puVar1)(*(undefined4 *)(iVar4 + 0x14));
            }
            iVar4 = iVar4 + 0x18;
          }
        }
      }
      else if (*(int *)(psVar6 + 10) != 0) {
        if ((psVar6[3] & 3U) == 0) {
          (*(code *)PTR_FUN_8c051ca0)(*(undefined4 *)(psVar6 + 10),*(undefined4 *)(psVar6 + 0xc));
        }
        psVar6[3] = psVar6[3] + 1;
      }
    }
    else {
      if ((psVar6[3] & 3U) == 0) {
        (*(code *)PTR_FUN_8c051b54)(*(undefined4 *)(psVar6 + 0xc));
      }
      psVar6[3] = psVar6[3] + 1;
    }
  }
  return;
}

