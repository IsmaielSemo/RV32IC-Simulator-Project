
t0.elf:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <_start>:
   0:	00014117          	auipc	sp,0x14
   4:	00010113          	mv	sp,sp
   8:	2029                	jal	12 <main>

0000000a <loop>:
   a:	48a9                	li	a7,10
   c:	00000073          	ecall
  10:	bfed                	j	a <loop>

00000012 <main>:
  12:	4085                	li	ra,1
  14:	4109                	li	sp,2
  16:	418d                	li	gp,3
  18:	4211                	li	tp,4
  1a:	4295                	li	t0,5
  1c:	4319                	li	t1,6
  1e:	002083b3          	add	t2,ra,sp
  22:	00038433          	add	s0,t2,zero
  26:	007404b3          	add	s1,s0,t2
  2a:	40648533          	sub	a0,s1,t1
  2e:	931a                	add	t1,t1,t1
  30:	8c65                	and	s0,s0,s1
  32:	8ca9                	xor	s1,s1,a0
  34:	0395                	add	t2,t2,5
  36:	8c89                	sub	s1,s1,a0
  38:	7a000593          	li	a1,1952
  3c:	0596                	sll	a1,a1,0x5
  3e:	7a000613          	li	a2,1952

00000042 <test_branch>:
  42:	0020c363          	blt	ra,sp,48 <L1>
  46:	a031                	j	52 <FAIL>

00000048 <L1>:
  48:	00535363          	bge	t1,t0,4e <L2>
  4c:	a019                	j	52 <FAIL>

0000004e <L2>:
  4e:	00c58463          	beq	a1,a2,56 <PASS>

00000052 <FAIL>:
  52:	5ffd                	li	t6,-1
  54:	a011                	j	58 <finish>

00000056 <PASS>:
  56:	4f81                	li	t6,0

00000058 <finish>:
  58:	48a9                	li	a7,10
  5a:	00000073          	ecall

Disassembly of section .riscv.attributes:

00000000 <.riscv.attributes>:
   0:	1e41                	add	t3,t3,-16
   2:	0000                	unimp
   4:	7200                	.insn	2, 0x7200
   6:	7369                	lui	t1,0xffffa
   8:	01007663          	bgeu	zero,a6,14 <main+0x2>
   c:	0014                	.insn	2, 0x0014
   e:	0000                	unimp
  10:	7205                	lui	tp,0xfffe1
  12:	3376                	.insn	2, 0x3376
  14:	6932                	.insn	2, 0x6932
  16:	7032                	.insn	2, 0x7032
  18:	5f31                	li	t5,-20
  1a:	30703263          	.insn	4, 0x30703263
	...
