
t2.elf:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <_start>:
   0:	00014117          	auipc	sp,0x14
   4:	00010113          	mv	sp,sp
   8:	2099                	jal	4e <main>

0000000a <loop>:
   a:	48a9                	li	a7,10
   c:	00000073          	ecall
  10:	bfed                	j	a <loop>

00000012 <printInt>:
  12:	1101                	add	sp,sp,-32 # 13fe0 <_end+0x13f58>
  14:	ce22                	sw	s0,28(sp)
  16:	1000                	add	s0,sp,32
  18:	fea42623          	sw	a0,-20(s0)
  1c:	4885                	li	a7,1
  1e:	853e                	mv	a0,a5
  20:	fef42623          	sw	a5,-20(s0)
  24:	00000073          	ecall
  28:	0001                	nop
  2a:	4472                	lw	s0,28(sp)
  2c:	6105                	add	sp,sp,32
  2e:	8082                	ret

00000030 <printString>:
  30:	1101                	add	sp,sp,-32
  32:	ce22                	sw	s0,28(sp)
  34:	1000                	add	s0,sp,32
  36:	fea42623          	sw	a0,-20(s0)
  3a:	4891                	li	a7,4
  3c:	853e                	mv	a0,a5
  3e:	fef42623          	sw	a5,-20(s0)
  42:	00000073          	ecall
  46:	0001                	nop
  48:	4472                	lw	s0,28(sp)
  4a:	6105                	add	sp,sp,32
  4c:	8082                	ret

0000004e <main>:
  4e:	1101                	add	sp,sp,-32
  50:	ce06                	sw	ra,28(sp)
  52:	cc22                	sw	s0,24(sp)
  54:	1000                	add	s0,sp,32
  56:	47a9                	li	a5,10
  58:	fef42623          	sw	a5,-20(s0)
  5c:	47d1                	li	a5,20
  5e:	fef42423          	sw	a5,-24(s0)
  62:	fe842783          	lw	a5,-24(s0)
  66:	00179713          	sll	a4,a5,0x1
  6a:	fec42783          	lw	a5,-20(s0)
  6e:	97ba                	add	a5,a5,a4
  70:	17cd                	add	a5,a5,-13
  72:	fef42623          	sw	a5,-20(s0)
  76:	fec42503          	lw	a0,-20(s0)
  7a:	3f61                	jal	12 <printInt>
  7c:	4785                	li	a5,1
  7e:	853e                	mv	a0,a5
  80:	40f2                	lw	ra,28(sp)
  82:	4462                	lw	s0,24(sp)
  84:	6105                	add	sp,sp,32
  86:	8082                	ret

Disassembly of section .riscv.attributes:

00000000 <.riscv.attributes>:
   0:	2041                	jal	80 <main+0x32>
   2:	0000                	unimp
   4:	7200                	.insn	2, 0x7200
   6:	7369                	lui	t1,0xffffa
   8:	01007663          	bgeu	zero,a6,14 <printInt+0x2>
   c:	0016                	c.slli	zero,0x5
   e:	0000                	unimp
  10:	1004                	add	s1,sp,32
  12:	7205                	lui	tp,0xfffe1
  14:	3376                	.insn	2, 0x3376
  16:	6932                	.insn	2, 0x6932
  18:	7032                	.insn	2, 0x7032
  1a:	5f31                	li	t5,-20
  1c:	30703263          	.insn	4, 0x30703263
	...

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347          	.insn	4, 0x3a434347
   4:	2820                	.insn	2, 0x2820
   6:	5078                	lw	a4,100(s0)
   8:	6361                	lui	t1,0x18
   a:	4e47206b          	.insn	4, 0x4e47206b
   e:	2055                	jal	b2 <_end+0x2a>
  10:	4952                	lw	s2,20(sp)
  12:	562d4353          	.insn	4, 0x562d4353
  16:	4520                	lw	s0,72(a0)
  18:	626d                	lui	tp,0x1b
  1a:	6465                	lui	s0,0x19
  1c:	6564                	.insn	2, 0x6564
  1e:	2064                	.insn	2, 0x2064
  20:	20434347          	.insn	4, 0x20434347
  24:	7261                	lui	tp,0xffff8
  26:	366d                	jal	fffffbd0 <_stack_start+0xfffebbd0>
  28:	2934                	.insn	2, 0x2934
  2a:	3120                	.insn	2, 0x3120
  2c:	2e322e33          	.insn	4, 0x2e322e33
  30:	0030                	add	a2,sp,8
