/* Ghidra draft only; not buildable/verified source. */

void FUN_8c6510a4(void)

{
  undefined *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined *puVar5;
  int *piVar6;
  undefined *puVar7;
  int iVar8;
  int iVar9;
  undefined *puVar10;
  int iVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  undefined4 uVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  
  puVar1 = PTR_DAT_8c6511e4;
  if (*(int *)PTR_DAT_8c6511e4 == 0) {
    *(undefined4 **)PTR_DAT_8c6511e4 = DAT_8c6511e8;
    (*(code *)PTR_FUN_8c6511ec)(*(undefined4 *)puVar1,0,(int)DAT_8c6511e0,0);
  }
  puVar3 = DAT_8c6511f0;
  puVar2 = DAT_8c6511e8;
  *DAT_8c6511f0 = 3;
  puVar1 = PTR_s_COPYRIGHT__C__NEC_Corporation_19_8c6511f4;
  puVar3[1] = 2;
  puVar3[2] = 6;
  puVar3[3] = 2;
  puVar3[4] = puVar1;
  *puVar2 = puVar3;
  puVar3 = DAT_8c6511f8;
  *DAT_8c6511f8 = 0;
  puVar4 = DAT_8c6511fc;
  *DAT_8c6511fc = 0;
  puVar2[0xf] = 0;
  puVar3[7] = 0;
  puVar1 = PTR_FUN_8c651200;
  puVar4[7] = 0;
  (*(code *)puVar1)();
  *DAT_8c651204 = 0;
  *DAT_8c651208 = 0;
  puVar5 = PTR_FUN_8c651210;
  puVar1 = PTR_s_RENDER_8c65120c;
  iVar11 = (*(code *)PTR_FUN_8c651210)(PTR_s_RENDER_8c65120c,PTR_s_VSYNCCount_8c651214,1);
  piVar6 = DAT_8c651218;
  *DAT_8c651218 = iVar11 + -1;
  *DAT_8c65121c = *piVar6;
  *DAT_8c651224 = DAT_8c651220;
  uVar16 = DAT_8c651228;
  *DAT_8c65122c = DAT_8c651228;
  *DAT_8c651230 = uVar16;
  *DAT_8c651234 = 0;
  *DAT_8c651238 = 0;
  *DAT_8c65123c = 0;
  *DAT_8c651240 = 3;
  puVar7 = PTR_FUN_8c651248;
  *DAT_8c651244 = 1;
  (*(code *)puVar7)();
  uVar12 = (*(code *)puVar5)(puVar1,PTR_s_RegionSize_8c65124c,(int)DAT_8c6511e2);
  uVar13 = (*(code *)puVar5)(puVar1,PTR_s_ObjListSize_8c651254,DAT_8c651250);
  uVar14 = (*(code *)puVar5)(puVar1,PTR_s_ISPParamSize_8c65125c,DAT_8c651258);
  uVar15 = (*(code *)puVar5)(puVar1,PTR_s_TSPParamSize_8c651260,DAT_8c651258);
  *DAT_8c651264 = uVar14 & 0xfffff800;
  *DAT_8c651268 = uVar15 & 0xfffff800;
  iVar8 = DAT_8c651270;
  iVar11 = DAT_8c65126c;
  iVar19 = (uVar12 & 0xfffff800) + (uVar13 & 0xfffff800);
  iVar18 = 0;
  iVar17 = 0;
  for (uVar13 = 0; puVar2 = DAT_8c651428, iVar9 = DAT_8c651420, uVar13 < 0x20;
      uVar13 = uVar13 + 0x10) {
    *(int *)(DAT_8c651420 + uVar13) = iVar17;
    *(uint *)(iVar9 + uVar13 + 4) = iVar17 + (uVar12 & 0xfffff800);
    *(int *)(iVar9 + uVar13 + 8) = iVar17 + iVar19;
    *(uint *)(iVar9 + uVar13 + 0xc) = (uVar14 & 0xfffff800) + iVar19 + iVar17;
    *(int *)(DAT_8c651424 + iVar18) = iVar11 - iVar17;
    iVar17 = iVar17 + iVar8;
    iVar18 = iVar18 + 4;
  }
  *DAT_8c651428 = 0;
  *DAT_8c65142c = 0;
  puVar3 = DAT_8c651430;
  *DAT_8c651430 = 0;
  puVar2[3] = 0xffffffff;
  puVar4 = DAT_8c651434;
  *DAT_8c651434 = 0xffffffff;
  puVar3[3] = 0xffffffff;
  puVar2[6] = 0;
  puVar4[3] = 0;
  puVar3[6] = 0;
  *DAT_8c651438 = 0;
  *DAT_8c65143c = 0;
  puVar10 = PTR_s_VertexBuffer_8c65144c;
  puVar7 = PTR_s_BufferBase_8c651448;
  uVar16 = DAT_8c651444;
  *DAT_8c651440 = 1;
  uVar16 = (*(code *)puVar5)(puVar10,puVar7,uVar16);
  *DAT_8c651450 = uVar16;
  uVar16 = (*(code *)puVar5)(PTR_s_VertexBuffer_8c65144c,PTR_s_BufferSize_8c651454,iVar8);
  *DAT_8c651458 = uVar16;
  uVar16 = (*(code *)puVar5)(puVar1,PTR_s_PoolingTAStatus_8c65145c,0);
  *DAT_8c651460 = uVar16;
  uVar16 = (*(code *)puVar5)(puVar1,PTR_s_RenderTimeOut_8c651464,10);
  *DAT_8c651468 = uVar16;
  uVar16 = (*(code *)puVar5)(puVar1,PTR_s_VertexTimeOut_8c65146c,10);
  *DAT_8c651470 = uVar16;
  uVar16 = (*(code *)puVar5)(puVar1,PTR_s_VSYNCTimeOut_8c651474,10);
  *DAT_8c651478 = uVar16;
  *DAT_8c65147c = 0;
  *DAT_8c651480 = 1;
  puVar2 = DAT_8c651484;
  *DAT_8c651484 = 0xf;
  *puVar2 = *puVar2;
  *DAT_8c651488 = 0;
  *DAT_8c65148c = 0;
  (*(code *)PTR_FUN_8c651490)();
  return;
}

