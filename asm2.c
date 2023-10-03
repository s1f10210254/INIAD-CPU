// M32R assembler (V.22.01)
// 名前 (Name)：渡邊浩樹 Hiroki Watanabe　学籍番号 (ID)：10210254

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

// Instruction macros for code generation
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
#define aligndata()     ( ( (addr&0x00000003) != 0 ) ? addr = (addr&0xfffffffc)+4 : 0 )
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
    int L0, L1, L2, L3, L4, L5, L6, L7, L8, L9,
        L10, L11, L12, L13, L14, L15, L16, L17, L18, L19, L19E,
        L20E, L21, L22, L23, L24, L25, L26, L27, L28, L29, L30,
        L_TEST1, L_TEST2, L_S0, L_S1, L_STARTTEST, L_ENDTEST;

    for( pass = 0; pass < 2; pass++ ) { // Process the assembler code two times

// Sample code: Test program
    org(0);
// trap(1);
    LD24(R0,L_STARTTEST);
trap(4);
    aligncode();        // BL24 must be aligned to 4-byte boundary
    BL24(L_TEST1);      // Call TEST1
    aligncode();
    LD24(R0,L_TEST2);
    JL(R0);             // Call TEST2
    aligncode();
    LD24(R0,L_ENDTEST);
trap(4);
trap(0);
L_STARTTEST = addr;
    string("Start test.");
L_ENDTEST = addr;
    string("End of the test.");
    aligndata();

    aligncode();
L_TEST1 = addr;
    LDI(R0,1);          // Test No. R0 = 1, AND
    LD24(R1,0xABCDEF);
    LD24(R2,0xF0F0F0);
    LD24(R3,0xA0C0E0);
    AND(R2,R1);
    BEQ(R2,R3,L2);
trap(2); trap(6);
    aligncode();
L2 = addr;
    LDI(R0,2);          // Test No. R0 = 2, AND3
    LD24(R1,0xABCDEF);
    LD24(R3,0x00C0E0);
    AND3(R2,R1,0xF0F0);
    BEQ(R2,R3,L3);
trap(2); trap(6);
    aligncode();
L3 = addr;
    LDI(R0,3);          // Test No. R0 = 3, CMPI
    LDI(R1,-128);
    LDI(R2,0);
    LDI(R3,0);
    CMP(R1,R1);
    CMPI(R1,-127);
    BC(L4);
trap(2); trap(6);
    aligncode();
L4 = addr;
    LDI(R0,4);          // Test No. R0 = 4, CMPU
    LDI(R1,-1);
    LDI(R2,2);
    LDI(R3,0);
    CMP(R1,R2);
    CMPU(R1,R2);
    BNC(L5);
trap(2); trap(6);
    aligncode();
L5 = addr;
    LDI(R0,5);          // Test No. R0 = 5, CMPUI
    LDI(R1,-1);
    LDI(R2,0);
    LDI(R3,0);
    CMP(R1,R2);
    CMPUI(R1,0xFFF0);
    BNC24(L6);
trap(2); trap(6);
    aligncode();
L6 = addr;
    LDI(R0,6);          // Test No. R0 = 6, DIV
    LDI(R1,10);
    LDI(R2,-128);
    LDI(R3,-12);
    DIV(R2,R1);
    BEQ(R2,R3,L7);
trap(2); trap(6);
    aligncode();
L7 = addr;
    LDI(R0,7);          // Test No. R0 = 7, DIVU
    LDI(R1,35);
    LDI16(R2,3500);
    LDI(R3,100);
    DIVU(R2,R1);
    BEQ(R2,R3,L8);
trap(2); trap(6);
    aligncode();
L8 = addr;
    LDI(R0,8);          // Test No. R0 = 8, NEG
    LDI(R1,-8);
    LDI(R3,8);
    NEG(R2,R1);
    BEQ(R2,R3,L9);
trap(2); trap(6);
    aligncode();
L9 = addr;
    LDI(R0,9);          // Test No. R0 = 9, NOT
    LDI16(R1,0xAAAA);
    LDI16(R3,0x5555);
    NOT(R2,R1);
    BEQ(R2,R3,L10);
trap(2); trap(6);
    aligncode();
L10 = addr;
    LDI(R0,10);         // Test No. R0 = 10, REM
    LDI(R1,10);
    LDI(R2,-128);
    LDI(R3,-8);
    REM(R2,R1);
    BEQ(R2,R3,L11);
trap(2); trap(6);
    aligncode();
L11 = addr;
    LDI(R0,11);         // Test No. R0 = 11, REMU
    LDI(R1,100);
    LDI16(R2,3456);
    LDI(R3,56);
    REMU(R2,R1);
    BEQ(R2,R3,L12);
trap(2); trap(6);
    aligncode();
L12 = addr;
    LDI(R0,12);         // Test No. R0 = 12, SRA
    LDI(R1,1);
    LDI(R2,-4);
    LDI(R3,-2);
    SRA(R2,R1);
    BEQ(R2,R3,L13);
trap(2); trap(6);
    aligncode();
L13 = addr;
    LDI(R0,13);         // Test No. R0 = 13, SRA3
    LDI16(R1,0xFEDC);
    LDI16(R3,0xFFED);
    SRA3(R2,R1,4);
    BEQ(R2,R3,L14);
trap(2); trap(6);
    aligncode();
L14 = addr;
    LDI(R0,14);         // Test No. R0 = 14, SRAI
    LDI(R1,0);
    LDI16(R2,0xFEDC);
    LDI16(R3,0xFFFE);
    SRAI(R2,8);
    BEQ(R2,R3,L15);
trap(2); trap(6);
    aligncode();
L15 = addr;
    LDI(R0,15);         // Test No. R0 = 15, SRL
    LDI(R1,12);
    SETH(R2,0xFEDC);
    OR3(R2,R2,0xBA98);  // R2 = 0xFEDCBA98
    LD24(R3,0x0FEDCB);
    SRL(R2,R1);
    BEQ(R2,R3,L16);
trap(2); trap(6);
    aligncode();
L16 = addr;
    LDI(R0,16);         // Test No. R0 = 16, SRL3
    SETH(R1,0xFEDC);
    OR3(R1,R1,0xBA98);  // R1 = 0xFEDCBA98
    LD24(R3,0x00FEDC);
    SRL3(R2,R1,16);
    BEQ(R2,R3,L17);
trap(2); trap(6);
    aligncode();
L17 = addr;
    LDI(R0,17);         // Test No. R0 = 17, SUB
    LDI(R1,10);
    LDI(R2,8);
    LDI(R3,-2);
    SUB(R2,R1);
    BEQ(R2,R3,L18);
trap(2); trap(6);
    aligncode();
L18 = addr;
    LDI(R0,18);         // Test No. R0 = 18, XOR
    LD24(R1,0x0B0D0F);
    LD24(R2,0xABCDEF);
    LD24(R3,0xA0C0E0);
    XOR(R2,R1);
    BEQ(R2,R3,L19);
trap(2); trap(6);
    aligncode();
L19 = addr;
    LDI(R0,19);         // Test No. R0 = 19, XOR3
    LD24(R1,0xABCDEF);
    LD24(R3,0xAB00EF);
    XOR3(R2,R1,0xCD00);
    BEQ(R2,R3,L19E);
trap(2); trap(6);
    aligncode();
L19E = addr;
    JMP(R14);

    aligncode();
L_TEST2 = addr;
    LDI(R0,20);         // Test No. R0 = 20, ST & LD
    SETH(R1,((L_S1>>16)&0xFFFF));
    OR3(R1,R1,(L_S1&0xFFFF));   // R1 = L_S1
    SETH(R2,0x5555);
    OR3(R2,R2,0x5555);          // R2 = 0x55555555
    SETH(R3,0xAAAA);
    OR3(R3,R3,0xAAAA);          // R3 = 0xAAAAAAAA
    ST_DEC(R2,R1);
    ST_INC(R3,R1);
    ADDI(R1,-4);
    LD_INC(R4,R1);
    LD_INC(R5,R1);
    ADDI(R1,-4);
    BNE(R4,R2,L20E);
    SUB(R5,R3);
    BNEZ(R5,L20E);
    BRA24(L21);
    aligncode();
L20E = addr;
trap(2); trap(6);
    BRA24(L21);
    aligndata();
L_S0 = addr;            // L_S0: 0x55555555
L_S1 = addr+4;          // L_S1: 0xAAAAAAAA
addr = addr+8;

    aligncode();
L21 = addr;
    LDI(R0,21);         // Test No. R0 = 21, ST & LDBR
    LDI(R2,0x12);
    STBR(R2,R1,-4);     // L_S0: 0x12555555
    LD24(R3,0x00CDEF);
    STHR(R3,R1,-2);     // L_S0: 0x1255CDEF
    STH(R3,R1);         // L_S1: 0xCDEFAAAA
    LDBR(R2,R1,-3);
    LD24(R3,0x000055);
    BEQ(R2,R3,L22);
trap(2); trap(6);
    aligncode();
L22 = addr;
    LDI(R0,22);         // Test No. R0 = 22, LDH
    LDI16(R3,0xCDEF);
    LDH(R2,R1);
    BEQ(R2,R3,L23);
trap(2); trap(6);
    aligncode();
L23 = addr;
    LDI(R0,23);         // Test No. R0 = 23, LDHR
    LDI16(R3,0xAAAA);
    LDHR(R2,R1,2);
    BEQ(R2,R3,L24);
trap(2); trap(6);
    aligncode();
L24 = addr;
    LDI(R0,24);         // Test No. R0 = 24, LDUB
    LDI16(R3,0x00CD);
    LDUB(R2,R1);
    BEQ(R2,R3,L25);
trap(2); trap(6);
    aligncode();
L25 = addr;
    LDI(R0,25);         // Test No. R0 = 25, LDUBR
    LDI16(R3,0x00EF);
    LDUBR(R2,R1,1);
    BEQ(R2,R3,L26);
trap(2); trap(6);
    aligncode();
L26 = addr;
    LDI(R0,26);         // Test No. R0 = 26, LDUH
    LD24(R3,0x00CDEF);
    LDUH(R2,R1);
    BEQ(R2,R3,L27);
trap(2); trap(6);
    aligncode();
L27 = addr;
    LDI(R0,27);         // Test No. R0 = 27, LDUHR
    LD24(R3,0x00AAAA);
    LDUHR(R2,R1,2);
    BEQ(R2,R3,L28);
trap(2); trap(6);
    aligncode();
L28 = addr;
    LDI(R0,28);         // Test No. R0 = 28, BGEZ
    SUB(R1,R1);
    BGEZ(R1,L29);
trap(2); trap(6);
    aligncode();
L29 = addr;
    LDI(R0,29);         // Test No. R0 = 29, BGEZ
    LDI(R1,127);
    BGEZ(R1,L30);
trap(2); trap(6);
    aligncode();
L30 = addr;
    LDI(R0,30);         // Test No. R0 = 30
    JMP(R14);

    }
    return 0;
}
