/* Ghidra draft only; not buildable/verified source. */

void FUN_8c641e00(uint param_1)

{
  uint uVar1;
  undefined *puVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 *puVar5;
  int *piVar6;
  int iVar7;
  uint uVar8;
  undefined1 auStack_28 [4];
  uint local_24;
  
  uVar3 = DAT_8c641ed8;
  puVar2 = PTR_FUN_8c641ed4;
  uVar1 = DAT_8c641ed0;
  uVar8 = 0;
  puVar5 = (undefined4 *)(DAT_8c641ec8 + *DAT_8c641ec4 * 4);
  piVar6 = (int *)(DAT_8c641ecc + *DAT_8c641ec4 * 4);
  iVar7 = (int)DAT_8c641ec0;
  local_24 = param_1;
  do {
    if ((*piVar6 != 0) && (local_24 != uVar8)) {
      if ((*DAT_8c641edc == 1) && ((uVar8 == 1 || (uVar8 == 3)))) {
        *(uint *)*DAT_8c641ee0 = *(uint *)*DAT_8c641ee0 & uVar1 | uVar8 << 0x18;
        do {
          iVar4 = (*(code *)puVar2)(uVar3,*DAT_8c641ee0,*DAT_8c641ee4,0x10,auStack_28);
        } while (iVar4 == iVar7);
      }
      do {
        iVar4 = (*(code *)puVar2)(uVar3,*puVar5,*piVar6 + 0x20,0x10,auStack_28);
      } while (iVar4 == iVar7);
    }
    uVar8 = uVar8 + 1;
    piVar6 = piVar6 + 4;
    puVar5 = puVar5 + 4;
  } while (uVar8 < 5);
  return;
}

