/* Ghidra draft only; not buildable/verified source. */

/* WARNING: Removing unreachable block (ram,0x0c90c45a) */

void callback_candidate_c90c384(undefined4 param_1,int param_2,int param_3)

{
  float fVar1;
  undefined4 uVar2;
  undefined8 *puVar3;
  float fVar5;
  double dVar4;
  double in_dr14;
  undefined8 in_xd2;
  undefined8 in_xd14;
  undefined4 in_PR;
  char in_FPSCR_PR;
  char in_FPSCR_SZ;
  undefined8 local_c;
  
  if (in_FPSCR_SZ == '\0') {
    puVar3 = (undefined8 *)((int)&local_c + 4);
    local_c = CONCAT44(SUB84(in_dr14,0),(undefined4)local_c);
  }
  else {
    puVar3 = &local_c;
    local_c = in_xd14;
  }
  *(undefined4 *)((int)puVar3 + -4) = in_PR;
  *(int *)(param_3 + 0x74) = *(int *)(param_3 + 0x74) + -0x1e;
  if ((*(uint *)(param_3 + 0x60) & 2) == 0) {
    *(undefined2 *)(param_3 + 0x78) = 0;
  }
  else if (*(ushort *)(param_3 + 0x78) < 0x96) {
    *(int *)(param_3 + 0x74) = *(int *)(param_3 + 0x74) + 0x46;
    *(short *)(param_3 + 0x78) = *(short *)(param_3 + 0x78) + 1;
  }
  if (*(int *)(param_3 + 0x74) < 0x100) {
    if (*(int *)(param_3 + 0x74) < 0) {
      *(undefined4 *)(param_3 + 0x74) = 0;
    }
  }
  else {
    *(undefined4 *)(param_3 + 0x74) = 0xff;
  }
  if (10 < *(int *)(param_3 + 0x74)) {
    *(undefined1 *)((int)puVar3 + -0x12) = *(undefined1 *)(param_3 + 0x8c6);
    *(undefined1 *)((int)puVar3 + -0x13) = *(undefined1 *)(param_3 + 0x8c5);
    *(undefined1 *)((int)puVar3 + -0x14) = *(undefined1 *)(param_3 + 0x8c4);
    *(undefined1 *)((int)puVar3 + -0x11) = *(undefined1 *)(param_3 + 0x74);
    if (in_FPSCR_SZ == '\0') {
      fVar5 = *(float *)(param_3 + 0xd8);
    }
    else {
      fVar5 = (float)((ulonglong)*(undefined8 *)(param_3 + 0xd8) >> 0x20);
    }
    dVar4 = (double)CONCAT44(fVar5,(float)*(int *)(param_3 + 0x74));
    if (in_FPSCR_PR == '\0') {
      fVar5 = (float)*(int *)(param_3 + 0x74) * fVar5;
    }
    else {
      fVar5 = SUB84(dVar4 * dVar4,0);
    }
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)CONCAT44(DAT_0c90c540,fVar5);
      in_dr14 = (double)CONCAT44((int)((ulonglong)in_dr14 >> 0x20),fVar5);
      in_xd2 = in_xd14;
    }
    else {
      dVar4 = (double)CONCAT44(DAT_0c90c544,DAT_0c90c540);
    }
    if (in_FPSCR_PR == '\0') {
      fVar5 = SUB84(in_dr14,0) * (float)((ulonglong)dVar4 >> 0x20);
    }
    else {
      fVar5 = SUB84(in_dr14 * dVar4,0);
    }
    uVar2 = *(undefined4 *)(param_2 + 0x24);
    *(undefined4 *)((int)puVar3 + -0x10) = *(undefined4 *)(param_2 + 0x20);
    *(undefined4 *)((int)puVar3 + -0xc) = uVar2;
    if (in_FPSCR_SZ == '\0') {
      in_xd2 = CONCAT44(fVar5,param_1);
    }
    *(undefined4 *)((int)puVar3 + -8) = *(undefined4 *)(param_2 + 0x28);
    if (in_FPSCR_SZ == '\0') {
      dVar4 = (double)CONCAT44(*(undefined4 *)((int)puVar3 + -0xc),DAT_0c90c544);
      fVar1 = DAT_0c90c548;
    }
    else {
      dVar4 = *(double *)((int)puVar3 + -0xc);
      fVar1 = fRam0c90c54c;
    }
    if (in_FPSCR_PR == '\0') {
      fVar5 = fVar1 * fVar5 + SUB84(dVar4,0);
      dVar4 = (double)CONCAT44((float)((ulonglong)dVar4 >> 0x20) + fVar5,fVar5);
    }
    else {
      dVar4 = dVar4 + dVar4;
    }
    if (in_FPSCR_SZ == '\0') {
      *(int *)((int)puVar3 + -0xc) = (int)((ulonglong)dVar4 >> 0x20);
    }
    else {
      *(double *)((int)puVar3 + -0xc) = dVar4;
    }
    FUN_0c90c274(in_xd2,*(undefined4 *)(param_3 + 0xec),(undefined1 *)((int)puVar3 + -0x10),
                 (undefined1 *)((int)puVar3 + -0x14));
  }
  return;
}

