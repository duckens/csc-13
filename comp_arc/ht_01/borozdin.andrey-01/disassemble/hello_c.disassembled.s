
bin/hello_c:     file format elf32-i386


Disassembly of section .init:

080482b0 <_init>:
 80482b0:	53                   	push   %ebx
 80482b1:	83 ec 08             	sub    $0x8,%esp
 80482b4:	e8 00 00 00 00       	call   80482b9 <_init+0x9>
 80482b9:	5b                   	pop    %ebx
 80482ba:	81 c3 47 1d 00 00    	add    $0x1d47,%ebx
 80482c0:	8b 83 fc ff ff ff    	mov    -0x4(%ebx),%eax
 80482c6:	85 c0                	test   %eax,%eax
 80482c8:	74 05                	je     80482cf <_init+0x1f>
 80482ca:	e8 31 00 00 00       	call   8048300 <__gmon_start__@plt>
 80482cf:	83 c4 08             	add    $0x8,%esp
 80482d2:	5b                   	pop    %ebx
 80482d3:	c3                   	ret    

Disassembly of section .plt:

080482e0 <puts@plt-0x10>:
 80482e0:	ff 35 04 a0 04 08    	pushl  0x804a004
 80482e6:	ff 25 08 a0 04 08    	jmp    *0x804a008
 80482ec:	00 00                	add    %al,(%eax)
	...

080482f0 <puts@plt>:
 80482f0:	ff 25 0c a0 04 08    	jmp    *0x804a00c
 80482f6:	68 00 00 00 00       	push   $0x0
 80482fb:	e9 e0 ff ff ff       	jmp    80482e0 <_init+0x30>

08048300 <__gmon_start__@plt>:
 8048300:	ff 25 10 a0 04 08    	jmp    *0x804a010
 8048306:	68 08 00 00 00       	push   $0x8
 804830b:	e9 d0 ff ff ff       	jmp    80482e0 <_init+0x30>

08048310 <__libc_start_main@plt>:
 8048310:	ff 25 14 a0 04 08    	jmp    *0x804a014
 8048316:	68 10 00 00 00       	push   $0x10
 804831b:	e9 c0 ff ff ff       	jmp    80482e0 <_init+0x30>

Disassembly of section .text:

08048320 <_start>:
 8048320:	31 ed                	xor    %ebp,%ebp
 8048322:	5e                   	pop    %esi
 8048323:	89 e1                	mov    %esp,%ecx
 8048325:	83 e4 f0             	and    $0xfffffff0,%esp
 8048328:	50                   	push   %eax
 8048329:	54                   	push   %esp
 804832a:	52                   	push   %edx
 804832b:	68 a0 84 04 08       	push   $0x80484a0
 8048330:	68 30 84 04 08       	push   $0x8048430
 8048335:	51                   	push   %ecx
 8048336:	56                   	push   %esi
 8048337:	68 0c 84 04 08       	push   $0x804840c
 804833c:	e8 cf ff ff ff       	call   8048310 <__libc_start_main@plt>
 8048341:	f4                   	hlt    
 8048342:	66 90                	xchg   %ax,%ax
 8048344:	66 90                	xchg   %ax,%ax
 8048346:	66 90                	xchg   %ax,%ax
 8048348:	66 90                	xchg   %ax,%ax
 804834a:	66 90                	xchg   %ax,%ax
 804834c:	66 90                	xchg   %ax,%ax
 804834e:	66 90                	xchg   %ax,%ax

08048350 <deregister_tm_clones>:
 8048350:	b8 23 a0 04 08       	mov    $0x804a023,%eax
 8048355:	2d 20 a0 04 08       	sub    $0x804a020,%eax
 804835a:	83 f8 06             	cmp    $0x6,%eax
 804835d:	77 02                	ja     8048361 <deregister_tm_clones+0x11>
 804835f:	f3 c3                	repz ret 
 8048361:	b8 00 00 00 00       	mov    $0x0,%eax
 8048366:	85 c0                	test   %eax,%eax
 8048368:	74 f5                	je     804835f <deregister_tm_clones+0xf>
 804836a:	55                   	push   %ebp
 804836b:	89 e5                	mov    %esp,%ebp
 804836d:	83 ec 18             	sub    $0x18,%esp
 8048370:	c7 04 24 20 a0 04 08 	movl   $0x804a020,(%esp)
 8048377:	ff d0                	call   *%eax
 8048379:	c9                   	leave  
 804837a:	c3                   	ret    
 804837b:	90                   	nop
 804837c:	8d 74 26 00          	lea    0x0(%esi,%eiz,1),%esi

08048380 <register_tm_clones>:
 8048380:	b8 20 a0 04 08       	mov    $0x804a020,%eax
 8048385:	2d 20 a0 04 08       	sub    $0x804a020,%eax
 804838a:	c1 f8 02             	sar    $0x2,%eax
 804838d:	89 c2                	mov    %eax,%edx
 804838f:	c1 ea 1f             	shr    $0x1f,%edx
 8048392:	01 d0                	add    %edx,%eax
 8048394:	d1 f8                	sar    %eax
 8048396:	75 02                	jne    804839a <register_tm_clones+0x1a>
 8048398:	f3 c3                	repz ret 
 804839a:	ba 00 00 00 00       	mov    $0x0,%edx
 804839f:	85 d2                	test   %edx,%edx
 80483a1:	74 f5                	je     8048398 <register_tm_clones+0x18>
 80483a3:	55                   	push   %ebp
 80483a4:	89 e5                	mov    %esp,%ebp
 80483a6:	83 ec 18             	sub    $0x18,%esp
 80483a9:	89 44 24 04          	mov    %eax,0x4(%esp)
 80483ad:	c7 04 24 20 a0 04 08 	movl   $0x804a020,(%esp)
 80483b4:	ff d2                	call   *%edx
 80483b6:	c9                   	leave  
 80483b7:	c3                   	ret    
 80483b8:	90                   	nop
 80483b9:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi

080483c0 <__do_global_dtors_aux>:
 80483c0:	80 3d 20 a0 04 08 00 	cmpb   $0x0,0x804a020
 80483c7:	75 13                	jne    80483dc <__do_global_dtors_aux+0x1c>
 80483c9:	55                   	push   %ebp
 80483ca:	89 e5                	mov    %esp,%ebp
 80483cc:	83 ec 08             	sub    $0x8,%esp
 80483cf:	e8 7c ff ff ff       	call   8048350 <deregister_tm_clones>
 80483d4:	c6 05 20 a0 04 08 01 	movb   $0x1,0x804a020
 80483db:	c9                   	leave  
 80483dc:	f3 c3                	repz ret 
 80483de:	66 90                	xchg   %ax,%ax

080483e0 <frame_dummy>:
 80483e0:	a1 10 9f 04 08       	mov    0x8049f10,%eax
 80483e5:	85 c0                	test   %eax,%eax
 80483e7:	74 1e                	je     8048407 <frame_dummy+0x27>
 80483e9:	b8 00 00 00 00       	mov    $0x0,%eax
 80483ee:	85 c0                	test   %eax,%eax
 80483f0:	74 15                	je     8048407 <frame_dummy+0x27>
 80483f2:	55                   	push   %ebp
 80483f3:	89 e5                	mov    %esp,%ebp
 80483f5:	83 ec 18             	sub    $0x18,%esp
 80483f8:	c7 04 24 10 9f 04 08 	movl   $0x8049f10,(%esp)
 80483ff:	ff d0                	call   *%eax
 8048401:	c9                   	leave  
 8048402:	e9 79 ff ff ff       	jmp    8048380 <register_tm_clones>
 8048407:	e9 74 ff ff ff       	jmp    8048380 <register_tm_clones>

0804840c <main>:
 804840c:	55                   	push   %ebp
 804840d:	89 e5                	mov    %esp,%ebp
 804840f:	83 e4 f0             	and    $0xfffffff0,%esp
 8048412:	83 ec 10             	sub    $0x10,%esp
 8048415:	c7 04 24 c8 84 04 08 	movl   $0x80484c8,(%esp)
 804841c:	e8 cf fe ff ff       	call   80482f0 <puts@plt>
 8048421:	b8 00 00 00 00       	mov    $0x0,%eax
 8048426:	c9                   	leave  
 8048427:	c3                   	ret    
 8048428:	66 90                	xchg   %ax,%ax
 804842a:	66 90                	xchg   %ax,%ax
 804842c:	66 90                	xchg   %ax,%ax
 804842e:	66 90                	xchg   %ax,%ax

08048430 <__libc_csu_init>:
 8048430:	55                   	push   %ebp
 8048431:	57                   	push   %edi
 8048432:	56                   	push   %esi
 8048433:	53                   	push   %ebx
 8048434:	e8 69 00 00 00       	call   80484a2 <__i686.get_pc_thunk.bx>
 8048439:	81 c3 c7 1b 00 00    	add    $0x1bc7,%ebx
 804843f:	83 ec 1c             	sub    $0x1c,%esp
 8048442:	8b 6c 24 30          	mov    0x30(%esp),%ebp
 8048446:	8d bb 0c ff ff ff    	lea    -0xf4(%ebx),%edi
 804844c:	e8 5f fe ff ff       	call   80482b0 <_init>
 8048451:	8d 83 08 ff ff ff    	lea    -0xf8(%ebx),%eax
 8048457:	29 c7                	sub    %eax,%edi
 8048459:	c1 ff 02             	sar    $0x2,%edi
 804845c:	85 ff                	test   %edi,%edi
 804845e:	74 29                	je     8048489 <__libc_csu_init+0x59>
 8048460:	31 f6                	xor    %esi,%esi
 8048462:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 8048468:	8b 44 24 38          	mov    0x38(%esp),%eax
 804846c:	89 2c 24             	mov    %ebp,(%esp)
 804846f:	89 44 24 08          	mov    %eax,0x8(%esp)
 8048473:	8b 44 24 34          	mov    0x34(%esp),%eax
 8048477:	89 44 24 04          	mov    %eax,0x4(%esp)
 804847b:	ff 94 b3 08 ff ff ff 	call   *-0xf8(%ebx,%esi,4)
 8048482:	83 c6 01             	add    $0x1,%esi
 8048485:	39 fe                	cmp    %edi,%esi
 8048487:	75 df                	jne    8048468 <__libc_csu_init+0x38>
 8048489:	83 c4 1c             	add    $0x1c,%esp
 804848c:	5b                   	pop    %ebx
 804848d:	5e                   	pop    %esi
 804848e:	5f                   	pop    %edi
 804848f:	5d                   	pop    %ebp
 8048490:	c3                   	ret    
 8048491:	eb 0d                	jmp    80484a0 <__libc_csu_fini>
 8048493:	90                   	nop
 8048494:	90                   	nop
 8048495:	90                   	nop
 8048496:	90                   	nop
 8048497:	90                   	nop
 8048498:	90                   	nop
 8048499:	90                   	nop
 804849a:	90                   	nop
 804849b:	90                   	nop
 804849c:	90                   	nop
 804849d:	90                   	nop
 804849e:	90                   	nop
 804849f:	90                   	nop

080484a0 <__libc_csu_fini>:
 80484a0:	f3 c3                	repz ret 

080484a2 <__i686.get_pc_thunk.bx>:
 80484a2:	8b 1c 24             	mov    (%esp),%ebx
 80484a5:	c3                   	ret    
 80484a6:	66 90                	xchg   %ax,%ax

Disassembly of section .fini:

080484a8 <_fini>:
 80484a8:	53                   	push   %ebx
 80484a9:	83 ec 08             	sub    $0x8,%esp
 80484ac:	e8 00 00 00 00       	call   80484b1 <_fini+0x9>
 80484b1:	5b                   	pop    %ebx
 80484b2:	81 c3 4f 1b 00 00    	add    $0x1b4f,%ebx
 80484b8:	83 c4 08             	add    $0x8,%esp
 80484bb:	5b                   	pop    %ebx
 80484bc:	c3                   	ret    
