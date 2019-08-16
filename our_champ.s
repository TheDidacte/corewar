.name "Champignon"
.comment "Wow, it's such good defense"

start:
	ld		%42991616, r16			# ld %0, r1 for defense wall
	ld		%17432076, r15			# zjmp %-500 for defense wall
	ld		%6, r2
	ld		%0,	r2
	zjmp	%:start2

defense:
	st		r1, 6
	live	%155134
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r15, -500
	st		r16, -500
	st		r16, -500
	st		r15, -500
	ld		%0, r2
	zjmp	%:defense

start2:
	fork	%:defense
	st		r1, 6
	live	%155134
	fork	%:preattack2
	st		r1, 6
	live	%155134
	fork	%:attack
	st		r1, 6
	live	%155134
	ld		%0, r3
	zjmp	%:start2



attack:
	ld %57675521, r15		#	st r15, 256	

	ld %4094231023, r14		#	zjmp %495
	ld %57675521, r13		#	st r15, 500	

	ld %3925868544, r12		#	zjmp %490
	ld %32704769, r11		#	st r14, 499	
	ld %4093865998, r10		#	st r13, 500	
	ld %57675009, r9		#	


	ld %151116544, r8		#	zjmp %475	
	ld %16777216, r7		#	live %0	
	ld %1879835121, r6		#	st r12, 497
	ld %184676867, r5		#	st r11, 498	
	ld %32703344, r4		#	
	ld %4093865994, r3		#	st r10, 499
	ld %57673985, r2		#	st r9, 500

	ld %0, r16

	st r2, 500
	st r3, 499
	st r4, 498
	st r5, 497
	st r6, 496
	st r7, 495
	st r1, 491
	st r8, 490
	st r1, 6
	live %123

	zjmp %450

# 190317573 equals 	sti	r4, r5
# so we can write forward

preattack2:
	ld		%190317573, r4
	ld		%290, r5					# in r5 is our "jump" size
	ld		%500, r7
	ld		%6, r6
	st		r1, 6						#write our player number behind live

attack2:								#loop for (r7 - r5) / r6 times
	live	%155134
	add		r5, r6, r5
	sti		r4, r5, %0
	xor		r7, r5, r8
	zjmp 	%300
	ld		%0, r10	
	zjmp 	%:attack2

