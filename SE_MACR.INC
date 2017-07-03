; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   :	SE_MACR.INC	    ┃
; ┃ 版本号   :	2.0		    ┃
; ┃ 作	 者   :	简晶		    ┃
; ┃ 日	 期   :	1991 年	12 月	    ┃
; ┃				    ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于宏的程序, 有以下几个:
; -----------------------------------
; Push_si\ Push_di\ Pop_si\ Pop_di
; -----------------------------------

PuSh_si		MACRO
		Mov	Bpbuf,Bp
		Mov	Bp,Siseg
		PuSh	Bp		; PuSh siseg
		PuSh	Si		; PuSh si
		Mov	Bp,Bpbuf
		ENDM

Pop_si		MACRO
		Mov	Bpbuf,Bp
		Pop	Si		; Pop si
		Pop	Bp
		Mov	Siseg,Bp	; Pop siseg
		Mov	Bp,Bpbuf
		ENDM

PuSh_di		MACRO
		Mov	Bpbuf,Bp
		Mov	Bp,Diseg
		PuSh	Bp		; PuSh diseg
		PuSh	Di		; PuSh di
		Mov	Bp,Bpbuf
		ENDM

Pop_di		MACRO
		Mov	Bpbuf,Bp
		Pop	Di		; Pop di
		Pop	Bp
		Mov	Diseg,Bp	; Pop diseg
		Mov	Bp,Bpbuf
		ENDM
