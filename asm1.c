// M32R assembler (V.22.01)
// 名前 (Name)：渡邊浩樹(Hiroki Watanabe)　学籍番号 (ID)：10210254

#include <stdio.h>


// Register name and register number 
#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define R8 8
#define R9 9
#define R10 10
#define R11 11
#define R12 12
#define R13 13
#define R14 14
#define R15 15

// Instruction macro-functions for code generation
#define ADD(d,s)        code_rr(0x00A0,d,s,"ADD")
#define ADD3(d,s,imm16)     code_rri(0x80A0,d,s,imm16,"ADD3")
#define ADDI(d,imm8)        code_rr(0x4000,d,(imm8)&0xff,"ADDI")
#define AND(d,s)        code_rr(0x00C0,d,s,"AND")
#define AND3(d,s,imm16)     code_rri(0x80C0,d,s,imm16,"AND3")
#define BC(label)       code_bc(0x7C00,label,"BC")
#define BC24(label)     code_bc24(0xFC00,label,"BC24")
#define BEQ(s1,s2,label)        code_beq(0xB000,s1,s2,label,"BEQ")
#define BEQZ(d,label)       code_beq(0xB080,0,d,label,"BEQZ")
#define BGEZ(d,label)       code_beq(0xB0B0,0,d,label,"BGEZ")
#define BGTZ(d,label)       code_beq(0xB0D0,0,d,label,"BGTZ")
#define BL(label)       code_bc(0x7E00,label,"BL")
#define BL24(label)     code_bc24(0xFE00,label,"BL24")
#define BLEZ(d,label)       code_beq(0xB0C0,0,d,label,"BLEZ")
#define BLTZ(d,label)       code_beq(0xB0A0,0,d,label,"BLTZ")
#define BNC(label)      code_bc(0x7D00,label,"BNC")
#define BNC24(label)        code_bc24(0xFD00,label,"BNC24")
#define BNE(s1,s2,label)        code_beq(0xB010,s1,s2,label,"BNE")
#define BNEZ(d,label)       code_beq(0xB090,0,d,label,"BNEZ")
#define BRA(label)      code_bc(0x7F00,label,"BRA")
#define BRA24(label)        code_bc24(0xFf00,label,"BRA24")
#define CMP(d,s)        code_rr(0x0040,d,s,"CMP")
#define CMPI(s,imm16)       code_rri(0x8040,0,s,imm16,"CMPI")
#define CMPU(d,s)       code_rr(0x0050,d,s,"CMPU")
#define CMPUI(s,imm16)      code_rri(0x8050,0,s,imm16,"CMPUI")
#define DIV(s,imm16)        code_rri(0x9000,s,imm16,0,"DIV")
#define DIVU(s,imm16)       code_rri(0x9010,s,imm16,0,"DIVU")
#define JL(s)       code_rr(0x1EC0,0,s,"JL")
#define JMP(s)      code_rr(0x1FC0,0,s,"JMP")
#define LD(d,s)     code_rr(0x20C0,d,s,"LD")
#define LD_INC(d,s)     code_rr(0x20E0,d,s,"LD_INC")
#define LDR(d,s,disp16)     code_rri(0xA0C0,d,s,disp16,"LDR")
#define LD24(d,imm24)       code_ld24(0xE000,d,imm24,"LD24")
#define LDB(d,s)        code_rr(0x2080,d,s,"LDB")
#define LDBR(d,s,disp16)        code_rri(0xA080,d,s,disp16,"LDBR")
#define LDH(d,s)        code_rr(0x20A0,d,s,"LDH")
#define LDHR(d,s,disp16)        code_rri(0xA0A0,d,s,disp16,"LDHR")
#define LDI(d,imm8)     code_rr(0x6000,d,(imm8)&0xff,"LDI")
#define LDI16(d,imm16)      code_rri(0x90F0,d,0,imm16,"LDI16")
#define LDUB(d,s)       code_rr(0x2090,d,s,"LDUB")
#define LDUBR(d,s,disp16)       code_rri(0xA090,d,s,disp16,"LDUBR")
#define LDUH(d,s)       code_rr(0x20B0,d,s,"LDUH")
#define LDUHR(d,s,disp16)       code_rri(0xA0B0,d,s,disp16,"LDUHR")
#define MUL(d,s)        code_rr(0x1060,d,s,"MUL")
#define MV(d,s)     code_rr(0x1080,d,s,"MV")
#define NEG(d,s)        code_rr(0x0030,d,s,"NEG")
#define NOP()       code_rr(0x7000,0,0,"NOP")
#define NOT(d,s)        code_rr(0x00B0,d,s,"NOT")
#define OR(d,s)     code_rr(0x00E0,d,s,"OR")
#define OR3(d,s,imm16)      code_rri(0x80E0,d,s,imm16,"OR3")
#define REM(d,s)        code_rri(0x9020,d,s,0x0000,"REM")
#define REMU(d,s)       code_rri(0x9030,d,s,0x0000,"REMU")
#define SETH(d,imm16)       code_rri(0xD0C0,d,0,imm16,"SETH")
#define SLL(d,s)        code_rr(0x1040,d,s,"SLL")
#define SLL3(d,s,imm16)     code_rri(0x90C0,d,s,imm16,"SLL3")
#define SLLI(d,imm5)        code_rr(0x5040,d,(imm5)&0x1f,"SLLI")
#define SRA(d,s)        code_rr(0x1020,d,s,"SRA")
#define SRA3(d,s,imm16)     code_rri(0x90A0,d,s,imm16,"SRA3")
#define SRAI(d,imm5)        code_rr(0x5020,d,(imm5)&0x1f,"SRAI")
#define SRL(d,s)        code_rr(0x1000,d,s,"SRL")
#define SRL3(d,s,imm16)     code_rri(0x9080,d,s,imm16,"SRL3")
#define SRLI(d,imm5)        code_rr(0x5000,d,(imm5)&0x1f,"SRLI")
#define ST(s1,s2)       code_rr(0x2040,s1,s2,"ST")
#define ST_INC(s1,s2)       code_rr(0x2060,s1,s2,"ST_INC")
#define ST_DEC(s1,s2)       code_rr(0x2070,s1,s2,"ST_DEC")
#define STR(s1,s2,disp16)       code_rri(0xA040,s1,s2,disp16,"STR")
#define STB(s1,s2)      code_rr(0x2000,s1,s2,"STB")
#define STBR(s1,s2,disp16)      code_rri(0xA000,s1,s2,disp16,"STBR")
#define STH(s1,s2)      code_rr(0x2020,s1,s2,"STH")
#define STHR(s1,s2,disp16)      code_rri(0xA020,s1,s2,disp16,"STHR")
#define SUB(s1,s2)      code_rr(0x0020,s1,s2,"SUB")
#define XOR(s1,s2)      code_rr(0x00D0,s1,s2,"XOR")
#define XOR3(d,s,imm16)     code_rri(0x80D0,d,s,imm16,"XOR3")
#define ADD(d,s)        code_rr(0x00A0,d,s,"ADD")
#define ADD3(d,s,imm16)     code_rri(0x80A0,d,s,imm16,"ADD3")
#define ADDI(d,imm8)        code_rr(0x4000,d,(imm8)&0xff,"ADDI")
#define AND(d,s)        code_rr(0x00C0,d,s,"AND")
#define AND3(d,s,imm16)     code_rri(0x80C0,d,s,imm16,"AND3")
#define BC(label)       code_bc(0x7C00,label,"BC")
#define BC24(label)     code_bc24(0xFC00,label,"BC24")
#define BEQ(s1,s2,label)        code_beq(0xB000,s1,s2,label,"BEQ")
#define BEQZ(d,label)       code_beq(0xB080,0,d,label,"BEQZ")
#define BGEZ(d,label)       code_beq(0xB0B0,0,d,label,"BGEZ")
#define BGTZ(d,label)       code_beq(0xB0D0,0,d,label,"BGTZ")
#define BL(label)       code_bc(0x7E00,label,"BL")
#define BL24(label)     code_bc24(0xFE00,label,"BL24")
#define BLEZ(d,label)       code_beq(0xB0C0,0,d,label,"BLEZ")
#define BLTZ(d,label)       code_beq(0xB0A0,0,d,label,"BLTZ")
#define BNC(label)      code_bc(0x7D00,label,"BNC")
#define BNC24(label)        code_bc24(0xFD00,label,"BNC24")
#define BNE(s1,s2,label)        code_beq(0xB010,s1,s2,label,"BNE")
#define BNEZ(d,label)       code_beq(0xB090,0,d,label,"BNEZ")
#define BRA(label)      code_bc(0x7F00,label,"BRA")
#define BRA24(label)        code_bc24(0xFf00,label,"BRA24")
#define CMP(d,s)        code_rr(0x0040,d,s,"CMP")
#define CMPI(s,imm16)       code_rri(0x8040,0,s,imm16,"CMPI")
#define CMPU(d,s)       code_rr(0x0050,d,s,"CMPU")
#define CMPUI(s,imm16)      code_rri(0x8050,0,s,imm16,"CMPUI")
#define DIV(s,imm16)        code_rri(0x9000,s,imm16,0,"DIV")
#define DIVU(s,imm16)       code_rri(0x9010,s,imm16,0,"DIVU")
#define JL(s)       code_rr(0x1EC0,0,s,"JL")
#define JMP(s)      code_rr(0x1FC0,0,s,"JMP")
#define LD(d,s)     code_rr(0x20C0,d,s,"LD")
#define LD_INC(d,s)     code_rr(0x20E0,d,s,"LD_INC")
#define LDR(d,s,disp16)     code_rri(0xA0C0,d,s,disp16,"LDR")
#define LD24(d,imm24)       code_ld24(0xE000,d,imm24,"LD24")
#define LDB(d,s)        code_rr(0x2080,d,s,"LDB")
#define LDBR(d,s,disp16)        code_rri(0xA080,d,s,disp16,"LDBR")
#define LDH(d,s)        code_rr(0x20A0,d,s,"LDH")
#define LDHR(d,s,disp16)        code_rri(0xA0A0,d,s,disp16,"LDHR")
#define LDI(d,imm8)     code_rr(0x6000,d,(imm8)&0xff,"LDI")
#define LDI16(d,imm16)      code_rri(0x90F0,d,0,imm16,"LDI16")
#define LDUB(d,s)       code_rr(0x2090,d,s,"LDUB")
#define LDUBR(d,s,disp16)       code_rri(0xA090,d,s,disp16,"LDUBR")
#define LDUH(d,s)       code_rr(0x20B0,d,s,"LDUH")
#define LDUHR(d,s,disp16)       code_rri(0xA0B0,d,s,disp16,"LDUHR")
#define MUL(d,s)        code_rr(0x1060,d,s,"MUL")
#define MV(d,s)     code_rr(0x1080,d,s,"MV")
#define NEG(d,s)        code_rr(0x0030,d,s,"NEG")
#define NOP()       code_rr(0x7000,0,0,"NOP")
#define NOT(d,s)        code_rr(0x00B0,d,s,"NOT")
#define OR(d,s)     code_rr(0x00E0,d,s,"OR")
#define OR3(d,s,imm16)      code_rri(0x80E0,d,s,imm16,"OR3")
#define REM(d,s)        code_rri(0x9020,d,s,0x0000,"REM")
#define REMU(d,s)       code_rri(0x9030,d,s,0x0000,"REMU")
#define SETH(d,imm16)       code_rri(0xD0C0,d,0,imm16,"SETH")
#define SLL(d,s)        code_rr(0x1040,d,s,"SLL")
#define SLL3(d,s,imm16)     code_rri(0x90C0,d,s,imm16,"SLL3")
#define SLLI(d,imm5)        code_rr(0x5040,d,(imm5)&0x1f,"SLLI")
#define SRA(d,s)        code_rr(0x1020,d,s,"SRA")
#define SRA3(d,s,imm16)     code_rri(0x90A0,d,s,imm16,"SRA3")
#define SRAI(d,imm5)        code_rr(0x5020,d,(imm5)&0x1f,"SRAI")
#define SRL(d,s)        code_rr(0x1000,d,s,"SRL")
#define SRL3(d,s,imm16)     code_rri(0x9080,d,s,imm16,"SRL3")
#define SRLI(d,imm5)        code_rr(0x5000,d,(imm5)&0x1f,"SRLI")
#define ST(s1,s2)       code_rr(0x2040,s1,s2,"ST")
#define ST_INC(s1,s2)       code_rr(0x2060,s1,s2,"ST_INC")
#define ST_DEC(s1,s2)       code_rr(0x2070,s1,s2,"ST_DEC")
#define STR(s1,s2,disp16)       code_rri(0xA040,s1,s2,disp16,"STR")
#define STB(s1,s2)      code_rr(0x2000,s1,s2,"STB")
#define STBR(s1,s2,disp16)      code_rri(0xA000,s1,s2,disp16,"STBR")
#define STH(s1,s2)      code_rr(0x2020,s1,s2,"STH")
#define STHR(s1,s2,disp16)      code_rri(0xA020,s1,s2,disp16,"STHR")
#define SUB(s1,s2)      code_rr(0x0020,s1,s2,"SUB")
#define XOR(s1,s2)      code_rr(0x00D0,s1,s2,"XOR")
#define XOR3(d,s,imm16)     code_rri(0x80D0,d,s,imm16,"XOR3")

int addr; // Memory address to place generated code and data
int pass; // Pass number of the assmbly process




// Pseudo inctruction functions
#define org(x)          ( addr = x )
#define aligncode()     ( ( addr%4 == 2 ) ? code_rr(0x7000,0,0,"NOP") : 0 )
#define aligndata()     ( ( addr%4 != 0 ) ? addr = (addr&0xfffffffc)+4 : 0 )
#define trap(n)         code_rr(0x10F0,0,n,"trap")

int string(char *s)
{
    char *p;

    for( p = s; *p != 0; addr++, p++ )
        if( pass > 0 )
            printf("%04x %02x // Byte data\n", addr, *p);
    if( pass > 0 )
        printf("%04x 00 // Byte data\n", addr);
    addr++;
    return 0;
}

// Code generator for the format of "*-*-" (Register-register)
int code_rr(int opcode, int m, int n, char *iname)
{
    if( pass > 0 )
        printf("%04x %04x // %s\n", addr, opcode|(m<<8)|n, iname);
    addr += 2;
    return 0;
}

// Code generator for the format of "*-*- imm16" (Register-register and 16-bit constant)
int code_rri(int opcode, int m, int n, int imm16, char *iname)
{
    if( pass > 0 )
        printf("%04x %04x%04x // %s\n", addr, opcode|(m<<8)|n, imm16&0xffff, iname);
    addr += 4;
    return 0;
}

// Code generator for the format of "**--" (Instruction format of BC)
int code_bc(int opcode, int label, char *iname)
{
    int pcdisp8;

    pcdisp8 = ((label&0xfffffffc)-(addr&0xfffffffc))>>2;
    if( pass > 0 )
        printf("%04x %04x // %s\n", addr, opcode|(pcdisp8&0xff), iname);
    addr += 2;
    return 0;
}

// Code generator for the format of "**-- pcdisp24" (Instruction format of BC24)
int code_bc24(int opcode, int label, char *iname)
{
    int pcdisp24;

    pcdisp24 = ((label&0xfffffffc)-(addr&0xfffffffc))>>2;
    if( pass > 0 )
        printf("%04x %04x%04x // %s\n", addr, opcode|((pcdisp24>>16)&0xff), pcdisp24&0xffff, iname);
    addr += 4;
    return 0;
}

// Code generator for the format of "*-*- pcdisp16" (Instruction format of BEQ)
int code_beq(int opcode, int m, int n, int label, char *iname)
{
    int pcdisp16;

    pcdisp16 = ((label&0xfffffffc)-(addr&0xfffffffc))>>2;
    if( pass > 0 )
        printf("%04x %04x%04x // %s\n", addr, opcode|(m<<8)|n, pcdisp16&0xffff, iname);
    addr += 4;
    return 0;
}

// Code generator for the format of "*--- imm24" (Instruction format of LD24)
int code_ld24(int opcode, int m, int imm24, char *iname)
{
    if( pass > 0 )
        printf("%04x %04x%04x // %s\n", addr, opcode|(m<<8)|((imm24>>16)&0xff), imm24&0xffff, iname);
    addr += 4;
    return 0;
}

// Main function of the M32R assmbler
int main()
{
    int L0, L1, L2, L3, L4, L5, L6, L7;

    for( pass = 0; pass < 2; pass++ ) { // Process the assembler code two times

// Sample code: Generate code for all instructions supported
    org(0);
trap(1);

L0 = addr;
    LDI(R0,0);      // R0 = 0
    LDI(R1,1);      // R1 = 1
    LDI(R2,2);      // R2 = 2
    LDI(R3,-3);     // R3 = -3
    LDI(R4,4);      // R4 = 4
    LDI(R5,5);      // R4 = 5
    LDI(R6,6);      // R4 = 6
    LDI(R7,7);      // R4 = 7
    LDI16(R8,8);    // R4 = 8
    LDI16(R9,9);    // R4 = 9
    LDI16(R10,10);  // R4 = 10
    LDI16(R11,11);  // R4 = 11
    LD24(R12,12);   // R4 = 12
    LD24(R13,13);   // R4 = 13
    LD24(R14,14);   // R4 = 14
    LD24(R15,15);   // R4 = 15
    ADD(R4,R5);
    ADD3(R6,R7,0x0fff);
    ADDI(R8,-2);
    AND(R9,R10);
    AND3(R11,R4,0xff00);
    CMP(R0,R0);     // C = 0
    BC(L0);
    BC24(L0);
    BEQ(R0,R1,L0);
    BEQZ(R2,L0);
    BGEZ(R3,L0);
    BGTZ(R0,L0);
    BL(L1);
    aligncode();
L1 = addr;
    BL24(L2);
    aligncode();
L2 = addr;
    BLEZ(R1,L0);
    BLTZ(R2,L0);
    CMP(R0,R1);     // C = 1
    BNC(L0);
    BNC24(L0);
    BNE(R3,R3,L0);
    BNEZ(R0,L0);
    BRA(L3);
    aligncode();
L3 = addr;
    BRA24(L4);
    aligncode();
L4 = addr;
    CMP(R4,R5);
    CMPI(R6,-4);
    CMPU(R7,R8);
    CMPUI(R9,0xABCD);
    DIV(R10,R3);
    DIVU(R4,R2);
    LDI16(R5,L5);
    JL(R5);
    aligncode();
L5 = addr;
    LD24(R6,L6);
    JMP(R6);
    aligncode();
L6 = addr;
    LDI16(R4,0);
    LD(R5,R4);
    LD_INC(R6,R4);
    LDR(R7,R4,-4);
    LD24(R5,0);
    LDB(R8,R5);
    LDBR(R9,R5,4);
    LDH(R10,R5);
    LDHR(R11,R5,4);
    LDUB(R12,R5);
    LDUBR(R13,R5,4);
    LDUH(R6,R4);
    LDUHR(R7,R4,4);
    MUL(R8,R9);
    MV(R10,R11);
    NEG(R12,R13);
    NOP();
    NOT(R5,R6);
    OR(R7,R8);
    OR3(R9,R10,0x1111);
    REM(R11,R3);
    REMU(R12,R2);
    SETH(R13,0x1234);
    SLL(R4,R5);
    SLL3(R5,R6,0x1);
    SLLI(R7,0x3);
    SRA(R8,R9);
    SRA3(R10,R11,0x5);
    SRAI(R12,0x7);
    SRL(R13,R4);
    SRL3(R5,R6,0x9);
    SRLI(R7,0xB);
    LDI16(R4,0x1234);   // R4 = 0x1234
    LDI16(R5,L7);       // R5 = L7
    ST(R4,R5);
    ST_INC(R1,R5);
    ST_DEC(R2,R5);
    STR(R3,R5,8);
    STB(R0,R5);
    STBR(R1,R5,12);
    STH(R2,R5);
    STHR(R3,R5,16);
    SUB(R6,R7);
    XOR(R8,R9);
    XOR3(R10,R11,0xf0f0);
trap(0);
L7 = addr;
    addr = addr+256;

    }
    return 0;
}
