/* Ghidra draft only; not buildable/verified source. */

undefined4
FUN_8c639c70(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,int param_5
            )

{
  undefined4 *puVar1;
  undefined4 in_r0;
  undefined8 *puVar2;
  undefined4 in_fr1;
  undefined4 in_fr3;
  undefined4 in_fr13;
  undefined4 in_fr15;
  undefined8 in_xd0;
  undefined8 in_xd2;
  undefined8 in_xd4;
  undefined8 in_xd6;
  undefined8 in_xd8;
  undefined8 in_xd10;
  undefined8 in_xd12;
  undefined8 in_xd14;
  char in_FPSCR_SZ;
  
  puVar1 = DAT_8c639cd4;
  if (*DAT_8c639c80 <= *DAT_8c639c84) {
    return 0;
  }
  *DAT_8c639c84 = *DAT_8c639c84 + 1;
  puVar2 = (undefined8 *)*puVar1;
  *(undefined4 *)puVar2 = in_r0;
  *(undefined4 *)(puVar2 + 4) = in_r0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar2 + 0x3c) = in_fr15;
    *(undefined4 *)(puVar2 + 7) = in_fr13;
    *(undefined4 *)((int)puVar2 + 0x34) = param_4;
    *(undefined4 *)(puVar2 + 6) = param_3;
    *(undefined4 *)((int)puVar2 + 0x2c) = param_2;
    *(undefined4 *)(puVar2 + 5) = param_1;
    *(undefined4 *)((int)puVar2 + 0x24) = in_fr3;
    puVar2 = puVar2 + 4;
    *(undefined4 *)puVar2 = in_fr1;
  }
  else {
    puVar2[7] = in_xd14;
    puVar2[6] = in_xd12;
    puVar2[5] = in_xd10;
    puVar2[4] = in_xd8;
    puVar2[3] = in_xd6;
    puVar2[2] = in_xd4;
    puVar2[1] = in_xd2;
    *puVar2 = in_xd0;
  }
  *puVar1 = puVar2 + 8;
  if (param_5 != 0) {
    return 1;
  }
  *(undefined4 *)(puVar2 + 8) = in_r0;
  *(undefined4 *)(puVar2 + 0xc) = in_r0;
  if (in_FPSCR_SZ == '\0') {
    *(undefined4 *)((int)puVar2 + 0x7c) = in_fr15;
    *(undefined4 *)(puVar2 + 0xf) = in_fr13;
    *(undefined4 *)((int)puVar2 + 0x74) = param_4;
    *(undefined4 *)(puVar2 + 0xe) = param_3;
    *(undefined4 *)((int)puVar2 + 0x6c) = param_2;
    *(undefined4 *)(puVar2 + 0xd) = param_1;
    *(undefined4 *)((int)puVar2 + 100) = in_fr3;
    *(undefined4 *)(puVar2 + 0xc) = in_fr1;
  }
  else {
    puVar2[0xf] = in_xd14;
    puVar2[0xe] = in_xd12;
    puVar2[0xd] = in_xd10;
    puVar2[0xc] = in_xd8;
    puVar2[0xb] = in_xd6;
    puVar2[10] = in_xd4;
    puVar2[9] = in_xd2;
    puVar2[8] = in_xd0;
  }
  return 1;
}

