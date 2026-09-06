public _intern IC_MOD_U64 U64 TestMod(U64 *q,U64 d);
U64 TestDivisor() {return 7;}
U64 q=100,r;
r=TestMod(&q,7);
if (q!=14 || r!=2) throw('ModConst');
q=100;
r=TestMod(&q,TestDivisor());
if (q!=14 || r!=2) throw('ModCall');
q=U64_MAX;
r=TestMod(&q,16);
if (q!=0x0FFFFFFFFFFFFFFF || r!=15) throw('ModWide');
"PASS: ModU64\n";
Exit;
