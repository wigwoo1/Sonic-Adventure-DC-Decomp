/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c663140(void)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined *puVar7;
  int iVar8;
  undefined *puVar9;
  
  uVar5 = DAT_8c6631f8;
  puVar4 = PTR_DAT_8c6631f4;
  puVar9 = PTR_DAT_8c6631f0;
  puVar3 = PTR_DAT_8c6631ec;
  puVar2 = PTR_DAT_8c6631e8;
  puVar1 = PTR_DAT_8c6631e4;
  if (*(int *)PTR_DAT_8c6631fc == 1) {
    iVar8 = (*(code *)PTR_FUN_8c663208)(PTR_s_HARDWARE_8c663204,PTR_s_EnableTexture16MB_8c663200,0);
    uVar6 = DAT_8c66320c;
    if (iVar8 == 1) {
      *DAT_8c663210 = DAT_8c66320c;
      iVar8 = (int)DAT_8c6631e0;
      *(undefined4 *)PTR_DAT_8c663214 = uVar6;
      *(undefined4 *)puVar2 = 0;
      *(undefined4 *)puVar4 = uVar6;
      puVar7 = PTR_FUN_8c663218;
      *(undefined4 *)puVar1 = uVar6;
      *(undefined4 *)puVar3 = 0;
      (*(code *)puVar7)(iVar8,0);
    }
    else {
      *DAT_8c663210 = uVar5;
      puVar7 = PTR_FUN_8c663218;
      iVar8 = (int)DAT_8c6631e0;
      *(undefined4 *)PTR_DAT_8c663214 = uVar5;
      *(undefined4 *)puVar2 = 0;
      *(undefined4 *)puVar4 = uVar5;
      *(undefined4 *)puVar1 = uVar5;
      *(undefined4 *)puVar3 = 0;
      (*(code *)puVar7)(iVar8,0);
    }
    FUN_8c663080();
    *(undefined4 *)(puVar9 + 0x10) = *(undefined4 *)puVar4;
    puVar1 = PTR_FUN_8c66321c;
    *(undefined4 *)(puVar9 + 0x24) = *(undefined4 *)puVar3;
    iVar8 = (*(code *)puVar1)();
    if (iVar8 != 0) {
      *(undefined4 *)(iVar8 + 0xc) = *(undefined4 *)puVar2;
      *(undefined4 *)(iVar8 + 0x10) = *(undefined4 *)puVar4;
LAB_8c6632ac:
      *(undefined4 *)(iVar8 + 8) = *(undefined4 *)(puVar9 + 8);
      *(int *)(puVar9 + 8) = iVar8;
      *(undefined4 *)(iVar8 + 4) = *(undefined4 *)(puVar9 + 0xc);
      *(int *)(puVar9 + 0xc) = iVar8;
      return 0;
    }
  }
  else {
    iVar8 = (*(code *)PTR_FUN_8c6632ec)
                      (PTR_s_HARDWARE_8c6632e8,PTR_s_EnableTexture16MB_8c6632e4,0,100);
    if (iVar8 == 1) {
      *DAT_8c6632f4 = DAT_8c6632f0;
      puVar7 = PTR_FUN_8c6632fc;
      iVar8 = (int)DAT_8c6632e0;
      *(undefined4 *)PTR_DAT_8c6632f8 = uVar5;
      *(undefined4 *)puVar2 = 0;
      *(undefined4 *)puVar4 = uVar5;
      *(undefined4 *)puVar1 = uVar5;
      *(undefined4 *)puVar3 = uVar5;
      (*(code *)puVar7)(iVar8,0);
    }
    else {
      *DAT_8c6632f4 = uVar5;
      uVar5 = DAT_8c663300;
      puVar7 = PTR_FUN_8c6632fc;
      *(undefined4 *)PTR_DAT_8c6632f8 = DAT_8c663300;
      *(undefined4 *)puVar2 = 0;
      *(undefined4 *)puVar4 = uVar5;
      *(undefined4 *)puVar1 = uVar5;
      *(undefined4 *)puVar3 = uVar5;
      (*(code *)puVar7)((int)DAT_8c6632e0,0);
    }
    FUN_8c663080();
    *(undefined4 *)(puVar9 + 0x10) = *(undefined4 *)puVar4;
    puVar7 = PTR_FUN_8c663304;
    *(undefined4 *)(puVar9 + 0x24) = *(undefined4 *)puVar3;
    iVar8 = (*(code *)puVar7)();
    if (iVar8 != 0) {
      *(undefined4 *)(iVar8 + 0xc) = *(undefined4 *)puVar2;
      *(undefined4 *)(iVar8 + 0x10) = *(undefined4 *)puVar4;
      *(undefined4 *)(iVar8 + 8) = *(undefined4 *)(puVar9 + 8);
      puVar2 = PTR_FUN_8c663304;
      *(int *)(puVar9 + 8) = iVar8;
      *(undefined4 *)(iVar8 + 4) = *(undefined4 *)(puVar9 + 0xc);
      *(int *)(puVar9 + 0xc) = iVar8;
      iVar8 = (*(code *)puVar2)();
      if (iVar8 != 0) {
        puVar9 = puVar9 + 0x14;
        *(undefined4 *)(iVar8 + 0xc) = *(undefined4 *)puVar1;
        *(undefined4 *)(iVar8 + 0x10) = *(undefined4 *)puVar3;
        goto LAB_8c6632ac;
      }
    }
  }
  return 3;
}

