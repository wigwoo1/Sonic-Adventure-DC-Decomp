/* Ghidra draft only; not buildable/verified source. */

undefined4 FUN_8c057b40(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int *piVar1;
  undefined8 *puVar2;
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
  
  piVar1 = DAT_8c057bac;
  if (*DAT_8c057b54 < *DAT_8c057b50) {
    *DAT_8c057b54 = *DAT_8c057b54 + 1;
    puVar2 = (undefined8 *)*piVar1;
    *piVar1 = (int)(puVar2 + 8);
    if (in_FPSCR_SZ == '\0') {
      *(undefined4 *)((int)puVar2 + 0x3c) = in_fr15;
      *(undefined4 *)(puVar2 + 7) = in_fr13;
      *(undefined4 *)((int)puVar2 + 0x34) = param_3;
      *(undefined4 *)(puVar2 + 6) = param_2;
      *(undefined4 *)((int)puVar2 + 0x2c) = param_1;
      *(undefined4 *)(puVar2 + 5) = 0;
      *(undefined4 *)((int)puVar2 + 0x24) = 0x3f800000;
      *(undefined4 *)(puVar2 + 4) = 0;
      *(undefined4 *)((int)puVar2 + 0x7c) = 0;
      *(undefined4 *)(puVar2 + 0xf) = 0;
      *(undefined4 *)((int)puVar2 + 0x74) = 0x3f800000;
      *(undefined4 *)(puVar2 + 0xe) = 0;
      *(undefined4 *)((int)puVar2 + 0x6c) = 0;
      *(undefined4 *)(puVar2 + 0xd) = 0;
      *(undefined4 *)((int)puVar2 + 100) = 0;
      *(undefined4 *)(puVar2 + 0xc) = 0x3f800000;
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
      puVar2[0xf] = 0x3f800000;
      puVar2[0xe] = 0;
      puVar2[0xd] = 0x3f80000000000000;
      puVar2[0xc] = 0;
      puVar2[0xb] = 0;
      puVar2[10] = 0x3f800000;
      puVar2[9] = 0;
      puVar2[8] = 0x3f80000000000000;
    }
    return 1;
  }
  return 0;
}

