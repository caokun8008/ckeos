	.text
	.file	"/usr/local/google/home/dschuff/s/wasm-waterfall/src/work/gcc/gcc/testsuite/gcc.c-torture/execute/pr42248.c"
	.section	.text.check,"ax",@progbits
	.hidden	check
	.globl	check
	.type	check,@function
check:                                  # @check
	.param  	i32, i32
# BB#0:                                 # %entry
	block   	
	f64.load	$push3=, 0($0)
	f64.load	$push2=, 0($1)
	f64.ne  	$push4=, $pop3, $pop2
	br_if   	0, $pop4        # 0: down to label0
# BB#1:                                 # %entry
	f64.load	$push0=, 8($0)
	f64.load	$push1=, 8($1)
	f64.ne  	$push5=, $pop0, $pop1
	br_if   	0, $pop5        # 0: down to label0
# BB#2:                                 # %if.end
	return
.LBB0_3:                                # %if.then
	end_block                       # label0:
	call    	abort@FUNCTION
	unreachable
	.endfunc
.Lfunc_end0:
	.size	check, .Lfunc_end0-check

	.section	.text.init,"ax",@progbits
	.hidden	init
	.globl	init
	.type	init,@function
init:                                   # @init
	.param  	i32, i32
# BB#0:                                 # %entry
	i64.load	$push0=, 0($1)
	i64.store	0($0), $pop0
	i64.load	$push1=, 8($1)
	i64.store	8($0), $pop1
                                        # fallthrough-return
	.endfunc
.Lfunc_end1:
	.size	init, .Lfunc_end1-init

	.section	.text.main,"ax",@progbits
	.hidden	main
	.globl	main
	.type	main,@function
main:                                   # @main
	.result 	i32
# BB#0:                                 # %entry
	i32.const	$push1=, 0
	i64.const	$push0=, 0
	i64.store	g1s+8($pop1), $pop0
	i32.const	$push4=, 0
	i64.const	$push2=, 4607182418800017408
	i64.store	g1s($pop4), $pop2
	i32.const	$push3=, 0
                                        # fallthrough-return: $pop3
	.endfunc
.Lfunc_end2:
	.size	main, .Lfunc_end2-main

	.hidden	g1s                     # @g1s
	.type	g1s,@object
	.section	.bss.g1s,"aw",@nobits
	.globl	g1s
	.p2align	3
g1s:
	.skip	32
	.size	g1s, 32


	.ident	"clang version 4.0.0 (trunk 283460) (llvm/trunk 283507)"
	.functype	abort, void
