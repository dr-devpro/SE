; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_OTHR.INC         ┃
; ┃ 版本号   : 2.0                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1991 年 12 月       ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于其他一些数据搬移和比较子程序
; ------------------------------------------

CmpAbsAdress:
                Mov     Warning,0
                Push    Ax
                Push    Bx
                Push    Dx
                Mov     Ax,Diseg
                Mov     Bx,16
                Mul     Bx
                Clc
                Add     Ax,Di
                Adc     Dx,0            ; Diseg*16+Di 得绝对地址
                Cmp     Dx,0ah          ; 与 A0000H 比较
                Jb      CAAr1           ; 小于, 则未超出
                Mov     Warning,1
CAAr1:          Pop     Dx
                Pop     Bx
                Pop     Ax
                Ret

MV_CALL:
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Mov     Es,Diseg
                Mov     Es:[Di],Al
                Ret

MV_BX:
                Or      Dx,Dx
                Jnz     Mov_BxOk
                Or      Cx,Cx
                Jz      Mov_Bx_Rt
MOV_BXOK:       Call    CmpAbsAdress
                Jae     Go_A0000_ret
                Call    Mv_Call
                Call    Di_inc          ; 数据往前搬
                Call    Si_inc
                Clc
                Sbb     Cx,1
                Sbb     Dx,0
                Jmp     SHORT Mv_bx
MOV_BX_RT:      Ret

Go_A0000_ret:
                Jmp     A0000_ret

MV_NX:
                Or      Dx,Dx
                Jnz     Mov_NxOk
                Or      Cx,Cx
                Jz      Mov_Bx_Rt
MOV_NXOK:       Call    Mv_Call
                Call    Si_dec          ; 数据往后搬
                Call    Di_dec
                Clc
                Sbb     Cx,1
                Sbb     Dx,0
                Jmp     SHORT Mv_nx
FULLZ:
                Mov     Cl,Col
                Mov     Crif1,0
JDNX:           Or      Ch,Ch
                Jz      Fz
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Mov     Es,Diseg
                Mov     Es:[Di],Al
                Call    Di_inc
                Cmp     Al,0dh
                Jz      Criok
                Cmp     Al,1ah
                Jnz     Fz
CRIOK:          Mov     Crif1,1
FZ:             Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],0      ; 将本行剩下列填零
                Inc     Cl
                Call    Si_inc
                Cmp     Cl,EditWide
                Jbe     Jdnx
                Ret

SURELAST:
                Call    Calps1
                Mov     DLif,1
LDOK1:          Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,0dh
                Jz      Sureok          ; 删除到回车或文件尾(本行末)
                Cmp     Al,0ah
                Jz      Sureok
                Cmp     Al,1ah
                Jz      Sureok
                Push_si
                Call    DelCurChr
                Pop_si
                Jmp     SHORT LDOK1
Sureok:         Mov     DLif,0
                Xor     Al,Al
                Jmp     StorDelChr

RetnspcNosh:
                Mov     Move,1
                Call    Retnspc
                Mov     Move,0
                Ret
RETNSPC:
                Mov     Al,Col
                PuSh    Ax
                Mov     Al,EditWide
                Mov     Col,Al
                Call    Nxcuc
                Pop     Ax
                Mov     Col,Al
                Ret

NXCUC:
                Call    NxcucCall
                Jnz     Dirret
                Cmp     Move,0
                Jnz     DIRRET
                Call    NewSh
DIRRET:         Ret

NXCUCCALL:
                Call    Calps1
NXCU:           Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Or      Al,Al
                Jnz     Nxcur
                Call    Si_dec
                Dec     Col
                Jmp     SHORT Nxcu
NXCUR:          Cmp     Al,0dh
                Jz      Nxcur4
                Cmp     Al,1ah
                Jnz     Nxcur1          ; 末字符非回车及结束符, 返回
NXCUR4:         Mov     BYTE PTR Es:[Si],0
                Mov     Ah,Al
NXCUR2:         Call    Si_dec
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,' '
                Jnz     Nxcur3
                Cmp     Col,0
                Jz      Nxcur3
                Dec     Col
                Mov     BYTE PTR Es:[Si],0
                Jmp     SHORT Nxcur2
NXCUR3:         Call    Si_inc
                Mov     Es,Siseg
                Mov     Es:[Si],Ah
                Cmp     Ah,Ah
NXCUR1:         Ret

CoppCall:                              ; 跳词时,对准汉字
                Push    Cx
                Push    Ax
                Mov     Al,Col
                Push    Ax
                Mov     Ax,FileRow
                Push    Ax
                Push_si
                Call    StFileRow&Col
                Mov     Al,Col
                Push    Ax
                Call    CorectCol
                Pop     Ax
                Or      Al,Al
                Jz      Yes0Col
                Cmp     Al,Col
Yes0Col:        Pop_si
                Pop     Ax
                Mov     FileRow,Ax
                Pop     Ax
                Mov     Col,Al
                Pop     Ax
                Pop     Cx
                Ret

COPP:
                Call    Si_dec
                Inc     Cx
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,0a1h
                Jb      Coppr
                Mov     Ah,Al
                Call    Si_dec
                Inc     Cx
                Call    FileTopIf
                Jb      Copppr
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,0a1h
                Jb      Copppr
                Call    CoppCall
                Jnz     Copppr
                Xchg    Al,Ah
                Call    IfHZS
                Jmp     SHORT Coppr
Copppr:         Call    Si_inc
                Dec     Cx
                Mov     Al,Ah
COPPR:          Cmp     Al,0ah
                Jz      Norp0a
                Cmp     Al,'['
                Jb      Tdret
                Cmp     Al,']'
                Ja      Dret
                Mov     Al,'('
Dret:           Cmp     Al,'{'
                Jb      Tdret
                Cmp     Al,'~'
                Jae     Tdret
Norp0a:         Mov     Al,'!'
Tdret:          Ret

COPP1:
                Inc     Cx
                Call    Si_inc
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,0dh
                Jz      Tdret
                Cmp     Al,1ah
                Jz      Tdret
                Cmp     Al,0a1h
                Jb      Copp1r
                Call    CoppCall
                Jnz     Copp1r
                Mov     Ah,Al
                Call    Si_inc
                Inc     Cx
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,0a1h
                Jb      Coppp1r
                Call    IfHZS
                Jz      COPP1R
Coppp1r:        Call    Si_dec
                Dec     Cx
                Mov     Al,Ah
COPP1R:         Jmp     SHORT Coppr

IfHZS:
                Cmp     Ax,''         ; 九区制表符
                Jb      No_9
                Cmp     Ax,''
                Ja      No_9
                Mov     Ax,'！'
No_9:           Cmp     Ax,'　'         ; 一区中文符
                Jb      IfHZSr
                Cmp     Ax,'№'
                Ja      Hokokp1
                Mov     Ax,'！'
Hokokp1:        Cmp     Ax,'［'         ; 全角西文符
                Jb      Nolrbs
                Cmp     Ax,'］'
                Ja      Nolrbs
                Mov     Ax,'（'
Nolrbs:         Cmp     Ax,'｛'
                Jb      Nolrhbs
                Cmp     Ax,'｝'
                Ja      Nolrhbs
                Mov     Ax,'！'
Nolrhbs:        Cmp     Ax,'？'         ; 是汉字
                Ja      IfHZSr
                Cmp     Ax,'！'
                Jb      IfHZSr
                Cmp     Ax,'０'
                Jb      Copp1p
                Cmp     Ax,'：'
                Jb      IfHZSr
Copp1p:         Sub     Ax,'！'
                Add     Al,'!'          ; 转换成对应 ASC 符
                Cmp     Al,Al
IfHZSr:         Ret

Downc:
                Mov     Ax,FileRow
                Inc     Ax
                Cmp     Ax,FileRowt
                Jae     Downr
                Call    Retnspc
                Inc     FileRow
                Mov     Al,Row
                Cmp     Al,EditHigh
                Jae     Scrollup
                Inc     Al
                Mov     Row,Al
Downr:          Ret
Scrollup:       Call    Scrollup1
                Ret

UPC:
                Mov     Ax,FileRow
                Cmp     Ax,0
                Jz      Upr
                Call    RetnspcNosh
                Dec     FileRow
                Mov     Al,Row
                Cmp     Al,2
                Jz      Scrolldown
                Dec     Al
                Mov     Row,Al
Upr:            Ret
Scrolldown:     Jmp     Scrolldown1

SCIF:
                Mov     Al,Col
                Cmp     Al,0
                Jz      Upline  ; 左箭头到屏幕最左列
                Dec     Al
Nt5:            Mov     Col,Al
                Jmp     Nxcuc
Upline:         Call    RetnspcNosh
                Mov     Ah,Row
                Cmp     Ah,2
                Jnz     U
                Cmp     FileRow,0
                Jbe     Jupc
                Mov     Al,EditWide
                Mov     Col,Al
Jupc:           Call    Upc
                Jmp     Nxcuc
U:              Dec     Ah
                Dec     FileRow
                Mov     Al,EditWide
                Mov     Row,Ah
                Jmp     SHORT Nt5

CmpCurasc:
                Call    Calps1
CmpCall:        Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],0a1h
                jb      short okaaa0
                push    ax
                xor     al,al
                cmp     byte ptr es:[si],0FFh
                jnz     short n1aff
                cmp     al,0FFh
                jmp     short cal00
n1aff:
                mov     al,0FFh
                cmp     al,0
cal00:
                pop     ax
okaaa0:
                Ret
CmpPreasc:
                Call    Si_dec
                Jmp     SHORT CmpCall
CmpNxtasc:
                Call    Si_inc
                Jmp     SHORT CmpCall

CorectSi_F:
                Cmp     Iffast,1
                Jz      C_F_r
                Push_si
                Push    Ax
                Mov     IfChr,0
C_F_next:       Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],0a1h
                Jb      Cs_F_r
                Cmp     BYTE PTR Es:[Si],0FFh
                Jz      Cs_F_r
                Mov     Al,1
                Sub     Al,IfChr
                Mov     IfChr,Al
                Call    Si_inc
                Jmp     SHORT C_F_next
Cs_F_r:         Cmp     IfChr,0
                Pop     Ax
                Pop_si
C_F_r:          Ret

CorectSi:
                Cmp     Iffast,0
                Jnz     Cs_r1
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],0a1h
                Jb      Cs_r
                Cmp     BYTE PTR Es:[Si],0FFh
                Jz      Cs_r1
                Mov     IfChr,0
                Mov     Al,Col
                Push    Ax
                Push_si
                Call    Vernext
                Pop_si
                Pop     Ax
                Mov     Col,Al
                Cmp     IfChr,1
                Jz      Cs_r
                Call    Si_inc
                Mov     Al,0ffh
                Cmp     Al,Row
Cs_r:           Ret
Cs_r1:          Xor     Al,Al
                Cmp     Al,Row
                Ret
CorectCol:
                Cmp     Iffast,0
                Jnz     Ncloc           ; 不在全角状态,返回
                Cmp     IfChr1,0
                Jnz     Ncloc
                Mov     IfChr,1
                Call    CmpCurasc
                Jb      Ncloc
Vernext:        Call    FileTopif
                Jb      Verok
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],0a1h
                Jb      Verok
                Cmp     BYTE PTR Es:[Si],0FFh
                Jz      Verok
                Mov     Al,1
                Sub     Al,IfChr
                Mov     IfChr,Al
                Call    Si_dec
                Jmp     SHORT Vernext
Verok:
                Cmp     IfChr,0
                Jz      Ncloc
                Cmp     Col,0
                Ja      Coldok
                Inc     Col
                Inc     Col
Coldok:         Dec     Col
Ncloc:          Ret

FileTopIf:
                Mov     Es,Siseg
                Push    Dx
                Mov     Dx,Es
                Cmp     Dx,FileSto      ; 超出文件顶,则 Jb 成立
                Pop     Dx
                Ja      FTIR
                Cmp     Si,0
FTIR:           Ret

GotoLineEnd:
                mov     col,0
                Xor     Cx,Cx
                Call    Calps1
NxtChrLine1:    Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,1ah
                Jz      OneLineok1
                Cmp     Al,0dh
                Jz      Onelineok1
                Or      Al,Al
                Jz      OneLineok1
                Call    Si_inc
                Inc     Cx
                Jmp     SHORT NxtChrLine1
OneLineok1:     Or      Cx,Cx
                Ret

GotoLineBegin:
                Xor     Cx,Cx
                Call    Calps1
                Call    FileTopIf
                Jbe     OL1
NxtChrLine:     Call    Si_dec
                Inc     Cx
                Call    FileTopIf
                Jb      OneLineok
                Je      OL1
NoTop:          Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,0dh
                Jz      Onelineok
                Or      Al,Al
                Jnz     NxtChrLine
OneLineok:      Dec     Cx
OL:             Call    Si_inc
OL1:            Or      Cx,Cx
                Ret

GotoPrevWord:
                Call    FileTopIf
                Jbe     Chartr
                Xor     Cx,Cx
                Cmp     Al,'?'
                Ja      Chart
                Cmp     Al,'9'
                Ja      Jpns
                Cmp     Al,'0'
                Jae     Chart
Jpns:           Call    Copp
                Call    FileTopIf
                Jb      OL
                Je      Chartr
                Cmp     Al,'0'
                Jb      Jpns
                Cmp     Al,'?'
                Ja      Chartrr
                Cmp     Al,'9'
                Ja      Jpns
Chartrr:
                Call    Copp
                Call    FileTopIf
                Jb      OL
                Je      Chartr
                Cmp     Al,'?'
                Ja      Chartrr
                Cmp     Al,'9'
                Ja      Chartr
                Cmp     Al,'0'
                Jae     Chartrr
                call    si_inc
Chartr:
                Retn
Chart:
                Call    Copp
                Call    FileTopIf
                Jb      OL
                Je      Chartr
                Cmp     Al,'?'
                Ja      Chart
                Cmp     Al,'9'
                Ja      Jpns
                Cmp     Al,'0'
                Jae     Chart
                Jmp     SHORT Jpns

GotoNextWord_t:
                xor     cx,cx
                cmp     al,20h
                jz      short cure_zf20
                call    get_jgf
                jc      short cure_zf   ; 当前光标位置是字符,转
nxwt1:
                inc     cx
                Call    si_inc
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                cmp     al,0Dh
                jz      short cnwrr
                cmp     al,1Ah
                jz      short cnwrr
                call    get_jgf
                jnc     short nxwt1
                cmp     byte ptr cs:hz_xom,1
                jnz     short cnwrr
                dec     cx
                Call    si_dec
cnwrr:
                retn
cure_zf:
                inc     cx
                Call    si_inc
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                cmp     al,0Dh
                jz      short cnwrr
                cmp     al,1Ah
                jz      short cnwrr
                cmp     al,20h
                jz      short cure_zf20
                call    get_jgf
                jc      short cure_zf
                cmp     byte ptr cs:hz_xom,1
                jnz     short gnwt_r
                dec     cx
                Call    si_dec
gnwt_r:
                retn

cure_zf20:
                inc     cx
                Call    si_inc
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                cmp     al,0Dh
                jz      short cnwrr
                cmp     al,1Ah
                jz      short cnwrr
                cmp     al,20h
                jz      short cure_zf20
                retn

hz_xom          db      0

is_ghz:
                mov     byte ptr cs:hz_xom,1
                inc     cx
                call    si_inc
                cmp     al,0A9h
                jz      short is_jgf
                cmp     al,0A3h
                ja      short no_jgf
                jmp     short is_jgf            ; 汉字前三区,间隔符

get_jgf:
                mov     byte ptr cs:hz_xom,0
                cmp     al,0A0h
                ja      short is_ghz    ;no_jgf
                cmp     al,'_'
                jz      short no_jgf
                cmp     al,'0'
                jb      short is_jgf
                cmp     al,'9'
                jbe     short no_jgf
                cmp     al,'z'
                ja      short is_jgf
                cmp     al,'a'
                jae     short no_jgf
                cmp     al,'Z'
                ja      short is_jgf
                cmp     al,'A'
                jb      short is_jgf
no_jgf:
                stc
                retn
is_jgf:
                clc
                retn

GotoNextWord:
                Xor     Cx,Cx
                Cmp     Al,'?'
                Ja      Chart1
                Cmp     Al,'9'
                Ja      Jpns1
                Cmp     Al,'0'
                Jae     Chart1
Jpns1:          Call    Copp1
                Jz      Jpns3r
                Cmp     Al,'0'
                Jb      Jpns1
                Cmp     Al,'?'
                Ja      Jpns3r
                Cmp     Al,':'
                Jae     Jpns1
Jpns3r:         Ret
Chart1:         Call    Copp1
                Jz      Jpns3r
                Cmp     Al,'?'
                Ja      Chart1
                Cmp     Al,'0'
                Jb      Chart1r
                Cmp     Al,'9'
                Jbe     Chart1
Chart1r:        Jmp     SHORT Jpns1

StFilerow&Col:
                Call    Caline
                Mov     FileRow,Ax
                Mov     Ax,Si
                Cmp     Al,EditWide
                Jbe     Col_ok
                Mov     Al,EditWide
Col_ok:         Mov     Col,Al
                Ret

cx_scr          dw      0

RowBackUp:
                Mov     Ax,FileRow      ; 光标行是否回上 ( Jae = 否 )
                Push    Ax
                Cmp     Ax,WinRow
                Jae     Nclort1
                Mov     Cx,WinRow
                Sub     Cx,FileRow
                mov     cx_scr,cx
NxSD:           Push    Cx
                Mov     Cx,WinRow
                Dec     Cx
                Mov     FileRow,Cx
                cmp     cx_scr,1
                ja      short dcok
                Call    Scrolldown1
                jmp     short dced
dcok:
                dec     WinRow
dced:
                Pop     Cx
                Loop    NxSD
                Mov     Row,2
                cmp     cx_scr,1
                jbe     short rbur
                Call    sold
                Jmp     SHORT RBUR
Nclort1:        Sub     Bx,Ax
                Sub     Row,Bl
RBUR:           Pop     Ax
                Mov     FileRow,Ax
                Ret

RowForwDn:
                Mov     Ax,FileRow
                Push    Ax
                Sub     Ax,Bx
                Jz      Nalort
                Mov     Ax,WinRow
                Mov     Cl,EditHigh
                Xor     Ch,Ch
                Add     Ax,Cx
                Sub     Ax,2
                Cmp     Ax,FileRow
                Jae     Nalort1
                Mov     Ax,FileRow
                Sub     Ax,Cx
                Add     Ax,2
                Sub     Ax,WinRow
                Mov     Cx,Ax
                mov     cx_scr,cx
NxSU:           Push    Cx
                Mov     Cx,WinRow
                Mov     Al,EditHigh
                Xor     Ah,Ah
                Add     Cx,Ax
                Dec     Cx
                Mov     FileRow,Cx
                cmp     cx_scr,1
                ja      short okic
                Call    ScrollUp1
                jmp     short iced
okic:
                inc     WinRow
iced:
                Pop     Cx
                Loop    NxSU
                Mov     Al,EditHigh
                Mov     Row,Al
                cmp     cx_scr,1
                jbe     short RBDR
                Call    sold
Nalort:         Jmp     SHORT RBDR
Nalort1:        Mov     Ax,FileRow
                Sub     Ax,Bx
                Add     Row,Al
RBDR:           Pop     Ax
                Mov     FileRow,Ax
                Ret

PB:
                mov     byte ptr if_color,0
                Call    Setpos_dh
                Mov     Si,OFFSET BlkWide
                Cmp     Block,2
                Jz      Spdhok
                Mov     Si,OFFSET PhaWide
Spdhok:         Call    Dispatr
                Mov     Si,OFFSET SetWide
                Call    Dispatr
                Mov     Al,File_Wide
                Xor     Ah,Ah
                Mov     Bp,64
                Call    Show_Num
                mov     byte ptr if_color,1
                Mov     Dl,71
                Call    Input_Num
                Jnz     Setok1
Sfr1:           Jmp     Editbegin
Setok1:         Or      Ch,Ch
                Jz      Setok
                Call    Get_Num
                Cmp     Ax,3
                Jb      Setok
                Cmp     Ax,127          ; 排版宽度在3-127个汉字之间
                Ja      Setok
                Mov     File_Wide,Al
Setok:          Mov     AltBIf,0
                Cmp     Block,2
                Jz      Ctrl_Bok        ; 有块, 则排版块
                Mov     Ax,FileRow      ; 否则, 排版段落
                Push    Ax
                Mov     Blockh,Ax
                Mov     AltBIf,1
                Call    PaiBlock
                Pop     Ax
                Mov     FileRow,Ax
                Mov     Col,0
                Jmp     Shed_Old
Ctrl_Bok:       Call    PaiBlock
                Jmp     Topok           ; 转到块首

To_endl:
                Mov     Ax,FileRowt
                Sbb     Ax,WinRow
                Mov     Cl,EditHigh
                Xor     Ch,Ch
                Cmp     Ax,Cx
                Jb      D2
                Mov     Row,Cl
                Mov     Ax,WinRow
                Add     Ax,Cx
                Sub     Ax,2
                Mov     FileRow,Ax
                Retn
D2:
                Inc     Al
                Mov     Row,Al
                Mov     Ax,FileRowt
                Dec     Ax
                Mov     FileRow,Ax
                Retn
