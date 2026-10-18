/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c908ed2) */
/* WARNING: Removing unreachable block (ram,0x0c908ec2) */
/* WARNING: Removing unreachable block (ram,0x0c908ee8) */

void callback_candidate_c908d80
               (undefined4 param_1,undefined4 param_2,int param_3,undefined4 param_4)

{
  undefined *puVar1;
  code *pcVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined8 *puVar5;
  float fVar6;
  undefined8 in_dr2;
  undefined4 uVar8;
  double dVar7;
  undefined4 uVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  undefined4 uVar12;
  double in_dr14;
  double dVar13;
  undefined8 in_xd4;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_c;
  
  if (in_FPSCR_SZ == '\0') {
    puVar5 = (undefined8 *)((int)&local_c + 4);
    local_c = CONCAT44(SUB84(in_dr14,0),(undefined4)local_c);
  }
  else {
    puVar5 = &local_c;
    local_c = in_xd14;
  }
  *(undefined4 *)((int)puVar5 + -4) = in_PR;
  uVar8 = (undefined4)((ulonglong)in_dr2 >> 0x20);
  dVar7 = (double)CONCAT44(uVar8,0x3f800000);
  if (in_FPSCR_PR == '\0') {
    dVar7 = (double)CONCAT44(uVar8,0x40000000);
  }
  else {
    dVar7 = dVar7 + dVar7;
  }
  *(undefined4 *)((int)puVar5 + -0x6c) = param_4;
  if (in_FPSCR_SZ == '\0') {
    in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),*(undefined4 *)(param_3 + 0xd8));
  }
  else {
    in_xd14 = *(undefined8 *)(param_3 + 0xd8);
  }
  puVar4 = (undefined4 *)((int)puVar5 + -0x68);
  *puVar4 = DAT_0c908fc0;
  *(undefined1 **)((int)puVar5 + -0x14) = (undefined1 *)((int)puVar5 + -0x44);
  *(undefined1 **)((int)puVar5 + -0x10) = (undefined1 *)((int)puVar5 + -0x54);
  *(undefined1 **)((int)puVar5 + -0xc) = (undefined1 *)((int)puVar5 + -100);
  *(undefined4 *)((int)puVar5 + -8) = 4;
  uVar8 = (undefined4)DAT_0c908fc4;
  if (in_FPSCR_SZ != '\0') {
    in_xd4 = CONCAT44(DAT_0c908fc4._4_4_,(undefined4)DAT_0c908fc4);
    uVar8 = param_1;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)*(undefined8 **)((int)puVar5 + -0x14) = uVar8;
    uVar10 = CONCAT44(DAT_0c908fc4._4_4_,uVar8);
  }
  else {
    **(undefined8 **)((int)puVar5 + -0x14) = in_xd4;
    uVar10 = CONCAT44(DAT_0c908fcc,DAT_0c908fc4._4_4_);
  }
  uVar8 = (undefined4)((ulonglong)uVar10 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)((int)puVar5 + -0x14) + 4) = uVar8;
    uVar11 = CONCAT44(DAT_0c908fcc,param_2);
  }
  else {
    *(undefined8 *)(*(int *)((int)puVar5 + -0x14) + 4) = uVar10;
    uVar11 = CONCAT44(PTR_PTR_0c908fd0,DAT_0c908fcc);
  }
  uVar12 = (undefined4)((ulonglong)uVar11 >> 0x20);
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)((int)puVar5 + -0x14) + 8) = uVar12;
  }
  else {
    *(undefined8 *)(*(int *)((int)puVar5 + -0x14) + 8) = uVar11;
  }
  **(undefined2 **)((int)puVar5 + -0xc) = 0;
  *(undefined2 *)(*(int *)((int)puVar5 + -0xc) + 2) = 0;
  **(undefined4 **)((int)puVar5 + -0x10) = *puVar4;
  uVar9 = (undefined4)uVar10;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)((int)puVar5 + -0x14) + 0xc) = uVar9;
  }
  else {
    *(undefined8 *)(*(int *)((int)puVar5 + -0x14) + 0xc) = in_xd4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)((int)puVar5 + -0x14) + 0x10) = uVar9;
  }
  else {
    *(undefined8 *)(*(int *)((int)puVar5 + -0x14) + 0x10) = in_xd4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)((int)puVar5 + -0x14) + 0x14) = uVar12;
  }
  else {
    *(undefined8 *)(*(int *)((int)puVar5 + -0x14) + 0x14) = uVar11;
  }
  *(undefined2 *)(*(int *)((int)puVar5 + -0xc) + 4) = 0;
  *(undefined2 *)(*(int *)((int)puVar5 + -0xc) + 6) = 0xff;
  *(undefined4 *)(*(int *)((int)puVar5 + -0x10) + 4) = *puVar4;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)((int)puVar5 + -0x14) + 0x18) = uVar8;
  }
  else {
    *(undefined8 *)(*(int *)((int)puVar5 + -0x14) + 0x18) = uVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)((int)puVar5 + -0x14) + 0x1c) = uVar9;
  }
  else {
    *(undefined8 *)(*(int *)((int)puVar5 + -0x14) + 0x1c) = in_xd4;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)((int)puVar5 + -0x14) + 0x20) = uVar12;
  }
  else {
    *(undefined8 *)(*(int *)((int)puVar5 + -0x14) + 0x20) = uVar11;
  }
  *(undefined2 *)(*(int *)((int)puVar5 + -0xc) + 8) = 0xff;
  *(undefined2 *)(*(int *)((int)puVar5 + -0xc) + 10) = 0xff;
  *(undefined4 *)(*(int *)((int)puVar5 + -0x10) + 8) = *puVar4;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)((int)puVar5 + -0x14) + 0x24) = uVar8;
  }
  else {
    *(undefined8 *)(*(int *)((int)puVar5 + -0x14) + 0x24) = uVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)((int)puVar5 + -0x14) + 0x28) = uVar8;
  }
  else {
    *(undefined8 *)(*(int *)((int)puVar5 + -0x14) + 0x28) = uVar10;
  }
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)(*(int *)((int)puVar5 + -0x14) + 0x2c) = uVar12;
  }
  else {
    *(undefined8 *)(*(int *)((int)puVar5 + -0x14) + 0x2c) = uVar11;
  }
  *(undefined2 *)(*(int *)((int)puVar5 + -0xc) + 0xc) = 0xff;
  *(undefined2 *)(*(int *)((int)puVar5 + -0xc) + 0xe) = 0;
  puVar1 = PTR_PTR_0c908fd0;
  *(undefined4 *)(*(int *)((int)puVar5 + -0x10) + 0xc) = *puVar4;
  if (in_FPSCR_PR == '\0') {
    dVar13 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),SUB84(in_dr14,0) * SUB84(dVar7,0));
  }
  else {
    dVar13 = in_dr14 * dVar7;
  }
  (*DAT_0c908fd4)(puVar1);
  uVar8 = (undefined4)((ulonglong)dVar7 >> 0x20);
  uVar12 = (undefined4)uVar11;
  (*DAT_0c908fdc)(DAT_0c908fd8);
  (*DAT_0c908fe0)(0,*(int *)((int)puVar5 + -0x6c) + 0xc);
  uVar9 = SUB84(dVar13,0);
  uVar10 = in_xd14;
  if (in_FPSCR_SZ == '\0') {
    in_xd14 = CONCAT44(uVar9,uVar12);
    uVar10 = CONCAT44(uVar9,uVar9);
  }
  (*DAT_0c908fe4)(-(float)uVar10,in_xd14,0);
  if (in_FPSCR_SZ == '\0') {
    dVar13 = (double)CONCAT44((int)((ulonglong)dVar13 >> 0x20),(undefined4)DAT_0c908fe8);
  }
  iVar3 = (*DAT_0c908fe8._4_4_)();
  if (in_FPSCR_PR == '\0') {
    fVar6 = (float)iVar3 * SUB84(dVar13,0);
    dVar7 = (double)CONCAT44(uVar8,fVar6);
    iVar3 = (int)fVar6;
  }
  else {
    dVar7 = (double)CONCAT44(uVar8,(float)iVar3) * dVar13;
    iVar3 = (int)dVar7;
  }
  uVar8 = (undefined4)((ulonglong)dVar7 >> 0x20);
  if (iVar3 != 0) {
    iVar3 = (*DAT_0c908fe8._4_4_)();
    if (in_FPSCR_PR == '\0') {
      iVar3 = (int)((float)iVar3 * SUB84(dVar13,0));
    }
    else {
      iVar3 = (int)((double)CONCAT44(uVar8,(float)iVar3) * dVar13);
    }
    (*DAT_0c908ff0)(0,iVar3);
  }
  (*DAT_0c908fe0)(0,*(int *)((int)puVar5 + -0x6c) + 0x84);
  pcVar2 = DAT_0c908ff4;
  (*DAT_0c908ff4)(0,8);
  (*pcVar2)(1,10);
  (*DAT_0c908ff8)(0xb);
  (*DAT_0c909000)((undefined1 *)((int)puVar5 + -0x14),4,DAT_0c908ffc);
  (*pcVar2)(0,8);
  (*pcVar2)(1,6);
  (*DAT_0c909004)(1);
  return;
}

