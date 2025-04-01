# **C**isc **AS**se**M**bler (CASM)

`CASM` is an assembler that works on a custom CISC ISA. This assembler is part of a CPU simulator of the mentioned ISA, where it should be implemented in C#. This C implementation is made just for the sake of practicing C.

## Some details of the ISA

There are 4 types of instructions:

### B1:
| Oppcode    | Source addressing mode | Source register | Destination addressing mode | Destination register |
| ---------- | ---------------------- |---------------- |---------------------------- | -------------------- |
| 4b         | 2b                     | 4b              | 2b                          | 4b                   |

Instructions: MOV, ADD, SUB, CMP, AND, OR, XOR

### B2:
| Oppcode    | Destination addressing mode | Destination register |
| ---------- | --------------------------- | -------------------- |
| 10b        | 2b                          | 4b                   |

Instructions: CLR, NEG, INC, DEC, ASL, ASR, LSR, ROL, ROR, RLC, RRC, JMP, CALL, PUSH, POP

### B3:
| Oppcode    | Offset |
| ---------- | ------ |
| 8b         | 8b     |

Instructions: BR, BNE, BEQ, BPL, BMI, BCS, BCC, BVS, BVC

### B4:
| Oppcode    |
| ---------- |
| 16b        |

Instructions: CLC, CLV, CLZ, CLS, CCC, SEC, SEV, SEZ, SES, SCC, NOP, RET, RETI, HALT, WAIT, PUSHPC, POPPC, PUSHF, POPF

## Addressing modes

There are 4 types:

| Mode       | Index  |
| ---------- | ------ |
| Immediate  | 00     |
| Direct     | 01     |
| Indirect   | 10     |
| Indexed    | 11     |

## Registers

| Name       | Index  |
| ---------- | ------ |
| R0         | 0000   |
| R1         | 0001   |
| R2         | 0010   |
| R3         | 0011   |
| R4         | 0100   |
| R5         | 0101   |
| R6         | 0110   |
| R7         | 0111   |
| R8         | 1000   |
| R9         | 1001   |
| R10        | 1010   |
| R11        | 1011   |
| R12        | 1100   |
| R13        | 1101   |
| R14        | 1110   |
| R15        | 1111   |

## Example assembly program

**main.asm**
```asm
proc start
    mov r0, 5
    call factorial
    nop
    nop
    nop

    mov r1, 100
    mov 21[r2], 12[r1]
    mov 123[r3], r2

    halt
endp start

; Input: R0
; Output: R2
; Modifies: R0, R1, R2, R3
; Description: This function calculates the sum of the first R1 numbers
proc factorial
    mov r1, 1
    mov r2, 1
endp factorial
factorialLoop:
    mov r3,r1
shiftLoop:
    asl r2
    dec r3
    cmp r3, 0
    bne shiftLoop

    dec r0
    inc r1
    cmp r0, 0
    bne factorialLoop

    ret

```
## Generating the object code

Displays how many bytes the program has:
```
casm main.asm
```

Used for debugging the object code:
```
casm main.asm -v -S
```
![image](https://github.com/user-attachments/assets/aa103012-12d5-48a4-9fb2-08c97ac0eaa7)


