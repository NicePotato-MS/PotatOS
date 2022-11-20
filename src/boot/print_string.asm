print_string:			; Output string in SI to screen
	pusha
	mov ah, 0Eh				; int 10h teletype function

.repeat:
	lodsb					; Get char from string
	cmp al, 0
	je .done				; If char is zero, end of string
	int 10h					; Otherwise, print it
	jmp short .repeat

.done:
	popa
	ret