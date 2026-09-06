U0 CheckAliases(U8 *text,U8 *expected,I64 size)
{
  CCmpCtrl *cc=CmpCtrlNew(text,CCF_DONT_FREE_BUF,"Aliases");
  Lex(cc);
  CAiwniosAOTBlob *blob=PrsAsmBlk(cc);
  if (!blob || blob->len!=size || MemCmp(blob->data,expected,size)) {
    "FAIL: x86 aliases\n";
    throw('AsmBytes');
  }
  AiwniosAOTBlobDel(blob);
  CmpCtrlDel(cc);
}

U8 expected[31]={0x9c,0x66,0x9c,0x9d,0x66,0x9d,0x60,0x66,0x60,0x61,0x66,0x61,
      0x66,0x9c,0x9c,0x66,0x9d,0x9d,0x66,0x60,0x60,0x66,0x61,0x61,
      0x9b,0x48,0xdf,0xe0,0x9b,0xdf,0xe0};
CheckAliases("asm {\n"
  "USE16\n"
  "PUSHFW\nPUSHFD\nPOPFW\nPOPFD\nPUSHAW\nPUSHAD\nPOPAW\nPOPAD\n"
  "USE32\n"
  "PUSHFW\nPUSHFD\nPOPFW\nPOPFD\nPUSHAW\nPUSHAD\nPOPAW\nPOPAD\n"
  "USE64\n"
  "FSTSW AX\nFSTSW\nEND_ALIASES::\n};",expected,sizeof(expected));
"PASS: x86 aliases\n";
Exit;
