[global readcr0]
readcr0:
	mov eax, cr0
	retn
	
[global writecr0]
writecr0:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr0, eax
	pop ebp
	retn
	
[global readcr3]
readcr3:
	mov eax, cr3
	retn
	
[global writecr3]
writecr3:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax
	pop ebp
	retn
