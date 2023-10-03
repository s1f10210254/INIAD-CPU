// M32R assembler (V.22.01)
// 名前 (Name)：渡邊浩樹 (Hiroki Watanabe)　学籍番号 (ID)：10210254

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MEMSIZE 4096 // Memory size (Number of bytes)
#define LINESIZE 256 // Max number of characters of input line

unsigned char mem[MEMSIZE]; //Memory
int regfile[16]; // Register file
int pc = 0; // Program counter
int cbit = 0; // C-bit (Condition bit)  

int trace = 0; // Trace control flag

// Skip space characters in the input line
char *skipspace(char *p)
{
    while( isspace(*p) )
        p++;
    return p;
}

// Translate a hexadecimal character to number
int toxdigit(char c)
{
    if( isdigit(c) )
        return c-'0';
    else if( islower(c) )
        return c-'a'+10;
    else
        return c-'A'+10;
}

// Load binary data represented in the text file
int loaddata(char *fname)
{
    FILE *fp; // File pointer
    char line[LINESIZE], *p;
    int addr, data, nbyte;

    fp = fopen(fname, "r"); // Open file.
    if( fp == NULL ) {
        printf("Cannot open file: %s\n", fname);
        return -1;
    }
    while( fgets(line, LINESIZE, fp) != NULL ) {
        for( addr = 0, p = skipspace(line); isxdigit(*p); p++ )
            addr = (addr<<4)+toxdigit(*p);
        for( p = skipspace(p); isxdigit(*p) && isxdigit(*(p+1)); p = p+2 )
            mem[addr++] = (toxdigit(*p)<<4)+toxdigit(*(p+1));
    }
    fclose(fp); // Close file.
    return 0;
}

#define get2bytes(a) ((mem[a]<<8)|mem[(a)+1])
#define get4bytes(a) ((mem[a]<<24)|(mem[(a)+1]<<16)|(mem[(a)+2]<<8)|mem[(a)+3])
#define put2bytes(a,d) ((mem[a]=(((d)>>8)&0xff)), (mem[(a)+1]=((d)&0xff))) 
#define put4bytes(a,d) ((mem[a]=(((d)>>24)&0xff)), (mem[(a)+1]=(((d)>>16)&0xff)), (mem[(a)+2]=(((d)>>8)&0xff)), (mem[(a)+3]=((d)&0xff))) 

// Simulate an instruction
int sim_ins()
{
    int icode1, icode2; // Store the first 16-bit code and the second 16-bit code 
    int nextpc; // Specifies the adress to fetch the next instruction code
    int rd, rs; // Store destination and source register numbers
    int a, d, i; // Working variables for address, data and etc.
    long long int ll; // Working variable for 64-bit int
    char *name; // Instruction name to output comment

    icode1 = get2bytes(pc); // Fetch the first 16-bit code
    if( (icode1&0x8000) == 0 ) { // If the MSB of the first 16-bit is 0, it is a 16-bit instruction
        nextpc = pc+2; // The next instruction code will be fetched from the address of pc+2
    } else { // If the MSB of the first 16-bit is 1, it is a 32-bit instruction
        icode2 = get2bytes(pc+2); // Fetch the second 16-bit code
        nextpc = pc+4; // The next instruction code will be fetched from the address of pc+4
    }

    rd = (icode1>>8)&0x0f;
    rs = icode1&0x0f;
    switch( icode1&0xf0f0 ) {
    case 0x00a0: // ADD
        regfile[rd] += regfile[rs];
        name = "ADD";
        break;
    case 0x80a0: // ADD3
        regfile[rd] = regfile[rs]+(short)icode2;
        name = "ADD3";
        break;
    case 0x00c0: // AND
        regfile[rd] &= regfile[rs];
        name = "AND";
        break;
    case 0x80c0: // AND3
        regfile[rd] = regfile[rs]&(unsigned short)icode2;
        name = "AND3";
        break;
    case 0xb000: // BEQ
        if( regfile[rd] == regfile[rs] )
            nextpc = (pc&0xfffffffc)+((short)icode2<<2);
        name = "BEQ";
        break;
    case 0xb010: // BNE
        if( regfile[rd] != regfile[rs] )
        nextpc = (pc&0xfffffffc)+((short)icode2<<2);
        name = "BNE";
        break;
    case 0x0040: // CMP
        cbit = ((regfile[rd] < regfile[rs]) ? 1 : 0);
        name = "CMP";
        break;
    case 0x0050: // CMPU
        cbit = (((unsigned)regfile[rd] < (unsigned)regfile[rs]) ? 1 : 0);
        name = "CMPU";
        break;
    case 0x9000: // DIV
        regfile[rd] = regfile[rd] / regfile[rs];
        name = "DIV";
        break;
    case 0x9010: // DIVU
        regfile[rd] = (unsigned)regfile[rd] / regfile[rs];
        name = "DIVU";
        break;
    case 0x20c0: // LD
        a = regfile[rs];
        regfile[rd] = get4bytes(a);
        name = "LD";
        break;
    case 0x20e0: // LD_INC
        a = regfile[rs];
        regfile[rs] += 4;
        regfile[rd] = get4bytes(a);
        name = "LD_INC";
        break;
    case 0xa0c0: // LDR
        a = regfile[rs]+(short)icode2;
        regfile[rd] = get4bytes(a);
        name = "LDR";
        break;
    case 0x2080: // LDB
        a = regfile[rs];
        regfile[rd] = (char)mem[a];
        name = "LDB";
        break;
    case 0xa080: // LDBR
        a = regfile[rs] + (short)icode2;
        regfile[rd] = (char)mem[a];
        name = "LDBR";
        break;
    case 0x20a0: // LDH
        a = regfile[rs];
        regfile[rd] = (short)get2bytes(a);
        name = "LDH";
        break;
    case 0xa0a0: // LDHR
        a = regfile[rs]+(short)icode2;
        regfile[rd] = (short)get2bytes(a);
        name = "LDHR";
        break;
    case 0x2090: // LDUB
        a = regfile[rs];
        regfile[rd] = (unsigned char)mem[a];
        name = "LDUB";
        break;
    case 0xa090: // LDUBR
        a = regfile[rs] +(short)icode2;
        regfile[rd] = (unsigned char)mem[a];
        name = "LDUBR";
        break;
    case 0x20b0: // LDUH
        a = regfile[rs];
        regfile[rd] = (unsigned short)get2bytes(a);
        name = "LDUH";
        break;
    case 0xa0b0: // LDUHR
        a = regfile[rs] + (short)icode2;
        regfile[rd] = (unsigned short)get2bytes(a);
        name = "LDUHR";
        break;
    case 0x1060: // MUL
        ll = (long long int)regfile[rd]*(long long int)regfile[rs];
        regfile[rd] = (int) ll;
        name = "MUL";
        break;
    case 0x1080: // MV
        regfile[rd] = regfile[rs];
        name = "MV";
        break;
    case 0x0030: // NEG
        regfile[rd] = 0-regfile[rs];
        name = "NEG";
        break;
    case 0x00b0: // NOT
        regfile[rd] = ~regfile[rs];
        name = "NOT";
        break;
    case 0x00e0: // OR
        regfile[rd] |= regfile[rs];
        name = "OR";
        break;
    case 0x80e0: // OR3
        regfile[rd] = regfile[rs] | (unsigned short)icode2;
        name = "OR3";
        break;
    case 0x9020: // REM
        regfile[rd] %= regfile[rs];
        name = "REM";
        break;
    case 0x9030: // REMU
        regfile[rd] = (unsigned)regfile[rd] % (unsigned)regfile[rs];
        name = "REMU";
        break;
    case 0x1040: // SLL
        regfile[rd] <<= (regfile[rs]&0x1f);
        name = "SLL";
        break;
    case 0x90c0: // SLL3
        regfile[rd] = regfile[rs]<<(icode2&0x1f);
        name = "SLL3";
        break;
    case 0x5040: // SLLI
    case 0x5050: // SLLI
        regfile[rd] <<= (icode1&0x1f);
        name = "SLLI";
        break;
    case 0x1020: // SRA
        regfile[rd] = regfile[rd]>>(regfile[rs]&0x1f);
        name ="SRA";
        break;
    case 0x90a0: // SRA3
        regfile[rd] = regfile[rs]>>(icode2&0x1f);
        name ="SRA3";
        break;
    case 0x5020: // SRAI
    case 0x5030: // SRAI
        regfile[rd] >>= (icode1&0x1f);
        name = "SRAI";
        break;
    case 0x1000: // SRL
        regfile[rd] = (unsigned)regfile[rd]>>(regfile[rs]&0x1f);
        name ="SRL";
        break;

    case 0x9080: // SRL3
        regfile[rd] = (unsigned)regfile[rs]>>(icode2&0x1f);
        break;
    case 0x5000: // SRLI
    case 0x5010: // SRLI
        regfile[rd] = (unsigned)regfile[rd]>>((icode1&0x1f));
        name = "SRLI";
        break;
    case 0x2040: // ST
        a = regfile[rs];
        d = regfile[rd];
        put4bytes(a,d);
        name = "ST";
        break;
    case 0x2060: // ST_INC
        regfile[rs] += 4;
        a = regfile[rs];
        d = regfile[rd];
        put4bytes(a,d);
        name = "ST_INC";
        break;

    case 0x2070: // ST_DEC
        regfile[rs] -= 4;
        a = regfile[rs];
        d = regfile[rd];
        put4bytes(a,d);
        name = "ST_DEC";
        break;

    case 0xa040: // STR
        a = regfile[rs]+(short)icode2;
        d = regfile[rd];
        put4bytes(a,d);
        name = "STR";
        break;

    case 0x2000: // STB
        a = regfile[rs];
        d = regfile[rd];
        mem[a] = d;
        name = "STB";
        break;

    case 0xa000: // STBR
        a = regfile[rs]+(short)icode2;
        d = regfile[rd];
        mem[a] = d;
        name = "STBR";
        break;

    case 0x2020: // STH
        a = (short)regfile[rs];
        d = regfile[rd];
        put2bytes(a,d);
        name = "STH";
        break;

    case 0xa020: // STHR
        a = regfile[rs]+(short)icode2;
        d = regfile[rd];
        put2bytes(a,d);
        name = "STHR";
        break;

    case 0x0020: // SUB
        regfile[rd] -= regfile[rs];
        name = "SUB";
        break;

    case 0x00d0: // XOR
        regfile[rd] ^= regfile[rs];
        name = "XOR";
        break;

    case 0x80d0: // XOR3
        regfile[rd] = regfile[rs]^(unsigned short)icode2;
        name = "XOR3";
        break;

    default:
        switch( icode1&0xff00 ) {
        case 0x7c00: // BC
            if( cbit == 1 )
                nextpc = (pc&0xfffffffc)+((char)(icode1&0x00ff)<<2);
            name = "BC";
            break;

        case 0xfc00: // BC24
            if( cbit == 1 )
                nextpc = (pc&0xfffffffc)+((((char)(icode1&0x00ff)<<16)|(unsigned short)icode2)<<2);
            name = "BC24";
            break;

        case 0x7e00: // BL
            regfile[14] = (pc&0xfffffffc)+4;
            nextpc = (pc&0xfffffffc)+((char)(icode1&0x00ff)<<2);
            name = "BL";
            break;

        case 0xfe00: // BL24
            regfile[14] = (pc&0xfffffffc)+4;
            nextpc = (pc&0xfffffffc)+((((char)(icode1&0x00ff)<<16)|(unsigned short)icode2)<<2);
            name = "BL24";
            break;

        case 0x7d00: // BNC
            if( cbit == 0 )
                nextpc = (pc&0xfffffffc)+((char)(icode1&0x00ff)<<2);
            name = "BNC";
            break;

        case 0xfd00: // BNC24
            if( cbit == 0 )
                nextpc = (pc&0xfffffffc)+((((char)(icode1&0x00ff)<<16)|(unsigned short)icode2)<<2);
            name = "BNC24";
            break;

        case 0x7f00: // BRA
            nextpc = (pc&0xfffffffc)+((char)(icode1&0x00ff)<<2);
            name = "BRA";
            break;

        case 0xff00: // BRA24
            if( cbit == 0 )
                nextpc = (pc&0xfffffffc)+((((char)(icode1&0x00ff)<<16)|(unsigned short)icode2)<<2);
            name = "BRA24";
            break;

        default:
            switch( icode1&0xfff0 ) {
            case 0xb080: // BEQZ
                if ( regfile[rs] == 0 )
                    nextpc = (pc&0xfffffffc)+((short)icode2<<2);
                name = "BGQZ";
                break;
            case 0xb0b0: // BGEZ
                if ( regfile[rs] >= 0 )
                    nextpc = (pc&0xfffffffc)+((short)icode2<<2);
                name = "BGEZ";
                break;
            case 0xb0d0: // BGTZ
                if ( regfile[rs] > 0 )
                    nextpc = (pc&0xfffffffc)+((short)icode2<<2);
                name = "BGTZ";
                break;
            case 0xb0c0: // BLEZ
                if ( regfile[rs] <= 0 )
                    nextpc = (pc&0xfffffffc)+((short)icode2<<2);
                name = "BLEZ";
                break;
            case 0xb0a0: // BLTZ
                if ( regfile[rs] < 0 )
                    nextpc = (pc&0xfffffffc)+((short)icode2<<2);
                name = "BLTZ";
                break;
            case 0xb090: // BNEZ
                if ( regfile[rs] != 0 )
                    nextpc = (pc&0xfffffffc)+((short)icode2<<2);
                name = "BNEZ";
                break;
            case 0x8040: // CMPI
                cbit = ((regfile[rs]<(short)icode2) ? 1 : 0);
                name = "CMPI";
                break;
            case 0x8050: // CMPUI
                cbit = (((unsigned)regfile[rs]<(unsigned)(short)icode2) ? 1 : 0);
                name = "CMPUI";
                break;
            case 0x1ec0: // JL
                regfile[14] = (pc&0xfffffffc)+4;
                nextpc = regfile[rs]&0xfffffffc;
                name = "JL";
                break;
            case 0x1fc0: // JMP
                nextpc = regfile[rs]&0xfffffffc;
                name = "JMP";
                break;
            case 0x10f0: // Trap (Simulation command)
                name = "trap";
                switch( rs ) {
                case 0: // Stop simukation
                    goto L_STOP;
                case 1: // Trace on
                    trace = 1;
                    break;
                case 2: // Display R0-R3
                    for( i = 0; i < 4; i++ )
                        printf("R%d = %d ( 0x%08x )\n", i, regfile[i], regfile[i]);
                    break;
                case 3: // Display R0-R15
                    for( i = 0; i < 16; i++ )
                        printf("R%d = %d ( 0x%08x )\n", i, regfile[i], regfile[i]);
                    break;
                case 4: // Display string from R0
                    printf("%08x \"%s\"\n", regfile[0], mem+regfile[0] );
                    break;
                case 5: // Display string from R0, and string from R1
                    printf("%08x \"%s\"\n", regfile[0], mem+regfile[0] );
                    printf("%08x \"%s\"\n", regfile[1], mem+regfile[1] );
                    break;
                case 6: // Display new-line
                    printf("\n");
                    break;
                }
                break;
            default:
                if( (icode1&0xf000) == 0x4000 ) { // ADDI
                    regfile[rd] += (char)(icode1&0x00ff);
                    name = "ADDI";
                } else if( (icode1&0xf000) == 0xe000 ) { // LD24
                    regfile[rd] = ((icode1&0x00ff)<<16)|(unsigned short)icode2;
                    name = "LD24";
                } else if( (icode1&0xf000) == 0x6000 ) { // LDI
                    regfile[rd] = (char)(icode1&0x00ff);
                    name = "LDI";
                } else if( (icode1&0xf0ff) == 0x90f0 ) { // LDI16
                    regfile[rd] = (short)icode2;
                    name = "LDI16";
                } else if( (icode1&0xffff) == 0x7000 ) { // NOP
                    name = "NOP";
                } else if( (icode1&0xf0ff) == 0xd0c0 ) { // SETH
                    regfile[rd] = (short)icode2<<16;
                    name = "SETH";
                } else {
                    goto L_DECERR;
                }
            }
        }
    }
    if( trace ) {
        if( (icode1&0x8000) == 0 )
            printf("PC: %08x %-8s %04x\n", pc, name, icode1);
        else
            printf("PC: %08x %-8s %04x %04x\n", pc, name, icode1, icode2);
    }
    pc = nextpc; // Set nextpc value to the program counter, pc
    return 0;

L_STOP:
    if( trace )
        printf("PC: %08x %s\n", pc, name);
    printf("Stop by trap(0)\n");
    return -1;
L_DECERR:
    if( (icode1&0x8000) == 0 )
        printf("Decode error: PC = %08x, icode1 = %04x\n", pc, icode1);
    else
        printf("Decode error: PC = %08x, icode1 = %04x, icode2 = %04x\n", pc, icode1, icode2);
    return -1;
L_DIV0:
    printf("PC: %08x %s (Divide by 0)\n", pc, name);
    return -1;
}

int main(int ac, char **av)
{
    int i, n;

    if( ac < 2 || 3 < ac ) { // The number of the arguments is 1 or 2
        printf("%s: file [#steps]\n", av[0]);
        return -1;
    }

    if( loaddata(av[1]) < 0 )
        return -1;
    if( ac == 3 ) // The number of simulation steps is specified by the second argument
        n = atoi(av[2]);
    else // The default number of simulation steps is 10
        n = 10;

    for( i = 0; i < n; i++ ) // Repeat simulation n steps
        if( sim_ins() < 0 )
            break;

    printf("Stopped after %d steps\n", i);
    return 0;
}
