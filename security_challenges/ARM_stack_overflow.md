This security challenge was to inspect an ELF binary that was compiled for an ARM processor. There was no access
to the original source code and address space layout randomisation (ASLR) was used. In addition, full RELRO
was in force for the binary (read-only relocations), which means entries in the global offset table and
procedure linkage table cannot be overwritten.

When the binary is run, the user is prompted for some data that the program will then dump to standard output.
When dumping the data to stdout, the base address of the stack buffer was included at the start of the line,
which therefore provided the means of overcoming ASLR in this challenge.

The calling convention in ARM is to pass arguments to functions in registers (and not on the stack, as with
the cdecl Intel calling convention). Arguments are passed in registers r1, r2, r3... The return value of a
function is stored in r0.

**Overview of code mechanics of the vulnerable program**
</br>
</br>
1. Set up the stack frame, saving certain registers' values and then decrementing 140 from stack pointer;
2. Call setvbuf() to change the standard output (stdout) stream to unbuffered [ `setvbuf(stdout, NULL, _IONBF, 0);` ];
3. Store the character 'y' at location (sp + 7);
4. Call (f)printf() to prompt the user for input;
5. scanf() is called, with the address of some format string, and the destination for writing input data at (sp + 8);
    - This means the buffer on the stack for storing the user's input data is 132 bytes in length
6. Print this data to stdout, one character at a time;
7. Prompt the user and ask if they would like to input more data, reading a character from stdin;
8. Go to 4 if the user wishes to input more data;
9. Otherwise, unwind the stack frame and return from main()
</br>
</br>
</br>

**Why is the program vulnerable to exploitation?**
</br>
</br>
scanf() is a vulnerable function, as it will continue reading data from stdin stream until a newline is reached.
This means a malicious user can easily overflow the buffer. When the stack is unwound at the end of the function,
values crafted by the malicious user will be popped back into the registers stored during the prologue, the
most important one being the value for lr (link register) that will be popped into pc (program counter). This
points to memory where the next instruction should be executed.

An address where shellcode is stored can be popped into the pc register and thus the malicious user will spawn
a shell. If the target program in question has the setuid bit set to 1, the malicious user will have a shell with
the same privileges as the vulnerable program ("privilege escalation").

```
000103d8 <main>:
==================================== Function Prologue =======================================
   103d8:       e3012008        movw    r2, #4104                    ; 0x1008
   103dc:       e3402002        movt    r2, #2                       ; 0x21008
   103e0:       e92d4ff0        push    {r4, r5, r6, r7, r8, r9, sl, fp, lr}
   103e4:       e3a03000        mov     r3, #0
   103e8:       e24dd08c        sub     sp, sp, #140                 ; 0x8c
   103ec:       e300b62c        movw    fp, #1580                    ; 0x62c
   103f0:       e300a644        movw    sl, #1604                    ; 0x644
   103f4:       e340b001        movt    fp, #1
   103f8:       e340a001        movt    sl, #1
   103fc:       e5920000        ldr     r0, [r2]
   10400:       e1a01003        mov     r1, r3
   10404:       e3a02002        mov     r2, #2
   
   10408:       ebffffe9        bl      103b4 <setvbuf@plt>         ; set stdout to unbuffered
   1040c:       e3a03079        mov     r3, #121                    ; 0x79 ('y')
   10410:       e5cd3007        strb    r3, [sp, #7]                ; *((char *)sp) + 7) = 'y'
   10414:       e1a0100b        mov     r1, fp                      ; address of format string
   10418:       e3a00001        mov     r0, #1                      ; stdout
   1041c:       ebffffe7        bl      103c0 <__printf_chk@plt>    ; prompt user for data
   10420:       e28d1008        add     r1, sp, #8                  ; r1 = base address of the stack buffer
   10424:       e1a0000a        mov     r0, sl                      ; address of format string
   10428:       ebffffd8        bl      10390 <scanf@plt>           ; Here is our vulnerable function call
   1042c:       e3500000        cmp     r0, #0
   10430:       0a000021        beq     104bc <main+0xe4>

   10434:       e3008664        movw    r8, #1636       ; 0x664
   10438:       e300766c        movw    r7, #1644       ; 0x66c
   1043c:       e3009674        movw    r9, #1652       ; 0x674
   10440:       e3408001        movt    r8, #1		    ; 0x11636
   10444:       e3407001        movt    r7, #1		    ; 0x11644
   10448:       e28d5008        add     r5, sp, #8	    ; r5 = base address of stack buffer
   1044c:       e3409001        movt    r9, #1		    ; 0x11652
   10450:       e3a04000        mov     r4, #0
   10454:       ea000005        b       10470 <main+0x98>
   
   ==================================== Start of loop ====================================
   10458:       e4d52001        ldrb    r2, [r5], #1              ; r2 = *r5++ (dereference r5 and get character; then increment address)
   1045c:       e1a01007        mov     r1, r7                    ; address of format string (probably "%c ")
   10460:       e3a00001        mov     r0, #1                    ; stdout
   10464:       ebffffd5        bl      103c0 <__printf_chk@plt>
   10468:       e356000f        cmp     r6, #15                   ; does r6 == 15?
   1046c:       0a00000b        beq     104a0 <main+0xc8>
   10470:       e28d0008        add     r0, sp, #8            ; r0 = base address of stack buffer
   10474:       ebffffcb        bl      103a8 <strlen@plt>
   10478:       e1500004        cmp     r0, r4                ; is the length 0?
   1047c:       9a00000e        bls     104bc <main+0xe4>

   10480:       e214600f        ands    r6, r4, #15             ; r6 = r4 & 0x000000ff
   10484:       e2844001        add     r4, r4, #1              ; ++r4
   10488:       1afffff2        bne     10458 <main+0x80>       ; if result is non-zero of the ands, go to start of loop
   1048c:       e1a02005        mov     r2, r5
   10490:       e1a01008        mov     r1, r8
   10494:       e3a00001        mov     r0, #1                  ; stdout
   10498:       ebffffc8        bl      103c0 <__printf_chk@plt>
   1049c:       eaffffed        b       10458 <main+0x80>

   104a0:       e1a01009        mov     r1, r9
   104a4:       e3a00001        mov     r0, #1                  ; stdout

   104a8:       ebffffc4        bl      103c0 <__printf_chk@plt>    ; prompt user
   104ac:       e28d0008        add     r0, sp, #8                  ; r0 = base address of stack buffer
   104b0:       ebffffbc        bl      103a8 <strlen@plt>
   104b4:       e1500004        cmp     r0, r4                      ; compare result with value in r4
   104b8:       8afffff0        bhi     10480 <main+0xa8>           ; if (unsigned int)r4 > (unsigned int)r0

   104bc:       e300164c        movw    r1, #1612                   ; 0x64c
   104c0:       e3a00001        mov     r0, #1                      ; stdout
   104c4:       e3401001        movt    r1, #1                      ; 0x1064c
   104c8:       ebffffbc        bl      103c0 <__printf_chk@plt>    ; prompt user for character
   104cc:       e28d1007        add     r1, sp, #7                  ; address of the 'y' character
   104d0:       e3000660        movw    r0, #1632                   ; 0x660
   104d4:       e3400001        movt    r0, #1                      ; 0x10660

   104d8:       ebffffac        bl      10390 <scanf@plt>           ; get character from user (store at sp + 7)
   104dc:       e3500000        cmp     r0, #0
   104e0:       0a000003        beq     104f4 <main+0x11c>
   104e4:       e5dd3007        ldrb    r3, [sp, #7]                ; get the character input by the user
   104e8:       e20330df        and     r3, r3, #223                ; c &= 0xdf ( c = c & 1101 1111 ) - set 5th bit to 0, or c -= 32
   104ec:       e3530059        cmp     r3, #89                     ; compare r3 with 0x59 ('Y') 
   104f0:       0affffc7        beq     10414 <main+0x3c>

==================================== Function epilogue ====================================
   104f4:       e3a00000        mov     r0, #0          ; return value of main
   104f8:       e28dd08c        add     sp, sp, #140    ; sp will be pointing at where r4 is stored on the stack
   104fc:       e8bd8ff0        pop     {r4, r5, r6, r7, r8, r9, sl, fp, pc}
   ```

In order to exploit this vulnerability, pwntools was used, which is a powerful python module with many capabilities.
For example, shellcode for a target architecture can be produced easily, padding can be added (e.g. a NOP slide),
and it has its builtin struct packing, which will pack the data in the right way given the context data it has on
the target binary (little / big endian, etc).

Given that the binary leaked the base address of the stack buffer when dumping the user's input data, this challenge
is very basic and simple to complete. Using pwntools, the a process object is created with the target binary running,
output from the binary is read in, the base address of the stack parsed, and the payload (complete with the correct
stack address) is sent as input. The buffer is overflowed and the offset on the stack where the saved value for the
link register (later popped into the program counter) is overwritten with an address that points to our NOP slide in
the payload. The main() function epilogue is executed and the stack address is placed into the pc, and thus the
shellcode is executed, spawning a shell with the higher privileges required to read the flag from the password file.
