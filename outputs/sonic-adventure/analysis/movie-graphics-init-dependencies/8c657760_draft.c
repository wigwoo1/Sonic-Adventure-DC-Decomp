/* Ghidra draft only; not buildable/verified source. */

int FUN_8c657760(int param_1)

{
  undefined4 *puVar1;
  undefined *puVar2;
  uint *puVar3;
  uint *puVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  undefined4 *puVar9;
  
  puVar1 = DAT_8c657800;
  uVar8 = 2;
  if (((param_1 != 1) ||
      (iVar5 = (*(code *)PTR_FUN_8c657804)
                         (*DAT_8c657800,DAT_8c657800[1],DAT_8c657800[2],DAT_8c657800[3],
                          DAT_8c657800[4],0), iVar5 == 0)) &&
     ((param_1 != 2 || (iVar5 = (*(code *)PTR_FUN_8c657808)(*puVar1,puVar1[0x16]), iVar5 == 0)))) {
    puVar2 = PTR_FUN_8c65780c;
    if (puVar1[7] == puVar1[0xb]) {
      uVar8 = 1;
    }
    uVar7 = 0;
    iVar5 = 0;
    if (uVar8 != 0) {
      do {
        puVar9 = (undefined4 *)((int)puVar1 + iVar5 + 0x1c);
        iVar6 = (*(code *)puVar2)(*puVar9,puVar9[1],puVar9[2],puVar1[6],puVar9[3]);
        if (iVar6 != 0) {
          return iVar6;
        }
        uVar7 = uVar7 + 1;
        iVar5 = iVar5 + 0x10;
      } while (uVar7 < uVar8);
    }
    iVar5 = (*(code *)PTR_FUN_8c65790c)(puVar1[0xf],puVar1[0x10],puVar1[6],puVar1[0x11]);
    puVar4 = DAT_8c65791c;
    puVar3 = DAT_8c657918;
    if ((iVar5 == 0) && ((*DAT_8c657910 == 1 || (*(int *)PTR_DAT_8c657914 == 1)))) {
      *DAT_8c65791c = *DAT_8c657918;
      puVar4[*puVar3 - 6] = 0;
      *DAT_8c657920 = 1;
      puVar2 = PTR_FUN_8c657924;
      uVar8 = *DAT_8c657918;
      (*(code *)PTR_FUN_8c657924)(8,1);
      (*(code *)puVar2)(8,0);
      iVar6 = (uVar8 & 1) * 0x20;
      (*(code *)puVar2)((int)DAT_8c657900,*(undefined4 *)(DAT_8c657928 + iVar6));
      (*(code *)puVar2)((int)DAT_8c657902,*(undefined4 *)(DAT_8c65792c + iVar6));
      (*(code *)puVar2)((int)DAT_8c657904,*(undefined4 *)(DAT_8c657930 + iVar6));
      (*(code *)puVar2)((int)DAT_8c657906,*(undefined4 *)(DAT_8c657934 + iVar6));
      (*(code *)puVar2)((int)DAT_8c657908,*(undefined4 *)(DAT_8c657938 + iVar6));
      (*(code *)puVar2)((int)DAT_8c65790a,DAT_8c65793c);
      (*(code *)PTR_FUN_8c657940)((int)DAT_8c65790a);
      *DAT_8c657944 = 0;
      *DAT_8c65794c = *DAT_8c657948;
    }
  }
  return iVar5;
}

