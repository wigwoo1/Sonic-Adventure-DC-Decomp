/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6461f0(void)

{
  undefined4 *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  undefined4 *puVar5;
  undefined4 *puVar6;
  undefined4 uVar7;
  int iVar8;
  
  puVar1 = DAT_8c6462c0;
  puVar3 = PTR_FUN_8c6462bc;
  puVar2 = PTR_DAT_8c6462b8;
  uVar7 = (*(code *)PTR_FUN_8c6462bc)
                    ((int)DAT_8c646298,PTR_LAB_8c6462c4,*(undefined4 *)PTR_DAT_8c6462b8,0,0x8c88e9a4
                    );
  puVar4 = PTR_LAB_8c6462c8;
  iVar8 = (int)DAT_8c64629a;
  *puVar1 = uVar7;
  uVar7 = (*(code *)puVar3)(iVar8,puVar4,*(undefined4 *)(puVar2 + 4),0);
  puVar4 = PTR_LAB_8c6462cc;
  iVar8 = (int)DAT_8c64629a;
  puVar1[1] = uVar7;
  uVar7 = (*(code *)puVar3)(iVar8,puVar4,*(undefined4 *)(puVar2 + 8),0);
  puVar4 = PTR_LAB_8c6462d0;
  iVar8 = (int)DAT_8c64629c;
  puVar1[2] = uVar7;
  uVar7 = (*(code *)puVar3)(iVar8,puVar4,*(undefined4 *)(puVar2 + 0xc),0);
  puVar5 = DAT_8c6462d4;
  puVar1[3] = uVar7;
  puVar6 = DAT_8c6462d8;
  puVar1 = DAT_8c6462ac;
  *puVar5 = 0;
  *puVar1 = 0;
  *puVar6 = 0;
  puVar2 = PTR_FUN_8c6462f8;
  *DAT_8c6462dc = 0;
  *DAT_8c6462e0 = 0;
  *DAT_8c6462e4 = 0;
  *DAT_8c6462e8 = 0xffffffff;
  *DAT_8c6462ec = 0;
  *DAT_8c6462f0 = 0;
  *DAT_8c6462f4 = 0;
  (*(code *)puVar2)(DAT_8c6462a8,0,0x38);
  (*(code *)puVar2)(DAT_8c6462b0,0,(int)DAT_8c64629e);
  (*(code *)puVar2)(DAT_8c6462fc,0,(int)DAT_8c64629e);
  (*(code *)puVar2)(DAT_8c646300,0,(int)DAT_8c6462a0);
  return;
}

