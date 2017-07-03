; ������������������������������������
; �� �ļ���   :	SE_MACR.INC	    ��
; �� �汾��   :	2.0		    ��
; �� ��	 ��   :	��		    ��
; �� ��	 ��   :	1991 ��	12 ��	    ��
; ��				    ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved in ShenZhen  ��
; ������������������������������������
;
; ���ļ��������ں�ĳ���, �����¼���:
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