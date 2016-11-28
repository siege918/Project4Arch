	.data
kx: .float 1.65
X2: .float -0.57
X1: .float 0.0
ky: .float 0.80
Y2: .float -0.92
Y1: .float 0.0
X0: .float 0.0
Y0: .float 0.0
bl: .float 0.0

	.text
	li $0, 100 #Loop
	la $10, X1
	l.d $26, ($10)
	la $11, Y1
    l.d $27, ($11)
	la $12, X2
	l.d $28, ($12)
	la $13, Y2
	l.d $29, ($13)
	la $14, kx
	l.d $30, ($14)
	la $15, ky
    l.d $31, ($15)
	la $16, bl
	l.d $5, ($16)
	la $17, X0
	la $3, ($17)
	la $18, Y0
	la $4, ($18)
loopStart:
    subi $0, $0, 1
	
	fmul $1, $30, $26
	fsub $1, $1, $28
	fmul $2, $31, $27
	fsub $2, $2, $29
	
	fadd $28, $5, $26
	fadd $26, $5, $1
	fadd $29, $5, $27
	fadd $27, $5, $2
	
	s.d $1, ($3)
	s.d $2, ($4)
	
	beqz $0, end
	b loopStart
end:
	li $v0, 10
	syscall