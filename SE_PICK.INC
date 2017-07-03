; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   :	SE_PICK.INC	    ┃
; ┃ 版本号   :	2.0		    ┃
; ┃ 作	 者   :	简晶		    ┃
; ┃ 日	 期   :	1991 年	12 月	    ┃
; ┃				    ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于片操作的子程序,	有以下几个:
; -------------------------------------------
; PickDefin  -	片定义
; CopyPian   -	片粘贴
; PickOpra   -	其他片操作(填空,转大小写,存盘)
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
Rz_0:		Push	Ax			; 将定义的片转储缓冲区
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
		Mov	Pian,0		; 不在插行时修改片的大小
		Mov	Ax,Pianhr
		Push	Ax
		Mov	Ax,FileRow
		Mov	Di,OFFSET User_Buffer
NxPi2:		Push	Ax
		Xor	Si,Si
		Mov	Co,0
		Call	Calps		; 计算当前行指针 --> Siseg:Si
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
		Mov	Inschr,' '		; 第一个位置是半个汉字,则
		Mov	Ah,Last			; 在此汉字前插入一个空格
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
		Mov	Pianec,Al	; 转换片位置到当前光标
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
		Cmp	Al,4		; 若是存盘, 要先打开文件
		Jz	Ysave
		Jmp	Nsave
Ysave:		Call	Setpos_dh
		Mov	Si,OFFSET AskFilename
		Call	Inputname	 ; 输入文件名
		Cmp	Al,Esca
		Jz	Noextkey
		Or	Ch,Ch
		Jz	Noextkey	; ESC 及长度为零, 返回
		Mov	BYTE PTR [Si],0	; 结束符为零
		Xor	Cx,Cx
		Mov	Dx,OFFSET PianFilenaMe
		Mov	Ah,5bh
		Int	21h		; 先建立文件
		Jc	Pianfier
		Mov	Handlepian,Ax
		Jmp	Ncopy
Pianfier:	Cmp	Al,50h		; 文件存在则打开
		Jz	Exisd
		Jmp	PianErr
Exisd:		Call	Setpos_dh
		Mov	Si,OFFSET Overit
		Call	Dispatr&inkey	; 提示用户
		Jz	Noextkey
		Cmp	Al,'1'
		Jz	Overitok
		Cmp	Al,'2'
		Jz	Overitok
Noextkey:	Jmp	Editbegin	; 返回
Overitok:	Push	Ax
		Mov	Ax,3d02h
		Mov	Dx,OFFSET PianFilenaMe
		Int	21h
		Mov	Handlepian,Ax
		Pop	Bx
		Jnc	Nperr
		Jmp	PianErr		; 打开错转
Nperr:		Cmp	Bl,'2'
		Jz	Nextbyte1
		Jmp	Ncopy		; 不是追加转
Nextbyte1:	Mov	Filesh,0
		Mov	Filesl,0	; 初始长度为零
Nextbyte:	Mov	Ah,3fh
		Mov	Bx,Handlepian
		Mov	Cx,1
		Mov	Dx,OFFSET Pianbuf
		Int	21h		; 读一个字节
		Or	Ax,Ax		; AX=0 文件已结束转
		Jz	Readend
		Cmp	Pianbuf,1ah
		Jz	Readend		; 一直读到文件结束
		Inc	Filesl		; 长度递增
		Jnz	Nextbyte
		Inc	Filesh
		Jmp	SHORT Nextbyte
Readend:	Mov	Ax,4200h
		Mov	Bx,Handlepian
		Mov	Cx,Filesh
		Mov	Dx,Filesl
		Int	21h		; 绝对位移到正确结束位置
		Mov	Pianbuf,0dh	; 然后在此写入一回车符
		Call	Writepianbuf	; 以新的一行写入片的数据
		Mov	Pianbuf,0ah
		Call	Writepianbuf
		Jmp	SHORT Ncopy
Nsave:		Mov	Chang,1
		Cmp	Al,3
		Jnz	Ncopy
		Jmp	CopyPian	; 选择片拷贝转
Ncopy:		Mov	Ax,Pianhr
Fsnx1:		PuSh	Ax
		Mov	Bl,Pianhc
		Xor	Bh,Bh
		Mov	Si,Bx
		Mov	Bp,Bx		; BP 存放片宽度计数
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
NoP0:		Cmp	Cap,4		; 不是存盘转
		Jnz	NotPiansave
WritePian1:	Mov	Pianbuf,Al
WritePian:	Mov	Crsave,0
		Call	Writepianbuf
		Cmp	Pianbuf,0dh
		Jz	Addah
		Cmp	Pianbuf,0ah
		Jnz	NoCrsave
		Mov	Crsave,1	; 已存过回车则置1
		Jmp	Fsn
NoCrsave:	Cmp	Pianbuf,1ah
		Jnz	G_Msp1
G_Fsn:		Jmp	Fsn
G_Msp1:		Jmp	Msp1
Addah:		Mov	Pianbuf,0ah	; 完整的回车 ( 0DH + 0AH )
		Jmp	SHORT WritePian
NotPiansave:	Cmp	Al,0dh
		Jz	G_Fsn
		Cmp	Al,1ah
		Jz	G_Fsn
		Mov	Al,Es:[Si]
		Cmp	Cap,2
		Jz	Msp		; 选择删除转
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
		Jz	Ovitok		; 插入开时, 填空
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
		Call	DelCur		; 关时,	删除
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
		Jmp	Fsnx		; 小于片宽度转继续
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
		Jz	NotsaveCr	; 已回车转
		Dec	Ax
		PuSh	Ax
		Mov	Al,0dh
		Jmp	WritePian1	; 继续存回车符
NotsaveCr:	Jmp	Fsnx1		; 小于片高度转继续
Fsn1:		Cmp	Cap,2
		Jnz	Noqxp
		Mov	Pian,0
		Jmp	SHORT ShPi	; 若是删除, 则取消片
Noqxp:		Cmp	Cap,4		; 若是存盘, 则要关闭文件
		Jnz	ShPi
		Mov	Pianbuf,1ah
		Call	Writepianbuf	; 存结束符
		Mov	Ah,40h
		Mov	Bx,HandlePian
		Xor	Cx,Cx
		Xor	Dx,Dx
		Int	21h		; 截断文件
		Mov	Ah,3eh
		Mov	Bx,HandlePian
		Int	21h		; 关闭文件
ShPi:		Call	Show
		Jmp	Editbegin