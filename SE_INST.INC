; ������������������������������������
; �� �ļ���   :	SE_INST.INC	    ��
; �� �汾��   :	2.0		    ��
; �� ��	 ��   :	��		    ��
; �� ��	 ��   :	1991 ��	12 ��	    ��
; ��				    ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved in ShenZhen  ��
; ������������������������������������
;
; ���ļ��������ڲ���������ӳ���, �����¼���:
; -----------------------------------------------------
; Insert       -  �� Inschr �е��ַ����뵱ǰ���λ��
; NewLine      -  �µ�һ��
; InsLine      -  ����һ����
; InsLastLine  -  ���ļ�ĩ�в���һ�س�,	F3 ���̹��ܵ���
; InsEnter     -  ���س����Ĵ���
; -----------------------------------------------------

; ����һ�ַ�, Ԥ�ȷ��� Inschr ��
INSERT:
		Call	Calps1
INSERT_REP:	Mov	Es,Siseg
		Mov	Chang,1
		Mov	Al,Es:[Si]
		Cmp	Al,0dh
		Jz	N_inc_Phc
		Cmp	Al,1ah
		Jz	N_inc_Phc
		Cmp	Inswp,0
		Jnz	N_inc_Phc
		Mov	Al,Inschr
		Mov	Es:[Si],Al
I_ret:		Ret
N_inc_Phc:
		Cmp	Warning,0
		Jnz	I_ret
INSNEXT:
		Mov	Al,Inschr
		PuSh	Ax
		Mov	Es,Siseg
		Mov	Al,BYTE	PTR Es:[Si]
		Mov	Inschr,Al
		Pop	Ax
		Mov	Es:[Si],Al
		Call	Si_inc
		Cmp	Al,1ah
		Jz	Nlif
		Cmp	Al,0dh
		Jnz	Insnext
NLIF:		Mov	Last,Al
		Call	Caline
		Cmp	Si,1
		Jnz	Inr
		Mov	Bx,FileRow
		PuSh	Bx
		Mov	FileRow,Ax
		Call	Newline
		Mov	Al,inschr
		Mov	Es,Siseg
		Mov	Es:[Si],Al
		Pop	Ax
		Mov	FileRow,Ax
INR:		Ret

NEWLINE:
		Call	Insline
		Mov	Ax,FileRow
		Xor	Si,Si
		Call	Calps
		Mov	Cx,1
		Mov	Al,Last
		Mov	Es,Siseg
		Mov	Es:[Si],Al
		Call	Si_inc
		Jmp	Jdnx

INSLINE:
		Cmp	Warning,0
		Jnz	INR
		Mov	Crif,1
		Mov	Chang,1
		Cmp	Block,2
		Jnz	Niok
		Mov	Ax,FileRow
		Dec	Ax
		Cmp	Ax,Blockh
		Jae	Ni1
		Inc	Blockh
		Inc	Blocke
		Jmp	SHORT Niok
NI1:		Cmp	Ax,Blocke
		Ja	Niok
		Inc	Blocke
NIOK:		Cmp	Pian,2
		Jnz	Niok1
		Mov	Ax,FileRow
		Dec	Ax
		Cmp	Ax,Pianhr
		Jae	Ni11
		Inc	Pianhr
		Inc	Pianer
		Jmp	SHORT Niok1
NI11:		Cmp	Ax,Pianer
		Ja	Niok1
		Inc	Pianer
NIOK1:		Mov	Ax,FileRowt
		Sub	Ax,FileRow	; ȫ���������Ƶ����� ->	AX
		Or	Ax,Ax
		Jnz	Noz
		Mov	Ax,1
NOZ:		Xor	Dx,Dx
		Mov	Cl,EditWide
		Inc	Cl
		Xor	Ch,Ch
		Mul	Cx		; AX * 80 = ȫ���ֽ��� -> DX:AX
		Push	Ax
		Push	Dx
		Call	EW_TO_BP
		Mov	Si,Bp
		Dec	Si
		Mov	Ax,FileRowt
		Call	Calps		; calc new line's PS	 ->DI
		Push_si
		Mov	Ax,FileRowt
		Dec	Ax
		Call	EW_TO_BP
		Mov	Si,Bp
		Dec	Si
		Call	Calps		; cals the last	line's PS ->SI
		Pop_di
		Pop	Dx
		Pop	Cx
		Call	Mv_nx
		Inc	FileRowt
		Ret

INSLASTLINE:
		Mov	Al,EditWide
		Mov	Col,Al
		Mov	Ax,FileRowt
		Dec	Ax
		Mov	Filerow,Ax		; �ļ��е���ĩ��
		Call	NxcucCall		; ָ�뵽���һ���ַ�
		Mov	Isnew,1			; �Ƿ��һ�еı��(1=��)
		Mov	BYTE PTR Es:[Si],0dh	; ����س���
		Cmp	Col,0
		Jz	Addrok
		Mov	Isnew,0
ADDROK:		Inc	FileRow
		Mov	Col,0
		Call	Calps1			; ������һ���е�ָ��
		Push_Si
		Pop_Di				; �õ��ļ���������ĵ�ַ
		Ret

INSENTER:
		Cmp	Inswp,0
		Jnz	Ili
		Jmp	Cns1
Ili:		Inc	FileRow
		Mov	Last,0dh
		Call	Newline
		Mov	Ax,FileRow
		Xor	Si,Si
		Call	Calps
		PuSh_si
		Dec	FileRow
		Call	Calps1
		Pop_di
		PuSh_si
		Mov	Ch,1
		Call	Fullz
		Pop_si
		Mov	Es,Siseg
		Mov	BYTE PTR Es:[Si],0dh
		Cmp	Crif1,1
		Jz	Cns1
		Cmp	Col,0
		Jz	Cns1
		Mov	Ax,FileRow
		PuSh	Ax
		Mov	Si,Di
		PuSh_si
		Call	Caline
		Mov	FileRow,Ax
		Pop_si
		Mov	Al,0dh
		Mov	Ll,0
		Call	Dnr8
		Pop	Ax
		Mov	FileRow,Ax
Cns1:		Mov	Col,0
		Mov	Edind,0		; ���Ի�����λ
		Cmp	Insdent,0
		Jz	Cns2rok
		Cmp	Inswp,0
		Jz	Cns2rok
		Call	Calps1
Isdn:		Mov	Es,Siseg
		Mov	Al,Es:[Si]
		Cmp	Al,' '
		Jnz	Cns2
		Call	Si_inc
		Inc	Edind		; ����λ�� 1
		Jmp	SHORT Isdn
Cns2:		Xor	Ch,Ch
		Mov	Cl,Edind
		Or	Cl,Cl
		Jz	Cns2rok
		Mov	Ax,FileRow
		Inc	Ax
		Cmp	Ax,FileRowt
		Jae	Cns2rok
		Mov	FileRow,Ax
IndentNext:	Mov	Inschr,' '
		Push	Ax
		Push	Cx
		Call	Insert
		Pop	Cx
		Pop	Ax
		Mov	FileRow,Ax
		Mov	Col,0
		Loop	IndentNext
		Dec	FileRow
		Mov	Cl,Edind
		Mov	Col,Cl
Cns2rok:	Ret