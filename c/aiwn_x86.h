#pragma once
#include <stddef.h>
#include <stdint.h>

#define ADD_U8(v) to[len++] = (v);
#define ADD_U16(v)                                                             \
  {                                                                            \
    *(int16_t *)(to + len) = (v);                                              \
    len += 2;                                                                  \
  }
#define ADD_U32(v)                                                             \
  {                                                                            \
    *(int32_t *)(to + len) = (v);                                              \
    len += 4;                                                                  \
  }
#define ADD_U64(v)                                                             \
  {                                                                            \
    *(int64_t *)(to + len) = (v);                                              \
    len += 8;                                                                  \
  }

#define SIB_BEGIN(_64, r, s, i, b, o)                                          \
  do {                                                                         \
    int64_t R = (r), S = (s), I = (i), B = (b), O = (o);                       \
    switch (S) {                                                               \
    default:                                                                   \
      S = -1;                                                                  \
      break;                                                                   \
    case 1:                                                                    \
      S = 0;                                                                   \
      break;                                                                   \
    case 2:                                                                    \
      S = 1;                                                                   \
      break;                                                                   \
    case 4:                                                                    \
      S = 2;                                                                   \
      break;                                                                   \
    case 8:                                                                    \
      S = 3;                                                                   \
    }                                                                          \
    if (B == RIP) {                                                            \
      if (_64 || (I & 0b1000) || (B & 0b1000) || (R & 0b1000))                 \
        ADD_U8(ErectREX(_64, R, 0, 0));                                        \
    } else if (I != -1 || B == R12) {                                          \
      if (S == -1)                                                             \
        I = RSP;                                                               \
      if (_64 || (I & 0b1000) || (B & 0b1000) || (R & 0b1000))                 \
        ADD_U8(ErectREX(_64, R, I, B));                                        \
    } else {                                                                   \
      if (_64 || (I & 0b1000) || (B & 0b1000) || (R & 0b1000))                 \
        ADD_U8(ErectREX(_64, R, I, B));                                        \
    }
#define SIB_END()                                                              \
  if (I == -1 && B == -1) {                                                    \
    ADD_U8(((R & 0b111) << 3) | 0b100);                                        \
                                                                               \
    ADD_U8(0x25);                                                              \
    ADD_U32(O);                                                                \
    break;                                                                     \
  }                                                                            \
  if (I == -1 && R12 != B && B != RIP && B != RSP) {                           \
    if (!O && B != RBP)                                                        \
      ADD_U8(((R & 0b111) << 3) | (B & 0b111))                                 \
    else if (-0x7f <= O && O <= 0x7f) {                                        \
      ADD_U8(0b01000000 | ((R & 0b111) << 3) | (B & 0b111));                   \
      ADD_U8(O);                                                               \
    } else {                                                                   \
      ADD_U8(0b10000000 | ((R & 0b111) << 3) | (B & 0b111));                   \
      ADD_U32(O);                                                              \
    }                                                                          \
    break;                                                                     \
  }                                                                            \
  if (B == RIP) {                                                              \
    ADD_U8(((R & 0b111) << 3) | 0b101)                                         \
    ADD_U32(O - (len + 4));                                                    \
    break;                                                                     \
  } else {                                                                     \
    if (B != -1) {                                                             \
      ADD_U8(MODRMSIB((R & 0b111)) |                                           \
             (((-0x7f <= O && O <= 0x7f) ? 0b01 : 0b10) << 6))                 \
    } else {                                                                   \
      ADD_U8(MODRMSIB(R & 0b111));                                             \
    }                                                                          \
  }                                                                            \
  if (S == -1) {                                                               \
    ADD_U8((RSP << 3) | (B & 0b111));                                          \
  } else if (B == -1) {                                                        \
    ADD_U8((S << 6) | ((I & 0b111) << 3) | RBP);                               \
  } else {                                                                     \
    ADD_U8((S << 6) | ((I & 0b111) << 3) | (B & 0b111));                       \
  }                                                                            \
  if ((-0x7f <= O && O <= 0x7f) && B != -1) {                                  \
    ADD_U8(O);                                                                 \
  } else {                                                                     \
    ADD_U32(O);                                                                \
  }                                                                            \
  }                                                                            \
  while (0)                                                                    \
    ;

// Add 1 in first bit for oppositive
#define X86_COND_A  0x10
#define X86_COND_AE 0x20
#define X86_COND_B  0x30
#define X86_COND_BE 0x40
#define X86_COND_C  0x50
#define X86_COND_E  0x60
#define X86_COND_Z  X86_COND_E
#define X86_COND_G  0x70
#define X86_COND_GE 0x80
#define X86_COND_L  0x90
#define X86_COND_LE 0xa0
#define X86_COND_S  0xb0
#define X86_COND_NS 0xb1

int64_t Is32Bit(int64_t num);
int64_t X86AddImm32(char *to, int64_t a, int64_t b);
int64_t X86AddIndir16Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o);
int64_t X86AddIndir16Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o);
int64_t X86AddIndir32Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o);
int64_t X86AddIndir32Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o);
int64_t X86AddIndir64Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o);
int64_t X86AddIndir64Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o);
int64_t X86AddIndir8Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                          int64_t b, int64_t o);
int64_t X86AddIndirXImm32(char *to, int64_t sz, int64_t imm, int64_t s,
                          int64_t i, int64_t b, int64_t o);
int64_t X86AddPDReg(char *to, int64_t a, int64_t b);
int64_t X86AddReg(char *to, int64_t a, int64_t b);
int64_t X86AddSDReg(char *to, int64_t a, int64_t b);
int64_t X86AddSDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off);
int64_t X86AddSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                    int64_t off);
int64_t X86AndImm(char *to, int64_t a, int64_t b);
int64_t X86AndIndirXImm(char *to, int64_t sz, int64_t imm, int64_t s, int64_t i,
                        int64_t b, int64_t o);
int64_t X86AndIndirXReg(char *to, int64_t sz, int64_t r, int64_t s, int64_t i,
                        int64_t b, int64_t o);
int64_t X86AndPDReg(char *to, int64_t a, int64_t b);
int64_t X86AndPDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off);
int64_t X86AndReg(char *to, int64_t a, int64_t b);
int64_t X86AndSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                    int64_t off);
int64_t X86BtRegImm(char *to, int64_t r, int64_t b);
int64_t X86BtRegReg(char *to, int64_t r, int64_t b);
int64_t X86BtSIBImm(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                    int64_t off);
int64_t X86BtSIBReg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                    int64_t off);
int64_t X86BtcRegImm(char *to, int64_t r, int64_t b);
int64_t X86BtcRegReg(char *to, int64_t r, int64_t b);
int64_t X86BtcSIBImm(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off);
int64_t X86BtcSIBReg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off);
int64_t X86BtrRegImm(char *to, int64_t r, int64_t b);
int64_t X86BtrRegReg(char *to, int64_t r, int64_t b);
int64_t X86BtrSIBImm(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off);
int64_t X86BtrSIBReg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off);
int64_t X86BtsRegImm(char *to, int64_t r, int64_t b);
int64_t X86BtsRegReg(char *to, int64_t r, int64_t b);
int64_t X86BtsSIBImm(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off);
int64_t X86BtsSIBReg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                     int64_t off);
int64_t X86CMovsRegReg(char *to, int64_t a, int64_t b);
int64_t X86COMISDRegReg(char *to, int64_t a, int64_t b);
int64_t X86CVTTSD2SIRegReg(char *to, int64_t a, int64_t b);
int64_t X86CVTTSD2SIRegSIB64(char *to, int64_t a, int64_t s, int64_t i,
                             int64_t b, int64_t off);
int64_t X86CVTTSI2SDRegReg(char *to, int64_t a, int64_t b);
int64_t X86CVTTSI2SDRegSIB64(char *to, int64_t a, int64_t s, int64_t i,
                             int64_t b, int64_t off);
int64_t X86Call32(char *to, int64_t off);
int64_t X86CallReg(char *to, int64_t reg);
int64_t X86CallSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t o);
int64_t X86CmovaRegReg(char *to, int64_t a, int64_t b);
int64_t X86CmovbRegReg(char *to, int64_t a, int64_t b);
int64_t X86CmovgRegReg(char *to, int64_t a, int64_t b);
int64_t X86CmovlRegReg(char *to, int64_t a, int64_t b);
int64_t X86CmpRegImm(char *to, int64_t a, int64_t imm);
int64_t X86CmpRegReg(char *to, int64_t a, int64_t b);
int64_t X86CmpRegSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                       int64_t o);
int64_t X86CmpSIB16Imm(char *to, int64_t imm, int64_t s, int64_t i, int64_t b,
                       int64_t o);
int64_t X86CmpSIB32Imm(char *to, int64_t imm, int64_t s, int64_t i, int64_t b,
                       int64_t o);
int64_t X86CmpSIB64Imm(char *to, int64_t imm, int64_t s, int64_t i, int64_t b,
                       int64_t o);
int64_t X86CmpSIB8Imm(char *to, int64_t imm, int64_t s, int64_t i, int64_t b,
                      int64_t o);
int64_t X86CmpSIBReg64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                       int64_t o);
int64_t X86Cqo(char *to, int64_t unused);
int64_t X86DecIndirX(char *to, int64_t sz, int64_t s, int64_t i, int64_t b,
                     int64_t o);
int64_t X86DecReg(char *to, int64_t a);
int64_t X86DecSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86DivReg(char *to, int64_t a);
int64_t X86DivSDReg(char *to, int64_t a, int64_t b);
int64_t X86DivSDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off);
int64_t X86DivSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86IDivReg(char *to, int64_t a);
int64_t X86IDivSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86IMul2Reg(char *to, int64_t a, int64_t b);
int64_t X86IMul2RegImm(char *to, int64_t a, int64_t b, int64_t imm8);
int64_t X86IMul2SIB64(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                      int64_t off);
int64_t X86IMulReg(char *to, int64_t a);
int64_t X86IMulSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86IncIndirX(char *to, int64_t sz, int64_t s, int64_t i, int64_t b,
                     int64_t o);
int64_t X86IncReg(char *to, int64_t a);
int64_t X86IncSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86Jcc(char *to, int64_t cond, int64_t off);
int64_t X86Jmp(char *to, int64_t off);
int64_t X86JmpReg(char *to, int64_t r);
int64_t X86JmpSIB(char *to, int64_t scale, int64_t idx, int64_t base,
                  int64_t off);
int64_t X86LeaSIB(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                  int64_t off);
int64_t X86Leave(char *to, int64_t ul);
int64_t X86Lock(char *to, int64_t ul);
int64_t X86MovF64RIP(char *to, int64_t a, int64_t off);
int64_t X86MovImm(char *to, int64_t a, int64_t off);
int64_t X86MovIndirI16Imm(char *to, int64_t imm, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovIndirI32Imm(char *to, int64_t imm, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovIndirI64Imm(char *to, int64_t imm, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovIndirI8Imm(char *to, int64_t imm, int64_t scale, int64_t index,
                         int64_t base, int64_t off);
int64_t X86MovIndirRegDQU(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovIndirRegF64(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovIndirRegI16(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovIndirRegI32(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovIndirRegI64(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovIndirRegI8(char *to, int64_t reg, int64_t scale, int64_t index,
                         int64_t base, int64_t off);
int64_t X86MovQF64I64(char *to, int64_t a, int64_t b);
int64_t X86MovQI64F64(char *to, int64_t a, int64_t b);
int64_t X86MovRegIndirDQU(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovRegIndirF64(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovRegIndirI16(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovRegIndirI32(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovRegIndirI64(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86MovRegIndirI8(char *to, int64_t reg, int64_t scale, int64_t index,
                         int64_t base, int64_t off);
int64_t X86MovRegReg(char *to, int64_t a, int64_t b);
int64_t X86MovRegRegF64(char *to, int64_t a, int64_t b);
int64_t X86MovSXRegIndirI16(char *to, int64_t reg, int64_t scale, int64_t index,
                            int64_t base, int64_t off);
int64_t X86MovSXRegIndirI32(char *to, int64_t reg, int64_t scale, int64_t index,
                            int64_t base, int64_t off);
int64_t X86MovSXRegIndirI8(char *to, int64_t reg, int64_t scale, int64_t index,
                           int64_t base, int64_t off);
int64_t X86MovUPDRegSIB128(char *to, int64_t reg, int64_t scale, int64_t index,
                           int64_t base, int64_t off);
int64_t X86MovUPDSIBReg128(char *to, int64_t reg, int64_t scale, int64_t index,
                           int64_t base, int64_t off);
int64_t X86MovZXRegIndirI16(char *to, int64_t reg, int64_t scale, int64_t index,
                            int64_t base, int64_t off);
int64_t X86MovZXRegIndirI8(char *to, int64_t reg, int64_t scale, int64_t index,
                           int64_t base, int64_t off);
int64_t X86MovZXRegRegI8(char *to, int64_t reg, int64_t reg2);
int64_t X86MulPDReg(char *to, int64_t a, int64_t b);
int64_t X86MulReg(char *to, int64_t a);
int64_t X86MulSDReg(char *to, int64_t a, int64_t b);
int64_t X86MulSDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off);
int64_t X86MulSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86NegReg(char *to, int64_t r);
int64_t X86NegSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86NotReg(char *to, int64_t r);
int64_t X86NotSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86OrImm(char *to, int64_t a, int64_t b);
int64_t X86OrIndirXImm(char *to, int64_t sz, int64_t imm, int64_t s, int64_t i,
                       int64_t b, int64_t o);
int64_t X86OrIndirXReg(char *to, int64_t sz, int64_t r, int64_t s, int64_t i,
                       int64_t b, int64_t o);
int64_t X86OrPDReg(char *to, int64_t a, int64_t b);
int64_t X86OrPDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                     int64_t off);
int64_t X86OrReg(char *to, int64_t a, int64_t b);
int64_t X86OrSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                   int64_t off);
int64_t X86PackedShuffleD128(char *to, int64_t a, int64_t b, int64_t imm8);
int64_t X86PopReg(char *to, int64_t reg);
int64_t X86PushImm(char *to, int64_t imm);
int64_t X86PushM16(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86PushM32(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86PushM64(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86PushReg(char *to, int64_t reg);
int64_t X86Ret(char *to, int64_t ul);
int64_t X86RoundSD(char *to, int64_t a, int64_t b, int64_t mode);
int64_t X86SarImm(char *to, int64_t a, int64_t imm);
int64_t X86SarImmSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86SarIndirXImm(char *to, int64_t sz, int64_t amt, int64_t s, int64_t i,
                        int64_t b, int64_t o);
int64_t X86SarRCX(char *to, int64_t a);
int64_t X86SarRCXSIB64(char *to, int64_t s, int64_t i, int64_t b, int64_t off);
int64_t X86Setcc(char *to, int64_t cond, int64_t r);
int64_t X86ShlImm(char *to, int64_t a, int64_t imm);
int64_t X86ShlIndirXImm(char *to, int64_t sz, int64_t amt, int64_t s, int64_t i,
                        int64_t b, int64_t o);
int64_t X86ShlRCX(char *to, int64_t a);
int64_t X86ShrImm(char *to, int64_t a, int64_t imm);
int64_t X86ShrIndirXImm(char *to, int64_t sz, int64_t amt, int64_t s, int64_t i,
                        int64_t b, int64_t o);
int64_t X86ShrRCX(char *to, int64_t a);
int64_t X86SqrtReg(char *to, int64_t a, int64_t b);
int64_t X86SqrtSIB(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                   int64_t off);
int64_t X86SubImm32(char *to, int64_t a, int64_t b);
int64_t X86SubIndir16Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o);
int64_t X86SubIndir16Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o);
int64_t X86SubIndir32Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o);
int64_t X86SubIndir32Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o);
int64_t X86SubIndir64Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                           int64_t b, int64_t o);
int64_t X86SubIndir64Reg(char *to, int64_t r, int64_t s, int64_t i, int64_t b,
                         int64_t o);
int64_t X86SubIndir8Imm32(char *to, int64_t imm, int64_t s, int64_t i,
                          int64_t b, int64_t o);
int64_t X86SubIndirXImm32(char *to, int64_t sz, int64_t imm, int64_t s,
                          int64_t i, int64_t b, int64_t o);
int64_t X86SubReg(char *to, int64_t a, int64_t b);
int64_t X86SubSDReg(char *to, int64_t a, int64_t b);
int64_t X86SubSDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off);
int64_t X86SubSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                    int64_t off);
int64_t X86Test(char *to, int64_t a, int64_t b);
int64_t X86UnpcklpdRegReg(char *to, int64_t a, int64_t b);
int64_t X86XChgIndirReg16(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86XChgIndirReg32(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86XChgIndirReg64(char *to, int64_t reg, int64_t scale, int64_t index,
                          int64_t base, int64_t off);
int64_t X86XChgIndirReg8(char *to, int64_t reg, int64_t scale, int64_t index,
                         int64_t base, int64_t off);
int64_t X86XorImm(char *to, int64_t a, int64_t b);
int64_t X86XorIndirXImm(char *to, int64_t sz, int64_t imm, int64_t s, int64_t i,
                        int64_t b, int64_t o);
int64_t X86XorIndirXReg(char *to, int64_t sz, int64_t r, int64_t s, int64_t i,
                        int64_t b, int64_t o);
int64_t X86XorPDReg(char *to, int64_t a, int64_t b);
int64_t X86XorPDSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                      int64_t off);
int64_t X86XorReg(char *to, int64_t a, int64_t b);
int64_t X86XorSIB64(char *to, int64_t a, int64_t s, int64_t i, int64_t b,
                    int64_t off);
uint8_t ErectREX(int64_t big, int64_t reg, int64_t index, int64_t base);
uint8_t MODRMRegReg(int64_t a, int64_t b);
uint8_t MODRMRip32(int64_t a);
uint8_t MODRMSIB(int64_t a);
