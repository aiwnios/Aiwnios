#exe { Cd("/Tests/Compiler/IncludeSearch/cwd"); }
#include "IncludeSearch/parent.HC"
if (INCLUDE_PRECEDENCE!=22 || INCLUDE_RELATIVE!=33)
  throw('Include');
"PASS: IncludeSearch\n";
Exit;
