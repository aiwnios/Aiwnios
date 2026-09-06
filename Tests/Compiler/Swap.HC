public _intern IC_SWAP_U8 U0 TestSwapU8(U8 *a,U8 *b);
public _intern IC_SWAP_U16 U0 TestSwapU16(U16 *a,U16 *b);
public _intern IC_SWAP_U32 U0 TestSwapU32(U32 *a,U32 *b);
public _intern IC_SWAP_I64 U0 TestSwapI64(I64 *a,I64 *b);
U8 a8=1,b8=255;
U16 a16=1,b16=65535;
U32 a32=1,b32=0xffffffff;
I64 a64=1,b64=0x123456789abcdef0;
TestSwapU8(&a8,&b8);
TestSwapU16(&a16,&b16);
TestSwapU32(&a32,&b32);
TestSwapI64(&a64,&b64);
if (a8!=255 || b8!=1 || a16!=65535 || b16!=1 ||
    a32!=0xffffffff || b32!=1 || a64!=0x123456789abcdef0 || b64!=1)
  throw('Swap');
"PASS: Swap widths\n";
Exit;
