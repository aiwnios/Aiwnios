U0 CheckOpcode(U8 *text,U8 *expected,I64 size)
{
  CCmpCtrl *cc=CmpCtrlNew(text,CCF_DONT_FREE_BUF,"Opcode");
  Lex(cc);
  CAiwniosAOTBlob *blob=PrsAsmBlk(cc);
  if (!blob || blob->len!=size || MemCmp(blob->data,expected,size)) {
    "FAIL: %s\n",text;
    if (blob) {
      I64 i;
      for (i=0;i<blob->len;i++) "%02x ",blob->data[i];
      "\n";
    }
    throw('AsmBytes');
  }
  AiwniosAOTBlobDel(blob);
  CmpCtrlDel(cc);
}

CheckOpcode("asm { BTS RAX,RCX; };","\x48\x0f\xab\xc8",4);
CheckOpcode("asm { LTR AX; };","\x48\x0f\x00\xd8",4);
CheckOpcode("asm { FXSAVE [RAX]; };","\x48\x0f\xae\x00",4);
CheckOpcode("asm { FXRSTOR [RAX]; };","\x48\x0f\xae\x08",4);
CheckOpcode("asm { FLD U32 [RAX]; };","\x48\xd9\x00",3);
CheckOpcode("asm { FLD U64 [RAX]; };","\x48\xdd\x00",3);
CheckOpcode("asm { FST U32 [RAX]; };","\x48\xd9\x10",3);
CheckOpcode("asm { FST U64 [RAX]; };","\x48\xdd\x10",3);
CheckOpcode("asm { FST ST1; };","\x48\xdd\xd1",3);
CheckOpcode("asm { FSTP U32 [RAX]; };","\x48\xd9\x18",3);
CheckOpcode("asm { FSTP U64 [RAX]; };","\x48\xdd\x18",3);
CheckOpcode("asm { FSTP ST1; };","\x48\xdd\xd9",3);
CheckOpcode("asm { TEST AL,1; };","\x48\xa8\x01",3);
"PASS: x86 opcodes\n";
Exit;
