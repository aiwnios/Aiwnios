public _intern IC_STRLEN I64 TestStrLen(U8 *text);

U8 *TestText(I64 select)
{
  if (select)
    return "wrong";
  return "Hello, world!";
}

if (TestStrLen(TestText(0)) != 13) throw('StrLen');

I64 LengthInRdx(U8 *input)
{
  U8 reg RDX *text=input;
  return TestStrLen(text);
}
if (LengthInRdx("Hello, world!") != 13) throw('StrLen');
if (LengthInRdx("") != 0) throw('StrLen');
"PASS: StrLen\n";
Exit;
