; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   :	SE_INST.INC	    ┃
; ┃ 版本号   :	2.0		    ┃
; ┃ 作	 者   :	简晶		    ┃
; ┃ 日	 期   :	1991 年	12 月	    ┃
; ┃				    ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于插入操作的子程序, 有以下几个:
; -----------------------------------------------------
; Insert       -  将 Inschr 中的字符插入当前光标位置
; NewLine      -  新的一行
; InsLine      -  插入一空行
; InsLastLine  -  将文件末行插入一回车,	F3 读盘功能调用
; InsEnter     -  按回车键的处理
; -----------------------------------------------------

; 插入一字符, 预先放入 Inschr 中
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
		Sub	Ax,FileRow	; 全部将被搬移的行数 ->	AX
		Or	Ax,Ax
		Jnz	Noz
		Mov	Ax,1
NOZ:		Xor	Dx,Dx
		Mov	Cl,EditWide
		Inc	Cl
		Xor	Ch,Ch
		Mul	Cx		; AX * 80 = 全部字节数 -> DX:AX
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
		Mov	Filerow,Ax		; 文件行到最末行
		Call	NxcucCall		; 指针到最后一个字符
		Mov	Isnew,1			; 是否第一列的标记(1=是)
		Mov	BYTE PTR Es:[Si],0dh	; 放入回车符
		Cmp	Col,0
		Jz	Addrok
		Mov	Isnew,0
ADDROK:		Inc	FileRow
		Mov	Col,0
		Call	Calps1			; 计算下一新行的指针
		Push_Si
		Pop_Di				; 得到文件继续读入的地址
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
		Mov	Edind,0		; 初试化缩进位
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
		Inc	Edind		; 缩进位加 1
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