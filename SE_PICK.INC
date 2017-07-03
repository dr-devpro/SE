; ������������������������������������
; �� �ļ���   :	SE_PICK.INC	    ��
; �� �汾��   :	2.0		    ��
; �� ��	 ��   :	��		    ��
; �� ��	 ��   :	1991 ��	12 ��	    ��
; ��				    ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved in ShenZhen  ��
; ������������������������������������
;
; ���ļ���������Ƭ�������ӳ���,	�����¼���:
; -------------------------------------------
; PickDefin  -	Ƭ����
; CopyPian   -	Ƭճ��
; PickOpra   -	����Ƭ����(���,ת��Сд,����)
; -------------------------------------------

PICKDEFIN:
		Mov	Ax,FileRow
		Cmp	Pian,0
		Jnz	Havepian
		Mov	Pianhr,Ax
Pianrok:	Mov	Pianer,Ax
		Jmp	SHORT Compcol
Havepian:	Cmp	Ax,Pianhr
		Jae	Pianrok
		Push	Ax
		Mov	Ax,Pianhr
		Mov	Pianer,Ax
		Pop	Ax
		Mov	Pianhr,Ax
Compcol:	Mov	Al,Col
		Cmp	Pian,0
		Jnz	Havepian1
		Mov	Pian,2
		Mov	Pianhc,Al
Piancok:	Mov	Pianec,Al
		Jmp	SHORT Compprok
Havepian1:	Cmp	Al,Pianhc
		jae	Piancok
		Push	Ax
		Mov	Al,Pianhc
		Mov	Pianec,Al
		Pop	Ax
		Mov	Pianhc,Al
Compprok:	Cmp	IfFast,0
		Jnz	NoHHHZ
		Mov	Al,Col
		Push	Ax
		Mov	Ax,FileRow
		Push	Ax
		Mov	Ax,Pianer
		Mov	FileRow,Ax
		Mov	Al,Pianec
		Mov	Col,Al
		Call	CmpCurasc
		Pop	Ax
		Mov	FileRow,Ax
		Pop	Ax
		Mov	Col,Al
		Jb	NoHHHZ
		Mov	Al,Pianec
		Cmp	Al,EditWide
		Jae	NoHHHZ
		Inc	Pianec
NoHHHZ:		Mov	Ax,Pianhr
		Mov	Di,OFFSET User_Buffer
Rz_0:		Push	Ax			; �������Ƭת��������
		Mov	Bl,Pianhc
		Xor	Bh,Bh
		Mov	Si,Bx
		Call	Calps
		Call	CorectSi
		Mov	Cl,Pianhc
		Jbe	Cz_0
		Inc	Cl
Cz_0:		Mov	Es,Siseg
		Mov	Al,Es:[Si]
		Cmp	Al,0dh
		Jz	Z_1
		Cmp	Al,1ah
		Jz	Z_1
		Or	Al,Al
		Jnz	Z_2
Z_1:		Mov	Al,' '
Z_2:		Mov	[Di],Al
		Inc	Cl
		Inc	Di
		Or	Di,Di
		Jnz	Z_3
		Dec	Di
Z_3:		Call	Si_inc
		Cmp	Cl,Pianec
		Jb	Cz_0
		Ja	Z_5
		Push	Cx
		Call	CorectSi
		Pop	Cx
		Jb	Cz_0
		Jz	Z_4
		Call	Si_dec
		Jmp	SHORT Cz_0
Z_4:		Dec	Cl
		Jmp	SHORT Cz_0
Z_5:		Mov	BYTE PTR [Di],0dh
		Inc	Di
		Pop	Ax
		Inc	Ax
		Cmp	Ax,Pianer
		Jbe	Rz_0
		Call	Show
		Ret

COPYPIAN:
		Mov	Pian,0		; ���ڲ���ʱ�޸�Ƭ�Ĵ�С
		Mov	Ax,Pianhr
		Push	Ax
		Mov	Ax,FileRow
		Mov	Di,OFFSET User_Buffer
NxPi2:		Push	Ax
		Xor	Si,Si
		Mov	Co,0
		Call	Calps		; ���㵱ǰ��ָ�� --> Siseg:Si
		Mov	Last,' '
Funs:		Mov	Es,Siseg
		Mov	Al,Es:[Si]
		Cmp	Al,0dh
		Jnz	Yesz2
		Mov	Last,0dh
		Jmp	SHORT Yesz
Yesz2:		Cmp	Al,1ah
		Jnz	YEsz1
		Mov	Last,1ah
		Jmp	SHORT Yesz
Yesz1:		Or	Al,Al
		Jnz	Noze
Yesz:		Mov	BYTE PTR Es:[Si],' '
Noze:		Call	Si_inc
		Inc	Co
		Mov	Al,Co
		Cmp	Al,Col		; untill to curpos
		Jbe	Funs
		Call	Si_dec
		Mov	Al,Last
		Cmp	Al,' '
		Jz	Okokok
		Mov	Es,Siseg
		Mov	Es:[Si],Al
Okokok:		Call	CorectSi
		Jbe	Ok_1
		Call	Si_dec
		Call	Si_dec
		Mov	Inschr,' '		; ��һ��λ���ǰ������,��
		Mov	Ah,Last			; �ڴ˺���ǰ����һ���ո�
		Push	Ax
		Push_si
		Call	Insert_rep
		Pop_si
		Pop	Ax
		Mov	Last,Ah
		Call	Si_inc
Ok_1:		Mov	Al,Pianhc
		Mov	Al,Col
		Mov	Rw,Al
NxPi1:		Mov	Al,[Di]
Oxk2:		Cmp	Al,0dh
		Jnz	Ox2
		Inc	Di
		Jmp	Zeok1
Ox2:		Cmp	Inswp,0
		Jz	Ovitok1
		Push_si
		Push	Di
		Mov	Inschr,Al
		Call	Insert_rep
		Pop	Di
		Pop_si
		Jmp	SHORT Tok2
Ovitok1:	Push	Ax
		Mov	Ah,Es:[Si]
		Cmp	Ah,0dh
		Jnz	O_1
		Mov	Last,0dh
O_1:		Cmp	Ah,1ah
		Jnz	O_2
		Mov	Last,1ah
O_2:		Mov	Al,Es:[Si]
		Call	StorDelChr
		Pop	Ax
		Mov	Es:[Si],Al	; copy Pian
Tok2:		Call	Si_inc
		Inc	Di
		Inc	Rw
		Mov	Al,Rw
		Cmp	Al,EditWide
		Jb	NxPi1
Zeok1:		Cmp	Inswp,0
		Jnz	Nxpi
		Mov	Al,Last
		Cmp	Al,' '
		Jnz	NxPi_1
		Push	Ax
		Call	CorectSi_F
		Pop	Ax
		Jz	Nxpi
NxPi_1:		Mov	Es,Siseg
		Mov	Es:[Si],Al	; if Col<79 then store CR or EOF
Nxpi:		Pop	Ax		; curline pop
		Inc	Ax		; and add 1
		Cmp	Ax,FileRowt
		Jae	Piced
		Inc	Pianhr		; Pian head add	1
		Mov	Bx,Pianhr
		Cmp	Bx,Pianer	; if not to Pian_end then countinue
		Ja	Piced
		Cmp	Inswp,0
		Jnz	JmpNxpi2ok
		Push	Ax
		Xor	Al,Al
		Push	Di
		Call	StorDelChr
		Pop	Di
		Pop	Ax
JmpNxpi2ok:	Jmp	NxPi2
Piced:		Pop	Ax		; else Pop old Pian_head_Row
		Mov	Pianhr,Ax
		Mov	Ax,Pianer
		Sub	Ax,Pianhr
		Add	Ax,FileRow
		Cmp	Ax,FileRowt
		Jae	NoMove
		Mov	Al,Pianec
		Sub	Al,Pianhc
		Add	Al,Col
		Cmp	Al,EditWide
		Ja	NoMove
		Mov	Pianec,Al	; ת��Ƭλ�õ���ǰ���
		Mov	Al,Col
		Mov	Pianhc,Al
		Mov	Ax,Pianer
		Sub	Ax,Pianhr
		Add	Ax,FileRow
		Mov	Pianer,Ax
		Mov	Ax,FileRow
		Mov	Pianhr,Ax
NoMove:		Mov	Pian,2
		Cmp	Inswp,0
		Jnz	Nopcsz
		Xor	Al,Al
		Call	StorDelChr
Nopcsz:		Call	Show		; Show all Screen
Pir:		Jmp	Editbegin	; return

PICKOPRA:
		Cmp	Pian,2
		Jnz	Pir
		Call	Setpos_dh
		Mov	Si,OFFSET Capmsg
		Call	Dispatr&inkey
		Jz	Pir
		Cmp	Al,'1'
		Jb	Pir
		Cmp	Al,'5'
		Ja	Pir
		Sub	Al,31h
Pick_Call:	Mov	Cap,Al
		Cmp	Al,4		; ���Ǵ���, Ҫ�ȴ��ļ�
		Jz	Ysave
		Jmp	Nsave
Ysave:		Call	Setpos_dh
		Mov	Si,OFFSET AskFilename
		Call	Inputname	 ; �����ļ���
		Cmp	Al,Esca
		Jz	Noextkey
		Or	Ch,Ch
		Jz	Noextkey	; ESC ������Ϊ��, ����
		Mov	BYTE PTR [Si],0	; ������Ϊ��
		Xor	Cx,Cx
		Mov	Dx,OFFSET PianFilenaMe
		Mov	Ah,5bh
		Int	21h		; �Ƚ����ļ�
		Jc	Pianfier
		Mov	Handlepian,Ax
		Jmp	Ncopy
Pianfier:	Cmp	Al,50h		; �ļ��������
		Jz	Exisd
		Jmp	PianErr
Exisd:		Call	Setpos_dh
		Mov	Si,OFFSET Overit
		Call	Dispatr&inkey	; ��ʾ�û�
		Jz	Noextkey
		Cmp	Al,'1'
		Jz	Overitok
		Cmp	Al,'2'
		Jz	Overitok
Noextkey:	Jmp	Editbegin	; ����
Overitok:	Push	Ax
		Mov	Ax,3d02h
		Mov	Dx,OFFSET PianFilenaMe
		Int	21h
		Mov	Handlepian,Ax
		Pop	Bx
		Jnc	Nperr
		Jmp	PianErr		; �򿪴�ת
Nperr:		Cmp	Bl,'2'
		Jz	Nextbyte1
		Jmp	Ncopy		; ����׷��ת
Nextbyte1:	Mov	Filesh,0
		Mov	Filesl,0	; ��ʼ����Ϊ��
Nextbyte:	Mov	Ah,3fh
		Mov	Bx,Handlepian
		Mov	Cx,1
		Mov	Dx,OFFSET Pianbuf
		Int	21h		; ��һ���ֽ�
		Or	Ax,Ax		; AX=0 �ļ��ѽ���ת
		Jz	Readend
		Cmp	Pianbuf,1ah
		Jz	Readend		; һֱ�����ļ�����
		Inc	Filesl		; ���ȵ���
		Jnz	Nextbyte
		Inc	Filesh
		Jmp	SHORT Nextbyte
Readend:	Mov	Ax,4200h
		Mov	Bx,Handlepian
		Mov	Cx,Filesh
		Mov	Dx,Filesl
		Int	21h		; ����λ�Ƶ���ȷ����λ��
		Mov	Pianbuf,0dh	; Ȼ���ڴ�д��һ�س���
		Call	Writepianbuf	; ���µ�һ��д��Ƭ������
		Mov	Pianbuf,0ah
		Call	Writepianbuf
		Jmp	SHORT Ncopy
Nsave:		Mov	Chang,1
		Cmp	Al,3
		Jnz	Ncopy
		Jmp	CopyPian	; ѡ��Ƭ����ת
Ncopy:		Mov	Ax,Pianhr
Fsnx1:		PuSh	Ax
		Mov	Bl,Pianhc
		Xor	Bh,Bh
		Mov	Si,Bx
		Mov	Bp,Bx		; BP ���Ƭ���ȼ���
		Call	Calps
		Push	Bp
		Call	CorectSi
		Pop	Bp
		Jbe	Fsnx
		Inc	Bp
Fsnx:		Mov	Es,Siseg
		Mov	Al,Es:[Si]
		Or	Al,Al
		Jnz	NoP0
		Mov	Al,0dh
NoP0:		Cmp	Cap,4		; ���Ǵ���ת
		Jnz	NotPiansave
WritePian1:	Mov	Pianbuf,Al
WritePian:	Mov	Crsave,0
		Call	Writepianbuf
		Cmp	Pianbuf,0dh
		Jz	Addah
		Cmp	Pianbuf,0ah
		Jnz	NoCrsave
		Mov	Crsave,1	; �Ѵ���س�����1
		Jmp	Fsn
NoCrsave:	Cmp	Pianbuf,1ah
		Jnz	G_Msp1
G_Fsn:		Jmp	Fsn
G_Msp1:		Jmp	Msp1
Addah:		Mov	Pianbuf,0ah	; �����Ļس� ( 0DH + 0AH )
		Jmp	SHORT WritePian
NotPiansave:	Cmp	Al,0dh
		Jz	G_Fsn
		Cmp	Al,1ah
		Jz	G_Fsn
		Mov	Al,Es:[Si]
		Cmp	Cap,2
		Jz	Msp		; ѡ��ɾ��ת
		Mov	Ah,Es:[Si]
		Cmp	Cap,1
		Jnz	Ucase
		Cmp	Ah,'A'
		Jb	G_Msp1
		Cmp	Ah,'Z'
		Ja	G_Msp1
		Add	Ah,20h
		Jmp	SHORT Mas
Ucase:		Cmp	Ah,'a'
		Jb	G_Msp1
		Cmp	Ah,'z'
		Ja	G_Msp1
		Sub	Ah,20h
Mas:		Mov	Es:[Si],Ah
		Jmp	SHORT Msp1
Msp:		Cmp	Inswp,0
		Jz	Ovitok		; ���뿪ʱ, ���
		Push_si
		Push	Bp
		Call	CorectSi
		Pop	Bp
		Jb	M_1
		Jnz	M_2
		Inc	Bp
		Jmp	SHORT M_1
M_2:		Call	Si_dec
M_1:		Push	Bp
		Mov	Dlif,1
		Call	DelCur		; ��ʱ,	ɾ��
		Mov	Dlif,0
		Pop	Bp
		Pop_si
		Jmp	SHORT Msp2
Ovitok:		Mov	Al,Es:[Si]
		Call	StorDelChr
		Call	CorectSi
		Jb	Oi_1
		Jz	Oi_2
		Call	Si_dec
		Jmp	SHORT Oi_1
Oi_2:		Call	Si_inc
		Mov	Es,Siseg
		Mov	BYTE PTR Es:[Si],' '
		Call	Si_dec
Oi_1:		Mov	Es,Siseg
		Mov	BYTE PTR Es:[Si],' '
Msp1:		Call	Si_inc
Msp2:		Inc	Bp
		Mov	Al,Pianec
		Xor	Ah,Ah
		Cmp	Bp,Ax
		Ja	Fsn
		Jmp	Fsnx		; С��Ƭ����ת����
Fsn:		Cmp	Cap,2
		Jnz	Nolip
		Xor	Al,Al
		Call	StorDelChr
Nolip:		Pop	Ax
		Inc	Ax
		Cmp	Ax,Pianer
		Ja	Fsn1
		Cmp	Cap,4
		Jnz	NotsaveCr
		Cmp	Crsave,1
		Jz	NotsaveCr	; �ѻس�ת
		Dec	Ax
		PuSh	Ax
		Mov	Al,0dh
		Jmp	WritePian1	; ������س���
NotsaveCr:	Jmp	Fsnx1		; С��Ƭ�߶�ת����
Fsn1:		Cmp	Cap,2
		Jnz	Noqxp
		Mov	Pian,0
		Jmp	SHORT ShPi	; ����ɾ��, ��ȡ��Ƭ
Noqxp:		Cmp	Cap,4		; ���Ǵ���, ��Ҫ�ر��ļ�
		Jnz	ShPi
		Mov	Pianbuf,1ah
		Call	Writepianbuf	; �������
		Mov	Ah,40h
		Mov	Bx,HandlePian
		Xor	Cx,Cx
		Xor	Dx,Dx
		Int	21h		; �ض��ļ�
		Mov	Ah,3eh
		Mov	Bx,HandlePian
		Int	21h		; �ر��ļ�
ShPi:		Call	Show
		Jmp	Editbegin