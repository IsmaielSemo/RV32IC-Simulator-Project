
t7.elf:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <_start>:
   0:	00014117          	auipc	sp,0x14
   4:	00010113          	mv	sp,sp
   8:	2abd                	jal	186 <main>

0000000a <loop>:
   a:	48a9                	li	a7,10
   c:	00000073          	ecall
  10:	bfed                	j	a <loop>

00000012 <printInt>:
  12:	1101                	add	sp,sp,-32 # 13fe0 <_end+0x3fdc>
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

0000004e <djb33_hash>:
  4e:	7179                	add	sp,sp,-48
  50:	d622                	sw	s0,44(sp)
  52:	1800                	add	s0,sp,48
  54:	fca42e23          	sw	a0,-36(s0)
  58:	fcb42c23          	sw	a1,-40(s0)
  5c:	6785                	lui	a5,0x1
  5e:	50578793          	add	a5,a5,1285 # 1505 <main+0x137f>
  62:	fef42623          	sw	a5,-20(s0)
  66:	a03d                	j	94 <djb33_hash+0x46>
  68:	fec42783          	lw	a5,-20(s0)
  6c:	0796                	sll	a5,a5,0x5
  6e:	fec42703          	lw	a4,-20(s0)
  72:	97ba                	add	a5,a5,a4
  74:	fef42623          	sw	a5,-20(s0)
  78:	fdc42783          	lw	a5,-36(s0)
  7c:	00178713          	add	a4,a5,1
  80:	fce42e23          	sw	a4,-36(s0)
  84:	0007c783          	lbu	a5,0(a5)
  88:	873e                	mv	a4,a5
  8a:	fec42783          	lw	a5,-20(s0)
  8e:	8fb9                	xor	a5,a5,a4
  90:	fef42623          	sw	a5,-20(s0)
  94:	fd842783          	lw	a5,-40(s0)
  98:	fff78713          	add	a4,a5,-1
  9c:	fce42c23          	sw	a4,-40(s0)
  a0:	f7e1                	bnez	a5,68 <djb33_hash+0x1a>
  a2:	fec42783          	lw	a5,-20(s0)
  a6:	853e                	mv	a0,a5
  a8:	5432                	lw	s0,44(sp)
  aa:	6145                	add	sp,sp,48
  ac:	8082                	ret

000000ae <oat_hash>:
  ae:	7179                	add	sp,sp,-48
  b0:	d622                	sw	s0,44(sp)
  b2:	1800                	add	s0,sp,48
  b4:	fca42e23          	sw	a0,-36(s0)
  b8:	fcb42c23          	sw	a1,-40(s0)
  bc:	fdc42783          	lw	a5,-36(s0)
  c0:	fef42623          	sw	a5,-20(s0)
  c4:	fe042423          	sw	zero,-24(s0)
  c8:	a83d                	j	106 <oat_hash+0x58>
  ca:	fec42783          	lw	a5,-20(s0)
  ce:	00178713          	add	a4,a5,1
  d2:	fee42623          	sw	a4,-20(s0)
  d6:	0007c783          	lbu	a5,0(a5)
  da:	873e                	mv	a4,a5
  dc:	fe842783          	lw	a5,-24(s0)
  e0:	97ba                	add	a5,a5,a4
  e2:	fef42423          	sw	a5,-24(s0)
  e6:	fe842783          	lw	a5,-24(s0)
  ea:	07aa                	sll	a5,a5,0xa
  ec:	fe842703          	lw	a4,-24(s0)
  f0:	97ba                	add	a5,a5,a4
  f2:	fef42423          	sw	a5,-24(s0)
  f6:	fe842783          	lw	a5,-24(s0)
  fa:	8399                	srl	a5,a5,0x6
  fc:	fe842703          	lw	a4,-24(s0)
 100:	8fb9                	xor	a5,a5,a4
 102:	fef42423          	sw	a5,-24(s0)
 106:	fd842783          	lw	a5,-40(s0)
 10a:	fff78713          	add	a4,a5,-1
 10e:	fce42c23          	sw	a4,-40(s0)
 112:	ffc5                	bnez	a5,ca <oat_hash+0x1c>
 114:	fe842783          	lw	a5,-24(s0)
 118:	078e                	sll	a5,a5,0x3
 11a:	fe842703          	lw	a4,-24(s0)
 11e:	97ba                	add	a5,a5,a4
 120:	fef42423          	sw	a5,-24(s0)
 124:	fe842783          	lw	a5,-24(s0)
 128:	83ad                	srl	a5,a5,0xb
 12a:	fe842703          	lw	a4,-24(s0)
 12e:	8fb9                	xor	a5,a5,a4
 130:	fef42423          	sw	a5,-24(s0)
 134:	fe842783          	lw	a5,-24(s0)
 138:	07be                	sll	a5,a5,0xf
 13a:	fe842703          	lw	a4,-24(s0)
 13e:	97ba                	add	a5,a5,a4
 140:	fef42423          	sw	a5,-24(s0)
 144:	fe842783          	lw	a5,-24(s0)
 148:	853e                	mv	a0,a5
 14a:	5432                	lw	s0,44(sp)
 14c:	6145                	add	sp,sp,48
 14e:	8082                	ret

00000150 <strlen>:
 150:	7179                	add	sp,sp,-48
 152:	d622                	sw	s0,44(sp)
 154:	1800                	add	s0,sp,48
 156:	fca42e23          	sw	a0,-36(s0)
 15a:	fe042623          	sw	zero,-20(s0)
 15e:	a031                	j	16a <strlen+0x1a>
 160:	fec42783          	lw	a5,-20(s0)
 164:	0785                	add	a5,a5,1
 166:	fef42623          	sw	a5,-20(s0)
 16a:	fec42783          	lw	a5,-20(s0)
 16e:	fdc42703          	lw	a4,-36(s0)
 172:	97ba                	add	a5,a5,a4
 174:	0007c783          	lbu	a5,0(a5)
 178:	f7e5                	bnez	a5,160 <strlen+0x10>
 17a:	fec42783          	lw	a5,-20(s0)
 17e:	853e                	mv	a0,a5
 180:	5432                	lw	s0,44(sp)
 182:	6145                	add	sp,sp,48
 184:	8082                	ret

00000186 <main>:
 186:	1101                	add	sp,sp,-32
 188:	ce06                	sw	ra,28(sp)
 18a:	cc22                	sw	s0,24(sp)
 18c:	ca26                	sw	s1,20(sp)
 18e:	1000                	add	s0,sp,32
 190:	67c1                	lui	a5,0x10
 192:	0007a483          	lw	s1,0(a5) # 10000 <str>
 196:	67c1                	lui	a5,0x10
 198:	0007a783          	lw	a5,0(a5) # 10000 <str>
 19c:	853e                	mv	a0,a5
 19e:	3f4d                	jal	150 <strlen>
 1a0:	87aa                	mv	a5,a0
 1a2:	85be                	mv	a1,a5
 1a4:	8526                	mv	a0,s1
 1a6:	3565                	jal	4e <djb33_hash>
 1a8:	fea42623          	sw	a0,-20(s0)
 1ac:	fec42783          	lw	a5,-20(s0)
 1b0:	853e                	mv	a0,a5
 1b2:	3585                	jal	12 <printInt>
 1b4:	67c1                	lui	a5,0x10
 1b6:	0007a483          	lw	s1,0(a5) # 10000 <str>
 1ba:	67c1                	lui	a5,0x10
 1bc:	0007a783          	lw	a5,0(a5) # 10000 <str>
 1c0:	853e                	mv	a0,a5
 1c2:	3779                	jal	150 <strlen>
 1c4:	87aa                	mv	a5,a0
 1c6:	85be                	mv	a1,a5
 1c8:	8526                	mv	a0,s1
 1ca:	35d5                	jal	ae <oat_hash>
 1cc:	fea42423          	sw	a0,-24(s0)
 1d0:	fe842783          	lw	a5,-24(s0)
 1d4:	853e                	mv	a0,a5
 1d6:	3d35                	jal	12 <printInt>
 1d8:	4785                	li	a5,1
 1da:	853e                	mv	a0,a5
 1dc:	40f2                	lw	ra,28(sp)
 1de:	4462                	lw	s0,24(sp)
 1e0:	44d2                	lw	s1,20(sp)
 1e2:	6105                	add	sp,sp,32
 1e4:	8082                	ret

Disassembly of section .rodata:

000001e8 <.rodata>:
 1e8:	6854                	.insn	2, 0x6854
 1ea:	2065                	jal	292 <main+0x10c>
 1ec:	7571                	lui	a0,0xffffc
 1ee:	6369                	lui	t1,0x1a
 1f0:	7262206b          	.insn	4, 0x7262206b
 1f4:	206e776f          	jal	a4,e73fa <_stack_start+0xd33fa>
 1f8:	6f66                	.insn	2, 0x6f66
 1fa:	2078                	.insn	2, 0x2078
 1fc:	756a                	.insn	2, 0x756a
 1fe:	706d                	c.lui	zero,0xffffb
 200:	766f2073          	.insn	4, 0x766f2073
 204:	7265                	lui	tp,0xffff9
 206:	7420                	.insn	2, 0x7420
 208:	6568                	.insn	2, 0x6568
 20a:	6c20                	.insn	2, 0x6c20
 20c:	7a61                	lui	s4,0xffff8
 20e:	2079                	jal	29c <main+0x116>
 210:	6f64                	.insn	2, 0x6f64
 212:	Address 0x212 is out of bounds.


Disassembly of section .sdata:

00010000 <str>:
   10000:	01e8                	add	a0,sp,204
	...

Disassembly of section .riscv.attributes:

00000000 <.riscv.attributes>:
   0:	2041                	jal	80 <djb33_hash+0x32>
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
   e:	2055                	jal	b2 <oat_hash+0x4>
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
