#include "aiwn_lexparser.h"
#include "aiwn_x86.h"

uint8_t ErectREX(int64_t big, int64_t reg, int64_t index, int64_t base) {
  if (index == -1)
    index = 0;
  if (base == -1)
    base = 0;
  return 0b01000000 | (big << 3) | (!!(reg & 0b1000) << 2) |
         (!!(index & 0b1000) << 1) | !!(base & 0b1000);
}

int64_t Is32Bit(int64_t num) {
  return num < 0x7fFFffFFll && num > -0x7fFFffFFll;
}
uint8_t MODRMRegReg(int64_t a, int64_t b) {
  return (0b11 << 6) | ((a & 0b111) << 3) | (b & 0b111);
}
uint8_t MODRMRip32(int64_t a) {
  return (0b00 << 6) | ((a & 0b111) << 4) | 0b101;
}

uint8_t MODRMSIB(int64_t a) {
  return ((a & 0b111) << 3) | 0b100;
}

int64_t X86CmpRegReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x3b);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86CmovaRegReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x47);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}
int64_t X86CmovbRegReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x42);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86CmovlRegReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x4c);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}
int64_t X86CmovgRegReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x4f);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86CmpSIB64Imm(char *to, int64_t imm, int64_t s, int64_t i, int64_t b,
                       int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 7, s, i, b, o);
  ADD_U8(0x81);
  SIB_END();
  ADD_U32(imm);
  return len;
}

int64_t X86CmpSIB32Imm(char *to, int64_t imm, int64_t s, int64_t i, int64_t b,
                       int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, 7, s, i, b, o);
  ADD_U8(0x81);
  SIB_END();
  ADD_U32(imm);
  return len;
}

int64_t X86CmpSIB16Imm(char *to, int64_t imm, int64_t s, int64_t i, int64_t b,
                       int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, 7, s, i, b, o);
  ADD_U8(0x81);
  SIB_END();
  ADD_U16(imm);
  return len;
}

int64_t X86CmpSIB8Imm(char *to, int64_t imm, int64_t s, int64_t i, int64_t b,
                      int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, 7, s, i, b, o);
  ADD_U8(0x80);
  SIB_END();
  ADD_U8(imm);
  return len;
}

int64_t X86CmpRegImm(char *to, int64_t a, int64_t imm) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0x81);
  ADD_U8(MODRMRegReg(7, a));
  ADD_U32(imm);
  return len;
}

int64_t X86MovRegReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, b, 0, a));
  ADD_U8(0x89);
  ADD_U8(MODRMRegReg(b, a));
  return len;
}

int64_t X86CMovsRegReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x48);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86Leave(char *to, int64_t ul) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xc9);
  return len;
}

int64_t X86Ret(char *to, int64_t ul) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (!ul) {
    ADD_U8(0xc3);
  } else {
    ADD_U8(0xc2);
    ADD_U16(ul);
  }
  return len;
}

int64_t X86MovImm(char *to, int64_t a, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (((1ll << 32) - 1) >= off && off >= 0) {
    // Is unsigned 32bit
    ADD_U8(ErectREX(0, a, 0, a));
    ADD_U8(0xB8 + (a & 0x7));
    ADD_U32(off);
    return len;
  }
  if (((1ll << 32) - 1) / 2 >= off) {
    if (-((1ll << 32) - 1) / 2 <= off) {
      // Is Signed 32bit
      // REX.W C7 /0 id
      ADD_U8(ErectREX(1, 0, 0, a));
      ADD_U8(0xc7);
      ADD_U8(MODRMRegReg(0, a));
      ADD_U32(off);
      return len;
    }
  }
  // REX.W + B8+ tf io: movabs
  ADD_U8(ErectREX(1, a, 0, a));
  ADD_U8(0xB8 + (a & 0x7));
  ADD_U64(off);
  return len;
}

int64_t X86AddIndir64Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, r, s, i, b, o);
  ADD_U8(0x1);
  SIB_END();
  return len;
}
int64_t X86AddIndir16Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, r, s, i, b, o);
  ADD_U8(0x1);
  SIB_END();
  return len;
}

int64_t X86AddIndir32Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, r, s, i, b, o);
  ADD_U8(0x1);
  SIB_END();
  return len;
}

int64_t X86MovF64RIP(char *to, int64_t a, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xF2);
  SIB_BEGIN(1, a, -1, -1, RIP, off);
  ADD_U8(0x0F);
  ADD_U8(0x10);
  SIB_END();
  return len;
}

int64_t X86AddImm32(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0x81);
  ADD_U8(MODRMRegReg(0, a));
  ADD_U32(b);
  return len;
}

int64_t X86AddIndir64Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 0, s, i, b, o);
  ADD_U8(0x81);
  SIB_END();
  ADD_U32(imm);
  return len;
}
int64_t X86AddIndir32Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, 0, s, i, b, o);
  ADD_U8(0x81);
  SIB_END();
  ADD_U32(imm);
  return len;
}
int64_t X86AddIndir16Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, 0, s, i, b, o);
  ADD_U8(0x81);
  SIB_END();
  ADD_U16(imm);
  return len;
}
int64_t X86AddIndir8Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                          int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, 0, s, i, b, o);
  ADD_U8(0x80);
  SIB_END();
  ADD_U8(imm);
  return len;
}
//
int64_t X86SubIndir64Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 5, s, i, b, o);
  ADD_U8(0x81);
  SIB_END();
  ADD_U32(imm);
  return len;
}
int64_t X86SubIndir32Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, 5, s, i, b, o);
  ADD_U8(0x81);
  SIB_END();
  ADD_U32(imm);
  return len;
}
int64_t X86SubIndir16Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, 5, s, i, b, o);
  ADD_U8(0x81);
  SIB_END();
  ADD_U16(imm);
  return len;
}
int64_t X86SubIndir8Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                          int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, 5, s, i, b, o);
  ADD_U8(0x80);
  SIB_END();
  ADD_U8(imm);
  return len;
}
//
int64_t X86SubIndir64Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, r, s, i, b, o);
  ADD_U8(0x29);
  SIB_END();
  return len;
}
int64_t X86SubIndir16Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, r, s, i, b, o);
  ADD_U8(0x29);
  SIB_END();
  return len;
}

int64_t X86SubIndir32Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, r, s, i, b, o);
  ADD_U8(0x29);
  SIB_END();
  return len;
}

int64_t X86SubImm32(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 5, 0, a));
  ADD_U8(0x81);
  ADD_U8(MODRMRegReg(5, a));
  ADD_U32(b);
  return len;
}

int64_t X86SubReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x2b);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86AddReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x03);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86AddSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                    int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, a, s, i, b, off);
  ADD_U8(0x03);
  SIB_END();
  return len;
}

int64_t X86SubSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                    int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, a, s, i, b, off);
  ADD_U8(0x2b);
  SIB_END();
  return len;
}

int64_t X86IncReg(char *to, int64_t a) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0xff);
  ADD_U8(MODRMRegReg(0, a));
  return len;
}

int64_t X86IncSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 0, s, i, b, off);
  ADD_U8(0xff);
  SIB_END();
  return len;
}

int64_t X86DecReg(char *to, int64_t a) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0xff);
  ADD_U8(MODRMRegReg(1, a));
  return len;
}

int64_t X86DecSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 1, s, i, b, off);
  ADD_U8(0xff);
  SIB_END();
  return len;
}

int64_t X86CVTTSD2SIRegSIB64(char *to, int64_t a, int64_t s, int64_t i,
                             int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  SIB_BEGIN(1, a, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0x2c);
  SIB_END();
  return len;
}

int64_t X86CVTTSD2SIRegReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x2c);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86CVTTSI2SDRegReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x2a);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86CVTTSI2SDRegSIB64(char *to, int64_t a, int64_t s, int64_t i,
                             int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  SIB_BEGIN(1, a, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0x2a);
  SIB_END();
  return len;
}

int64_t X86AndPDReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x54);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86AndPDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, a, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0x54);
  SIB_END();
  return len;
}

int64_t X86OrPDReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x56);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86OrPDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                     int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, a, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0x56);
  SIB_END();
  return len;
}

int64_t X86XorPDReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x57);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86XorPDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, a, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0x57);
  SIB_END();
  return len;
}

int64_t X86COMISDRegReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x2f);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86Test(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x85);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86AddSDReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x58);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86AddSDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  SIB_BEGIN(0, a, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0x58);
  SIB_END();
  return len;
}

int64_t X86SubSDReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x5c);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86SubSDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  SIB_BEGIN(0, a, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0x5c);
  SIB_END();
  return len;
}

int64_t X86RoundSD(char *to, int64_t a, int64_t b, int64_t mode) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x3a);
  ADD_U8(0x0b);
  ADD_U8(MODRMRegReg(a, b));
  ADD_U8(mode);
  return len;
}

int64_t X86SqrtSIB(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                   int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  SIB_BEGIN(0, a, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0x51);
  SIB_END();
  return len;
}
int64_t X86SqrtReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x51);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86MulSDReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x59);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86MulSDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  SIB_BEGIN(0, a, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0x59);
  SIB_END();
  return len;
}

int64_t X86DivSDReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x5E);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86DivSDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  SIB_BEGIN(0, a, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0x5e);
  SIB_END();
  return len;
}

int64_t X86AndReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x23);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86AndImm(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0x81);
  ADD_U8(MODRMRegReg(4, a));
  ADD_U32(b);
  return len;
}

int64_t X86AndSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                    int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, a, s, i, b, off);
  ADD_U8(0x23);
  SIB_END();
  return len;
}

int64_t X86OrReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x0b);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86OrSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                   int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, a, s, i, b, off);
  ADD_U8(0xb);
  SIB_END();
  return len;
}

int64_t X86OrImm(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0x81);
  ADD_U8(MODRMRegReg(1, a));
  ADD_U32(b);
  return len;
}

int64_t X86XorReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x33);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86XorSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                    int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, a, s, i, b, off);
  ADD_U8(0x33);
  SIB_END();
  return len;
}

int64_t X86XorImm(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0x81);
  ADD_U8(MODRMRegReg(6, a));
  ADD_U32(b);
  return len;
}

int64_t X86Cqo(char *to, int64_t unused) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, 0));
  ADD_U8(0x99);
  return len;
}

int64_t X86DivReg(char *to, int64_t a) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 6, 0, a));
  ADD_U8(0xf7);
  ADD_U8(MODRMRegReg(6, a));
  return len;
}

int64_t X86DivSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 6, s, i, b, off);
  ADD_U8(0xf7);
  SIB_END();
  return len;
}

int64_t X86IDivReg(char *to, int64_t a) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0xf7);
  ADD_U8(MODRMRegReg(7, a));
  return len;
}

int64_t X86IDivSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 7, s, i, b, off);
  ADD_U8(0xf7);
  SIB_END();
  return len;
}

int64_t X86SarRCX(char *to, int64_t a) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0xd3);
  ADD_U8(MODRMRegReg(7, a));
  return len;
}

int64_t X86SarRCXSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 7, s, i, b, off);
  ADD_U8(0xd3);
  SIB_END();
  return len;
}

int64_t X86SarImm(char *to, int64_t a, int64_t imm) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0xc1);
  ADD_U8(MODRMRegReg(7, a));
  ADD_U8(imm);
  return len;
}

int64_t X86SarImmSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 7, s, i, b, off);
  ADD_U8(0xc1);
  SIB_END();
  return len;
}

int64_t X86ShrRCX(char *to, int64_t a) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0xd3);
  ADD_U8(MODRMRegReg(5, a));
  return len;
}

int64_t X86ShrImm(char *to, int64_t a, int64_t imm) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0xc1);
  ADD_U8(MODRMRegReg(5, a));
  ADD_U8(imm);
  return len;
}

int64_t X86ShlRCX(char *to, int64_t a) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0xd3);
  ADD_U8(MODRMRegReg(4, a));
  return len;
}

int64_t X86ShlImm(char *to, int64_t a, int64_t imm) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0xc1);
  ADD_U8(MODRMRegReg(4, a));
  ADD_U8(imm);
  return len;
}

int64_t X86MulReg(char *to, int64_t a) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0xf7);
  ADD_U8(MODRMRegReg(4, a));
  return len;
}

int64_t X86MulSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 4, s, i, b, off);
  ADD_U8(0xf7);
  SIB_END();
  return len;
}

int64_t X86IMulReg(char *to, int64_t a) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 0, 0, a));
  ADD_U8(0xf7);
  ADD_U8(MODRMRegReg(5, a));
  return len;
}
int64_t X86IMulSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 5, s, i, b, off);
  ADD_U8(0xf7);
  SIB_END();
  return len;
}

int64_t X86IMul2Reg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0xaf);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}
int64_t X86IMul2RegImm(char *to, int64_t a, int64_t b, int64_t imm8) {
  int64_t len = 0;
  char buf[16];
  to = to ?: buf;
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x6b);
  ADD_U8(MODRMRegReg(a, b));
  ADD_U8(imm8);
  return len;
}

int64_t X86IMul2SIB64(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                      int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, r, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0xaf);
  SIB_END();
  return len;
}

int64_t X86Call32(char *to, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xe8);
  ADD_U32(off - 5);
  return len;
}

int64_t X86CallReg(char *to, int64_t reg) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 2, 0, reg));
  ADD_U8(0xff);
  ADD_U8(MODRMRegReg(2, reg));
  return len;
}

int64_t X86CallSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 2, s, i, b, o);
  ADD_U8(0xff);
  SIB_END();
  return len;
}

int64_t X86PushReg(char *to, int64_t reg) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (reg >= R8)
    ADD_U8(ErectREX(0, 0, 0, reg));
  ADD_U8(0x50 + (reg & 0x7));
  return len;
}

int64_t X86PushM16(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, 6, s, i, b, off);
  ADD_U8(0x66);
  ADD_U8(0xff);
  SIB_END();
  return len;
}
int64_t X86PushM32(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, 6, s, i, b, off);
  ADD_U8(0xff);
  SIB_END();
  return len;
}
int64_t X86PushM64(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 6, s, i, b, off);
  ADD_U8(0xff);
  SIB_END();
  return len;
}
int64_t X86PushImm(char *to, int64_t imm) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x68);
  ADD_U32(imm);
  return len;
}

int64_t X86PopReg(char *to, int64_t reg) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (reg >= R8)
    ADD_U8(ErectREX(0, 0, 0, reg));
  ADD_U8(0x58 + (reg & 0x7));
  return len;
}

int64_t X86CmpRegSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                       int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, a, s, i, b, o);
  ADD_U8(0x3b);
  SIB_END();
  return len;
}

int64_t X86CmpSIBReg64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                       int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, a, s, i, b, o);
  ADD_U8(0x39);
  SIB_END();
  return len;
}

int64_t X86MovQF64I64(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  ADD_U8(ErectREX(1, a, 0, b));
  ADD_U8(0x0F)
  ADD_U8(0x6e);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86LeaSIB(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                  int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, a, s, i, b, off);
  ADD_U8(0x8D);
  SIB_END();
  return len;
}

int64_t X86MovQI64F64(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  ADD_U8(ErectREX(1, b, 0, a));
  ADD_U8(0x0F)
  ADD_U8(0x7e);
  ADD_U8(MODRMRegReg(b, a));
  return len;
}

int64_t X86Jmp(char *to, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xe9);
  ADD_U32(off - 5);
  return len;
}

int64_t X86JmpSIB(char *to, int64_t scale, int64_t idx, int64_t base,
                  int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, 4, scale, idx, base, off);
  ADD_U8(0xff);
  SIB_END();
  return len;
}

int64_t X86JmpReg(char *to, int64_t r) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (r >= R8)
    ADD_U8(ErectREX(0, 4, 0, r));
  ADD_U8(0xff);
  ADD_U8(MODRMRegReg(4, r));
  return len;
}

int64_t X86NegReg(char *to, int64_t r) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 3, 0, r));
  ADD_U8(0xf7);
  ADD_U8(MODRMRegReg(3, r));
  return len;
}

int64_t X86NegSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 3, s, i, b, off);
  ADD_U8(0xf7);
  SIB_END();
  return len;
}

int64_t X86NotReg(char *to, int64_t r) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 2, 0, r));
  ADD_U8(0xf7);
  ADD_U8(MODRMRegReg(2, r));
  return len;
}

int64_t X86NotSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 2, s, i, b, off);
  ADD_U8(0xf7);
  SIB_END();
  return len;
}

// MOVSD
int64_t X86MovRegRegF64(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0F)
  ADD_U8(0x10);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}

int64_t X86MovIndirRegF64(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  SIB_BEGIN(1, reg, scale, index, base, off);
  ADD_U8(0x0F);
  ADD_U8(0x11);
  SIB_END();
  return len;
}
int64_t X86MovRegIndirF64(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf2);
  SIB_BEGIN(1, reg, scale, index, base, off);
  ADD_U8(0x0F);
  ADD_U8(0x10);
  SIB_END();
  return len;
}

int64_t X86MovRegIndirDQU(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf3);
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x0F);
  ADD_U8(0x6f);
  SIB_END();
  return len;
}
int64_t X86MovIndirRegDQU(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf3);
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x0F);
  ADD_U8(0x7f);
  SIB_END();
  return len;
}
int64_t X86PackedShuffleD128(char *to, int64_t a, int64_t b, int64_t imm8) {
  int64_t len = 0;
  char buf[16];
  to = to ?: buf;
  ADD_U8(0x66);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x70);
  ADD_U8(MODRMRegReg(a, b));
  ADD_U8(imm8);
  return len;
}
// Packed-double ALU, reg-reg forms (mirror X86AddSDReg with 0x66 prefix).
int64_t X86AddPDReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x58);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}
int64_t X86MulPDReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x59);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}
int64_t X86UnpcklpdRegReg(char *to, int64_t a, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  if ((a & 0b1000) || (b & 0b1000))
    ADD_U8(ErectREX(0, a, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0x14);
  ADD_U8(MODRMRegReg(a, b));
  return len;
}
int64_t X86MovUPDRegSIB128(char *to, int64_t reg, int64_t scale, int64_t index,
                           int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x0F);
  ADD_U8(0x10);
  SIB_END();
  return len;
}
int64_t X86MovUPDSIBReg128(char *to, int64_t reg, int64_t scale, int64_t index,
                           int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x0F);
  ADD_U8(0x11);
  SIB_END();
  return len;
}

int64_t X86MovRegIndirI64(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, reg, scale, index, base, off);
  ADD_U8(0x8b);
  SIB_END();
  return len;
}

int64_t X86MovIndirI8Imm(char *to, int64_t imm, int64_t scale, int64_t index,
                         int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, 0, scale, index, base, off);
  ADD_U8(0xc6);
  SIB_END();
  ADD_U8(imm);
  return len;
}

int64_t X86MovIndirI16Imm(char *to, int64_t imm, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, 0, scale, index, base, off);
  ADD_U8(0xc7);
  SIB_END();
  ADD_U16(imm);
  return len;
}

int64_t X86MovIndirI32Imm(char *to, int64_t imm, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, 0, scale, index, base, off);
  ADD_U8(0xc7);
  SIB_END();
  ADD_U32(imm);
  return len;
}

int64_t X86MovIndirI64Imm(char *to, int64_t imm, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 0, scale, index, base, off);
  ADD_U8(0xc7);
  SIB_END();
  ADD_U32(imm);
  return len;
}

int64_t X86MovIndirRegI64(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, reg, scale, index, base, off);
  ADD_U8(0x89);
  SIB_END();
  return len;
}

int64_t X86XChgIndirReg32(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x87);
  SIB_END();
  return len;
}

int64_t X86XChgIndirReg8(char *to, int64_t reg, int64_t scale, int64_t index,
                         int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x86);
  SIB_END();
  return len;
}

int64_t X86XChgIndirReg16(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x87);
  SIB_END();
  return len;
}

int64_t X86XChgIndirReg64(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, reg, scale, index, base, off);
  ADD_U8(0x87);
  SIB_END();
  return len;
}

int64_t X86MovRegIndirI32(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x8b);
  SIB_END();
  return len;
}

int64_t X86MovIndirRegI32(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x89);
  SIB_END();
  return len;
}

int64_t X86MovRegIndirI16(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x8b);
  SIB_END();
  return len;
}

int64_t X86MovZXRegIndirI8(char *to, int64_t reg, int64_t scale, int64_t index,
                           int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, reg, scale, index, base, off);
  ADD_U8(0x0f);
  ADD_U8(0xb6);
  SIB_END();
  return len;
}

int64_t X86MovZXRegRegI8(char *to, int64_t reg, int64_t reg2) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, reg, 0, reg2))
  ADD_U8(0x0f);
  ADD_U8(0xb6);
  ADD_U8(MODRMRegReg(reg, reg2));
  return len;
}

int64_t X86MovZXRegIndirI16(char *to, int64_t reg, int64_t scale, int64_t index,
                            int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, reg, scale, index, base, off);
  ADD_U8(0x0f);
  ADD_U8(0xb7);
  SIB_END();
  return len;
}

int64_t X86MovSXRegIndirI8(char *to, int64_t reg, int64_t scale, int64_t index,
                           int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, reg, scale, index, base, off);
  ADD_U8(0x0f);
  ADD_U8(0xbe);
  SIB_END();
  return len;
}

int64_t X86MovSXRegIndirI16(char *to, int64_t reg, int64_t scale, int64_t index,
                            int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, reg, scale, index, base, off);
  ADD_U8(0x0f);
  ADD_U8(0xbf);
  SIB_END();
  return len;
}

int64_t X86MovSXRegIndirI32(char *to, int64_t reg, int64_t scale, int64_t index,
                            int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, reg, scale, index, base, off);
  ADD_U8(0x63);
  SIB_END();
  return len;
}

int64_t X86MovIndirRegI16(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x66);
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x89);
  SIB_END();
  return len;
}

int64_t X86MovRegIndirI8(char *to, int64_t reg, int64_t scale, int64_t index,
                         int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x88);
  SIB_END();
  return len;
}

int64_t X86MovIndirRegI8(char *to, int64_t reg, int64_t scale, int64_t index,
                         int64_t base, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(0, reg, scale, index, base, off);
  ADD_U8(0x88);
  SIB_END();
  return len;
}

int64_t X86BtRegReg(char *to, int64_t r, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, b, 0, r));
  ADD_U8(0x0f);
  ADD_U8(0xa3);
  ADD_U8(MODRMRegReg(b, r))
  return len;
}

int64_t X86BtSIBReg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                    int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, r, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0xa3);
  SIB_END();
  return len;
}

int64_t X86BtRegImm(char *to, int64_t r, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 4, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0xba);
  ADD_U8(MODRMRegReg(4, b))
  ADD_U8(b);
  return len;
}

int64_t X86BtSIBImm(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                    int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 4, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0xba);
  SIB_END();
  ADD_U8(r);
  return len;
}

int64_t X86BtcRegReg(char *to, int64_t r, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, b, 0, r));
  ADD_U8(0x0f);
  ADD_U8(0xbb);
  ADD_U8(MODRMRegReg(b, r))
  return len;
}

int64_t X86BtcSIBReg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, r, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0xbb);
  SIB_END();
  return len;
}

int64_t X86Lock(char *to, int64_t ul) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0xf0);
  return len;
}
int64_t X86BtcRegImm(char *to, int64_t r, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, r, 0, 7));
  ADD_U8(0x0f);
  ADD_U8(0xba);
  ADD_U8(MODRMRegReg(7, r))
  ADD_U8(b);
  return len;
}

int64_t X86BtcSIBImm(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 7, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0xba);
  SIB_END();
  ADD_U8(r);
  return len;
}

int64_t X86BtsRegReg(char *to, int64_t r, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, b, 0, r));
  ADD_U8(0x0f);
  ADD_U8(0xab);
  ADD_U8(MODRMRegReg(b, r))
  return len;
}

int64_t X86BtsSIBReg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, r, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0xab);
  SIB_END();
  return len;
}

int64_t X86BtsRegImm(char *to, int64_t r, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 5, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0xba);
  ADD_U8(MODRMRegReg(5, b))
  ADD_U8(b);
  return len;
}

int64_t X86BtsSIBImm(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 5, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0xba);
  SIB_END();
  ADD_U8(r);
  return len;
}

int64_t X86BtrRegReg(char *to, int64_t r, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, b, 0, r));
  ADD_U8(0x0f);
  ADD_U8(0xb3);
  ADD_U8(MODRMRegReg(b, r))
  return len;
}

int64_t X86BtrSIBReg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, r, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0xb3);
  SIB_END();
  return len;
}

int64_t X86BtrRegImm(char *to, int64_t r, int64_t b) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(ErectREX(1, 6, 0, b));
  ADD_U8(0x0f);
  ADD_U8(0xba);
  ADD_U8(MODRMRegReg(6, b))
  ADD_U8(b);
  return len;
}

int64_t X86BtrSIBImm(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  SIB_BEGIN(1, 6, s, i, b, off);
  ADD_U8(0x0f);
  ADD_U8(0xba);
  SIB_END();
  ADD_U8(r);
  return len;
}

int64_t X86AndIndirXImm(char *to, int64_t sz, int64_t imm, int64_t s, int64_t i,
                        int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (sz == 2)
    ADD_U8(0x66);
  SIB_BEGIN(sz == 8, 4, s, i, b, o);
  if (sz == 1)
    ADD_U8(0x80)
  else
    ADD_U8(0x81)
  SIB_END();
  switch (sz) {
    break;
  case 1:
    ADD_U8(imm);
    break;
  case 2:
    ADD_U16(imm);
    break;
  case 4:
    ADD_U32(imm);
    break;
  case 8:
    ADD_U32(imm);
  }
  return len;
}

int64_t X86XorIndirXImm(char *to, int64_t sz, int64_t imm, int64_t s, int64_t i,
                        int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (sz == 2)
    ADD_U8(0x66);
  SIB_BEGIN(sz == 8, 6, s, i, b, o);
  if (sz == 1)
    ADD_U8(0x80)
  else
    ADD_U8(0x81)
  SIB_END();
  switch (sz) {
    break;
  case 1:
    ADD_U8(imm);
    break;
  case 2:
    ADD_U16(imm);
    break;
  case 4:
    ADD_U32(imm);
    break;
  case 8:
    ADD_U32(imm);
  }
  return len;
}

int64_t X86XorIndirXReg(char *to, int64_t sz, int64_t r, int64_t s, int64_t i,
                        int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (sz == 2)
    ADD_U8(0x66);
  SIB_BEGIN(sz == 8, r, s, i, b, o);
  ADD_U8(0x31);
  SIB_END();
  return len;
}

int64_t X86AndIndirXReg(char *to, int64_t sz, int64_t r, int64_t s, int64_t i,
                        int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (sz == 2)
    ADD_U8(0x66);
  SIB_BEGIN(sz == 8, r, s, i, b, o);
  ADD_U8(0x21);
  SIB_END();
  return len;
}

int64_t X86ShlIndirXImm(char *to, int64_t sz, int64_t amt, int64_t s, int64_t i,
                        int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  switch (sz) {
  case 1:
    SIB_BEGIN(sz == 8, 4, s, i, b, o);
    ADD_U8(0xc0);
    SIB_END();
    break;
  case 2:
    ADD_U8(0x66);
  case 4:
  case 8:
    SIB_BEGIN(sz == 8, 4, s, i, b, o);
    ADD_U8(0xc1);
    SIB_END();
  }
  ADD_U8(amt);
  return len;
}

int64_t X86ShrIndirXImm(char *to, int64_t sz, int64_t amt, int64_t s, int64_t i,
                        int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  switch (sz) {
  case 1:
    SIB_BEGIN(sz == 8, 5, s, i, b, o);
    ADD_U8(0xc0);
    SIB_END();
    break;
  case 2:
    ADD_U8(0x66);
  case 4:
  case 8:
    SIB_BEGIN(sz == 8, 5, s, i, b, o);
    ADD_U8(0xc1);
    SIB_END();
  }
  ADD_U8(amt);
  return len;
}

int64_t X86SarIndirXImm(char *to, int64_t sz, int64_t amt, int64_t s, int64_t i,
                        int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  switch (sz) {
  case 1:
    SIB_BEGIN(sz == 8, 7, s, i, b, o);
    ADD_U8(0xc0);
    SIB_END();
    break;
  case 2:
    ADD_U8(0x66);
  case 4:
  case 8:
    SIB_BEGIN(sz == 8, 7, s, i, b, o);
    ADD_U8(0xc1);
    SIB_END();
  }
  ADD_U8(amt);
  return len;
}
int64_t X86OrIndirXReg(char *to, int64_t sz, int64_t r, int64_t s, int64_t i,
                       int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (sz == 2)
    ADD_U8(0x66);
  SIB_BEGIN(sz == 8, r, s, i, b, o);
  ADD_U8(0x09);
  SIB_END();
  return len;
}

int64_t X86OrIndirXImm(char *to, int64_t sz, int64_t imm, int64_t s, int64_t i,
                       int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (sz == 2)
    ADD_U8(0x66);
  SIB_BEGIN(sz == 8, 1, s, i, b, o);
  if (sz == 1)
    ADD_U8(0x80)
  else
    ADD_U8(0x81)
  SIB_END();
  switch (sz) {
    break;
  case 1:
    ADD_U8(imm);
    break;
  case 2:
    ADD_U16(imm);
    break;
  case 4:
    ADD_U32(imm);
    break;
  case 8:
    ADD_U32(imm);
  }
  return len;
}

int64_t X86Jcc(char *to, int64_t cond, int64_t off) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  ADD_U8(0x0f);
  switch (cond) {
    break;
  case X86_COND_A:
    ADD_U8(0x87);
    break;
  case X86_COND_AE:
    ADD_U8(0x83);
    break;
  case X86_COND_B:
    ADD_U8(0x82);
    break;
  case X86_COND_BE:
    ADD_U8(0x86);
    break;
  case X86_COND_C:
    ADD_U8(0x82);
    break;
  case X86_COND_E:
    ADD_U8(0x84);
    break;
  case X86_COND_G:
    ADD_U8(0x8f);
    break;
  case X86_COND_GE:
    ADD_U8(0x8d);
    break;
  case X86_COND_L:
    ADD_U8(0x8c);
    break;
  case X86_COND_LE:
    ADD_U8(0x8e);
    break;
  case X86_COND_S:
    ADD_U8(0x88);
    break;
  case X86_COND_S | 1:
    ADD_U8(0x89);
    break;
  case X86_COND_A | 1:
    ADD_U8(0x86);
    break;
  case X86_COND_AE | 1:
    ADD_U8(0x82);
    break;
  case X86_COND_B | 1:
    ADD_U8(0x83);
    break;
  case X86_COND_BE | 1:
    ADD_U8(0x87);
    break;
  case X86_COND_C | 1:
    ADD_U8(0x83);
    break;
  case X86_COND_E | 1:
    ADD_U8(0x85);
    break;
  case X86_COND_G | 1:
    ADD_U8(0x8e);
    break;
  case X86_COND_GE | 1:
    ADD_U8(0x8c);
    break;
  case X86_COND_L | 1:
    ADD_U8(0x8d);
    break;
  case X86_COND_LE | 1:
    ADD_U8(0x8f);
  }
  ADD_U32(off - 6); //-6 as inst is 6 bytes long(RIP comes after INST);
  return len;
}
// 1st MOV RDI,1
//...
// 6th MOV R8,6
// PUSH 7
// CALL FOO
// ADD RSP,8 //To remove 7 from the stack

int64_t X86Setcc(char *to, int64_t cond, int64_t r) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if ((r & 0b1000) || r == RSI || r == RDI)
    ADD_U8(ErectREX(0, 0, 0, r))
  ADD_U8(0x0f);
  switch (cond) {
    break;
  case X86_COND_A:
    ADD_U8(0x97);
    break;
  case X86_COND_AE:
    ADD_U8(0x93);
    break;
  case X86_COND_B:
    ADD_U8(0x92);
    break;
  case X86_COND_BE:
    ADD_U8(0x96);
    break;
  case X86_COND_C:
    ADD_U8(0x92);
    break;
  case X86_COND_E:
    ADD_U8(0x94);
    break;
  case X86_COND_G:
    ADD_U8(0x9f);
    break;
  case X86_COND_GE:
    ADD_U8(0x9d);
    break;
  case X86_COND_L:
    ADD_U8(0x9c);
    break;
  case X86_COND_LE:
    ADD_U8(0x9e);

    break;
  case X86_COND_A | 1:
    ADD_U8(0x96);
    break;
  case X86_COND_AE | 1:
    ADD_U8(0x92);
    break;
  case X86_COND_B | 1:
    ADD_U8(0x93);
    break;
  case X86_COND_BE | 1:
    ADD_U8(0x97);
    break;
  case X86_COND_C | 1:
    ADD_U8(0x93);
    break;
  case X86_COND_E | 1:
    ADD_U8(0x95);
    break;
  case X86_COND_G | 1:
    ADD_U8(0x9e);
    break;
  case X86_COND_GE | 1:
    ADD_U8(0x9c);
    break;
  case X86_COND_L | 1:
    ADD_U8(0x9d);
    break;
  case X86_COND_LE | 1:
    ADD_U8(0x9f);
  }
  ADD_U8(MODRMRegReg(0, r));
  if (!to)
    len += X86MovZXRegRegI8(NULL, r, r);
  else
    len += X86MovZXRegRegI8(to + len, r, r);
  return len;
}

int64_t X86IncIndirX(char *to, int64_t sz, int64_t s, int64_t i, int64_t b,
                     int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (sz == 2)
    ADD_U8(0x66)
  SIB_BEGIN(sz == 8, 0, s, i, b, o);
  if (sz == 1) {
    ADD_U8(0xfe);
  } else
    ADD_U8(0xff)
  SIB_END();
  return len;
}
int64_t X86DecIndirX(char *to, int64_t sz, int64_t s, int64_t i, int64_t b,
                     int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (sz == 2)
    ADD_U8(0x66)
  SIB_BEGIN(sz == 8, 1, s, i, b, o);
  if (sz == 1) {
    ADD_U8(0xfe);
  } else
    ADD_U8(0xff)
  SIB_END();
  return len;
}
int64_t X86AddIndirXImm32(char *to, int64_t sz, int64_t imm, int64_t s,
                          int64_t i, int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (sz == 2)
    ADD_U8(0x66)
  SIB_BEGIN(sz == 8, 0, s, i, b, o);
  if (sz == 1) {
    ADD_U8(0x80);
  } else
    ADD_U8(0x81)
  SIB_END();
  switch (sz) {
  case 1:
    ADD_U8(imm);
    break;
  case 2:
    ADD_U16(imm);
    break;
  default:
    ADD_U32(imm);
    break;
  }
  return len;
}

int64_t X86SubIndirXImm32(char *to, int64_t sz, int64_t imm, int64_t s,
                          int64_t i, int64_t b, int64_t o) {
  int64_t len = 0;
  char buf[16];
  if (!to)
    to = buf;
  if (sz == 2)
    ADD_U8(0x66)
  SIB_BEGIN(sz == 8, 5, s, i, b, o);
  if (sz == 1) {
    ADD_U8(0x80);
  } else
    ADD_U8(0x81)
  SIB_END();
  switch (sz) {
  case 1:
    ADD_U8(imm);
    break;
  case 2:
    ADD_U16(imm);
    break;
  default:
    ADD_U32(imm);
    break;
  }
  return len;
}
