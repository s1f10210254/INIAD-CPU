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
    int L_REVBIT, L_ATOI, L_STRCPY, L_FACT, L_NEXT, L_NEXT2, L_NEXT3, L_END,
        L0, L1, L2, L3, L4, L5, L6, L7, L8, L_S0, L_S1, L_S2, L_S3, L_S4, L_S5, L_S6, L_S7;

    for( pass = 0; pass < 2; pass++ ) { // Process the assembler code two times

// Call L_REVBIT
    org(0);
    LDI16(R0,L_S0);     // R0 = L_S0
trap(4);
    SETH(R0,0x1234);    // R0 = 0x12340000
    OR3(R0,R0,0x5678);  // R0 = 0x12345678
    SUB(R1,R1);         // R1 = 0
    SUB(R2,R2);         // R2 = 0
    SUB(R3,R3);         // R3 = 0
trap(2);
    BL(L_REVBIT);       // Save return address & branch to L_REVBIT
    aligncode();
trap(2);
trap(6);
    BRA(L_NEXT);
L_S0 = addr;
    string("Call function 1: Reverse 32-bit bit-order");
    aligndata();

// Call L_ATOI
    aligncode();
L_NEXT = addr;
    LDI16(R0,L_S1);     // R0 = L_S1
trap(4);
    LDI16(R0,L_S2);     // R0 = L_S2
    SUB(R1,R1);         // R1 = 0
    SUB(R2,R2);         // R2 = 0
    SUB(R3,R3);         // R3 = 0
trap(2);
trap(4);
    BL(L_ATOI);         // Save return address & branch to L_ATOI
    aligncode();
trap(2);
trap(6);
    BRA(L_NEXT2);
L_S1 = addr;
    string("Call function 2: Ascii string to integer (atoi)");
L_S2 = addr;
    string("654321");
    aligndata();

// Call L_STRCPY
    aligncode();
L_NEXT2 = addr;
    LDI16(R0,L_S3);     // R0 = L_S3
trap(4);
    LDI16(R0,L_S4);     // R0 = L_S4
    LDI16(R1,L_S5);     // R1 = L_S5
    SUB(R2,R2);         // R2 = 0
    SUB(R3,R3);         // R3 = 0
trap(5);
    BL(L_STRCPY);       // Save return address & branch to L_STRCPY
    aligncode();
trap(5);
trap(6);
    BRA(L_NEXT3);
L_S3 = addr;
    string("Call function 3: String copy (strcpy)");
L_S4 = addr;
    string("Hello world!");
L_S5 = addr;
    string("********************************");
    aligndata();

// Call L_FACT
    aligncode();
L_NEXT3 = addr;
    LDI16(R0,L_S6);     // R0 = L_S6
trap(4);
    LDI(R0,8);          // R0 = 8
    SUB(R1,R1);         // R1 = 0
    SUB(R2,R2);         // R2 = 0
    SUB(R3,R3);         // R3 = 0
trap(2);
    BL(L_FACT);         // Save return address & branch to L_FACT
    aligncode();
trap(2);
trap(6);
    BRA(L_END);
L_S6 = addr;
    string("Call function 4: Factorial");
    aligndata();

// Ending
    aligncode();
L_END = addr;
    LDI16(R0,L_S7);     // R0 = L_S7
trap(4);
trap(0);
L_S7 = addr;
    string("End.");
    aligndata();

// Function 1: Reverse 32-bit bit-order (L_REVBIT)
    aligncode();
L_REVBIT = addr;
    LDI(R1,0);          // R1 = 0
    MV(R2,R0);          // R2 = R0
    LDI(R3,32);         // R3 = 32
    aligncode();
L0 = addr;
    AND3(R4,R2,0x1);    // R4 = R2&0x1
    OR(R1,R4);          // R1 = R1|R4
    ADDI(R3,-1);        // R3 = R3-1
    BLEZ(R3,L1);        // if( R3<=0 ) goto L1
    SLLI(R1,1);         // R1 = R1<<1
    SRLI(R2,1);         // R2 = R2>>1
    BRA(L0);            // goto L0
    aligncode();
L1 = addr;
    JMP(R14);           // return

// Function 2: Ascii string to integer (L_ATOI)
    aligncode();
L_ATOI = addr;
    LDI(R1,0);          // R1 = 0
    MV(R2,R0);          // R2 = R0
    LDI(R3,10);         // R3 = 10
    aligncode();
L2 = addr;
    LDB(R4,R2);         // R4 = mem[R2]
    BEQZ(R4,L3);        // if( R4==0 ) goto L3
    ADD3(R5,R4,-'9');   // R5 = R4-'9'
    BGTZ(R5,L4);        // if( R5>0 ) goto L4
    ADDI(R4,-'0');      // R4 = R4-'0'
    BLTZ(R4,L4);        // if( R4<0 ) goto L4
    MUL(R1,R3);         // R1 = R1*R3 = R1*10
    ADD(R1,R4);         // R1 = R1+R4
    ADDI(R2,1);         // R2 = R2+1
    BRA(L2);            // goto L2
    aligncode();
L3 = addr;
    JMP(R14);           // return
    aligncode();
L4 = addr;
    LDI(R1,-1);         // R1 = -1
    JMP(R14);           // return

// Function 3: String copy (L_STRCPY)
    aligncode();
L_STRCPY = addr;
    MV(R2,R0);          // R2 = R0
    MV(R3,R1);          // R3 = R1
    aligncode();
L5 = addr;
    LDB(R4,R2);         // R4 = mem[R2]
    STB(R4,R3);         // mem[R3] = R4
    ADDI(R2,1);         // R2 = R2+1
    ADDI(R3,1);         // R3 = R3+1
    BEQZ(R4,L6);        // if( R4==0 ) goto L6
    BRA(L5);            // goto L5
    aligncode();
L6 = addr;
    JMP(R14);           // return

// Function 4: Factorial (L_FACT)
    aligncode();
L_FACT = addr;
    LDI(R1,1);          // R1 = 1
    MV(R2,R0);          // R2 = R0
    aligncode();
L7 = addr;
    BEQZ(R2,L8);        // if( R2==0 ) goto L8
    MUL(R1,R2);         // R1 = R1*R2
    ADDI(R2,-1);        // R2 = R2-1
    BRA(L7);            // goto L7
    aligncode();
L8 = addr;
    JMP(R14);           // return

    }
    return 0;
}
