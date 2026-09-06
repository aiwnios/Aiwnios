I64 CallValue(I64 value) { return value; }
F64 CallFloat(F64 value) { return value; }

I64 equal=CallValue(123456)==CallValue(123456);
I64 unequal=CallValue(123456)!=CallValue(123456);
if (!equal || unequal)
  throw('CallCmp');
if (CallValue(123456)!=CallValue(123456))
  throw('CallCmp');
if (CallValue(123456)==CallValue(654321))
  throw('CallCmp');
if (CallValue(1)!=CallValue(1) || CallValue(2)!=CallValue(2))
  throw('CallCmp');
if (!(CallValue(1)==CallValue(1) && CallValue(2)==CallValue(2)))
  throw('CallCmp');
if (CallFloat(1.5)!=CallFloat(1.5))
  throw('CallCmp');
if (CallFloat(1.5)==CallFloat(2.5))
  throw('CallCmp');

U0 CheckCallComparison()
{
  I64 frame[2];
  frame[0]=123456;
  frame[1]=654321;
  if (CallValue(frame[0])!=CallValue(frame[0]) ||
      CallValue(frame[1])!=CallValue(frame[1]))
    throw('CallCmp');
  if (CallValue(123456)!=CallValue(123456))
    throw('CallCmp');
  if (CallValue(123456)==CallValue(654321))
    throw('CallCmp');
  if (frame[0]!=123456 || frame[1]!=654321)
    throw('CallCmp');
}
CheckCallComparison();
"PASS: CallCompare\n";
Exit;
