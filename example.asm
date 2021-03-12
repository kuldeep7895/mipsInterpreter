addi	$t1,$zero,0
addi	$t1,$t1,9

sw	$t1,56($zero)

lw	$t2,56($zero)

beq	$t1,$t2,20

addi	$t1,$t2,96

addi	$t1,$t2,-96

addi	$sp	,$sp,-4

sw	$t1,($sp)

lw	$t4, ($sp)
addi	$sp,$sp,4
