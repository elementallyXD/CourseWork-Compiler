;Bidyak Mikahilo KV-61 V-1
DATA SEGMENT
      Idb1   DB		1111011B
      Ids2	 DB		"Test"
      Idw3   DW		2108D
      Idd4   DD		0b121H
	  Val1   DW     1991
	  Val2   DD     label2
DATA ENDS
ASSUME CS:CODE,DS:DATA
CODE SEGMENT
	label1:
		CLI
		INC BL
		INC EAX
		DEC Idw3[BX]
		AND GS:Ids2[EDI], AL
		AND Ids2[ESI], BL
		CMP BX, Idw3[BP]
		ADD EAX, ECX
		JB label2
		MOV AX, 155H
		MOV BL, 9
		XOR DS:Val1[ESI], AX
		OR Val1[SI], 22H
		JMP label1
		JMP Val2
   label2:
CODE ENDS
END
