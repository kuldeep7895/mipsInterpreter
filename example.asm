addi	$t0,$zero,1
addi	$t1,$zero,10
addi	$t3,$zero,1

beq	$t1,$zero,11

mul	$t3,$t3,$t0
beq	$t0,$t1,11
addi	$t0,$t0,1
j	6

