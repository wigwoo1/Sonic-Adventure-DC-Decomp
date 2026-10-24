/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x8c645a9e) */
/* WARNING: Removing unreachable block (ram,0x8c645a86) */
/* WARNING: Removing unreachable block (ram,0x8c645a48) */
/* WARNING: Removing unreachable block (ram,0x8c645986) */
/* WARNING: Removing unreachable block (ram,0x8c645a68) */
/* WARNING: Removing unreachable block (ram,0x8c645a8e) */
/* WARNING: Removing unreachable block (ram,0x8c645ab4) */
/* WARNING: Removing unreachable block (ram,0x8c645a76) */

uint FUN_8c64594c(short param_1,undefined4 param_2,uint param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  int in_VBR;
  byte in_IMASK;
  byte in_S;
  byte in_T;
  
  iVar2 = DAT_8c645a10;
  iVar1 = DAT_8c645a0c;
  iVar3 = (int)param_1;
  iVar4 = iVar3 >> 4;
  param_3 = param_3 | DAT_8c645a14;
  iVar5 = *(int *)(iVar4 * 4 + DAT_8c645a10);
  iVar9 = iVar5;
  while ((iVar6 = iVar5, iVar6 != 0 && (*(uint *)(iVar6 + 0xc) < param_3))) {
    iVar9 = iVar6;
    iVar5 = *(int *)(iVar6 + 4);
  }
  uVar10 = 1;
  do {
    if ((*(uint *)(uVar10 * 0x10 + DAT_8c645a0c + 0xc) & DAT_8c645a14) == 0) break;
    uVar10 = uVar10 + 1;
  } while (uVar10 < (uint)(int)DAT_8c645a02);
  if (uVar10 < (uint)(int)DAT_8c645a02) {
    if ((iVar6 == 0) && (iVar9 == 0)) {
      if (iVar3 < DAT_8c645a04) {
        *(undefined **)((int)DAT_8c645a08 + in_VBR + (DAT_8c645a06 + iVar3 >> 3)) = PTR_FUN_8c645a18
        ;
      }
      else if (iVar3 == DAT_8c645a04) {
        (*(code *)PTR_FUN_8c645a20)(PTR_callback_8c645b1e_8c645a1c,0);
      }
      else if (iVar3 == DAT_8c645b44) {
        (*(code *)PTR_FUN_8c645b50)
                  (PTR_LAB_8c645b4c,0,
                   (short)((((uint)in_T | (uint)in_S << 1 | (uint)in_IMASK << 4) & 0xf0) >> 4),
                   &DAT_8c67341c);
      }
      else {
        *(undefined **)(DAT_8c645b46 + in_VBR) = PTR_LAB_8c645b54;
      }
      puVar7 = (undefined4 *)(uVar10 * 0x10 + iVar1);
      *(undefined4 **)(iVar4 * 4 + iVar2) = puVar7;
      puVar7[1] = 0;
      *puVar7 = param_2;
      puVar7[2] = param_4;
      puVar7[3] = param_3;
      uVar8 = (uint)(byte)((byte)param_1 >> 5);
      if (iVar3 >> 8 == 10) {
        *DAT_8c645b58 = *DAT_8c645b58 | 1 << uVar8 + 0xc;
      }
      else if (iVar3 >> 8 == 0xb) {
        *DAT_8c645b5c = *DAT_8c645b5c | 1 << uVar8;
      }
    }
    else {
      puVar7 = (undefined4 *)(uVar10 * 0x10 + DAT_8c645a0c);
      if (iVar6 == *(int *)(iVar4 * 4 + DAT_8c645a10)) {
        puVar7[1] = iVar6;
        *(undefined4 **)(iVar4 * 4 + iVar2) = puVar7;
      }
      else {
        *(undefined4 **)(iVar9 + 4) = puVar7;
        puVar7[1] = iVar6;
      }
      *puVar7 = param_2;
      puVar7[2] = param_4;
      puVar7[3] = param_3;
    }
  }
  else {
    uVar10 = 0;
  }
  return uVar10;
}

