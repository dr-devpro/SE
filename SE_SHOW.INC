; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_SHOW.INC         ┃
; ┃ 版本号   : 2.1                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 91,12 - 92,5        ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved              ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于字符显示及屏幕变化的子程序, 有以下几个:
; -----------------------------------------------------------
; Putc         -  显示一字符在当前光标位置
; SetColor     -  为 Attr 置颜色
; To_Scr       -  同 Putc, 但可区分字符的范围, 决定不同的颜色
; DispAtr      -  显示一字串, 以 0 结束
; ShowLine     -  显示 FileRow 行于编辑屏幕
; Show         -  显示整个当前编辑屏幕
; NewSh        -  插入/删除字符后, 调整显示新的内容
; Cls          -  清全屏幕
; Earse        -  清对话栏
; Scroll       -  滚动屏幕公用子程序
; ScrollDown1  -  下滚屏幕
; ScrollUp1    -  上滚屏幕
; ShIns        -  显示状态开关
; SeeLine      -  显示行号比例
; Window       -  显示编辑窗口
; -----------------------------------------------------------

vbuf_offset     dw      0
if_color        db      0
d_video         db      0               ; 直接写屏标记(1=Y)

PUTC:
                push    es
                push    di
                mov     di,VBUF         ;0b800h
                mov     es,di
                mov     di,vbuf_offset
                mov     ah,bl
                cmp     word ptr cs:VBUF,0B000h
                jz      short st_ok
                cmp     byte ptr psp_color,0
                jnz     short isbk      ;st_ok
                cmp     byte ptr if_color,0
                jz      short isbk      ;st_ok
                cmp     al,0a0h
                ja      short add_8
                cmp     al,'"'
                jz      short add_8
                cmp     al,"'"
                jz      short add_8
                cmp     al,'('
                jz      short add_8
                cmp     al,')'
                jz      short add_8
                cmp     al,'['
                jz      short add_8
                cmp     al,']'
                jz      short add_8
                cmp     al,'{'
                jz      short add_8
                cmp     al,'}'
                jz      short add_8
                cmp     al,'0'
                jb      short st_ok
                cmp     al,'9'
                ja      short st1_
                add     ah,3
                jmp     short st_ok
add_8:
                add     ah,8
                jmp     short st_ok
st1_:
                cmp     al,'z'
                ja      short st_ok
                cmp     al,'a'
                jb      short st2_
                add     ah,4
                jmp     short st_ok
st2_:
                cmp     al,'Z'
                ja      short st_ok
                cmp     al,'A'
                jb      short st_ok
                add     ah,7
                jmp     short st_ok
st_ok:
                cmp     byte ptr to_bjpm,0
                jnz     short isbk
                mov     bh,ah
                and     bh,0F0h
                cmp     bh,40h
                jz      short isbk
                cmp     bh,60h
                jz      short isbk
                mov     bh,lv
                and     bh,0F0h
                and     ah,0Fh
                or      ah,bh
isbk:
                cmp     byte ptr d_video,0
                jnz     short putc_dv
                push    bx
                Mov     Bh,mode         ; AL = ASCII 码
                mov     bl,ah           ; BL = ASCII 属性
                Mov     Cx,1
                Mov     Ah,9
                Int     10h
                pop     bx
putc_ret:
                pop     di
                pop     es
                retn
putc_dv:
                stosw
                jmp     short putc_ret

SETCOLOR:
; ----------------------------------------------
; 功能: 设置颜色 -> Attr, 提供 TO_SCR 子程序用
; 进入: DL  : 前景色
;       BL  : 背景色
; 返回: Attr: 存放属性
; ---------------------------------------------
                Mov     Al,Dh           ; DH = 背景色
                Mov     Cl,4
                Shl     Al,Cl           ; DL = 前景色
                Or      Al,Dl
                Mov     Attr,Al         ; 最后放在 Attr 中
                Ret

GetCtrlColor:
                cmp     word ptr cs:VBUF,0B000h
                jz      short gcc_r
                Cmp     Block,2
                Jnz     Nsok
                Mov     Bx,FileRow
                Cmp     Bx,Blockh
                Jb      Nsok
                Cmp     Bx,Blocke
                Ja      Nsok
                Mov     Bl,Attr ;87
gcc_r:
                Ret
Nsok:
                Mov     Bl,Lv
                And     Bl,0f0h ; 取背景
                Or      Bl,0ch  ; 红字
                Ret

to_bjpm         db      0

TO_SCR:
; -----------------------------------------------------------------------
; 功能: 显示编辑屏幕中的一个字符, 根据不同的字符用不同的颜色, 例:控制字符
; 进入: DL  : 要显示的字符
;       Attr: 存放编辑屏幕色
; 返回:
; -----------------------------------------------------------------------
                Mov     Bl,Attr         ; 预置显示属性为编辑屏幕色
                Mov     Al,Dl
                Cmp     Al,0ah
                Jz      Crokok
                Cmp     Al,0dh
                Jnz     Cont3           ; 非回车符转
Crokok:         Call    GetCtrlColor
                Mov     Al,Cr
                Cmp     Dl,0ah
                Jnz     Ns2
                Mov     Al,Cr1
Ns2:            Cmp     ShowCr,0
                Jz      Contt
                Mov     Al,20h
                Cmp     Dl,1ah
                Jnz     EofN
                Call    GetCtrlColor
                Mov     Al,26
                Jmp     Putc
EofN:           Cmp     Dl,0ah
                Jnz     Contt
                Mov     Al,0ah
Contt:
                cmp     al,0FFh
                jnz     short nsh1a
                mov     al,1Ah
                Call    GetCtrlColor
nsh1a:
                Cmp     Al,7fh
                Jnz     No@
                Call    GetCtrlColor
                Mov     Al,'@'
No@:
                Jmp     Putc            ; 显示该字符
Cont3:
                Cmp     Al,20h
                Jae     Contt
;                Cmp     Al,1ah
;                Jnz     Cont4
;                Mov     Al,eof
;                Mov     Bl,30h
;                Jmp     SHORT Ns2
;Cont4:
                Call    GetCtrlColor
                Cmp     ShowCr,0
                Jnz     Contt
                Add     Al,40h
                Jmp     SHORT Contt

DISPATR:
; -----------------------------------------------------------
; 功能: 从当前光标位置起显示一字符串, 遇回车符及响铃符可执行
;       不执行其他控制符, 不换行
; 进入: SI: 字符串存放起始缓冲区, 字串以 0 结束
;       BL: 字符串显示属性
;       DX: 起始光标位置, 进入前亦要先设置好
; 返回: 无
; -----------------------------------------------------------
                Mov     Al,[Si]
                Or      Al,Al
                Jz      Dispatrr        ; 字串以零结束
                Cmp     Al,0dh
                Jz      CtSh1
                Cmp     Al,0ah
                Jz      CtSh
                Cmp     Al,7
                Jz      CtSh
                Call    Putc
Dnxt:           Inc     Si
                Inc     Dl
                Call    Setpos
                Jmp     SHORT Dispatr
CtSh1:          Inc     Dh
                Mov     Dl,0ffh
CtSh:           Mov     Ah,2
                Push    Dx
                Mov     Dl,Al
                Int     21h             ; 显示特殊字符(回车等)
                Pop     Dx
                Jmp     SHORT Dnxt
Dispatrr:
                call    set_bios_pos
                Ret

SHOWLINE:
; ---------------------------------------------------------------
; 功能: 显示光标当前行于编辑屏幕, 可据此行位置决定 Attr 中的颜色
; 进入: FileRow: 当前光标所在的文件行
;       Editing: 决定是否在显示结束符后清除下半屏幕 (1=是)
; 返回: DL     : 0   (行结束时)
;              : 1Ah (文件结束时)
; ---------------------------------------------------------------
                Mov     Co,0
                Xor     Si,Si
                Mov     Ax,FileRow
                Call    Calps
                Mov     Al,Left_Mark
                Xor     Ah,Ah
                Mov     Bp,Ax
                Call    Addsi
                Call    CorectSi        ; 指针是否对准汉字
                Jbe     Shown1          ; 是, 转
                Mov     Dh,Rw           ; 否, 指针 Siseg:Si+1
                Xor     Dl,Dl
                Call    Setpos
                Mov     Al,20h
                Mov     Bl,Lv
                Call    Putc
                Mov     Co,1
Shown1:         Mov     Ax,FileRow
                Cmp     Block,2
                Jnz     Nobl
                Cmp     Ax,Blockh
                Jb      Nobl
                Cmp     Ax,Blocke
                Ja      Nobl
                Mov     Dx,0407h        ; 块红底白字
                Jmp     SHORT Set1
Nobl:           Mov     Dh,0
                Mov     Dl,Lv
Set1:           Cmp     Pian,2
                Jnz     Setco
                Cmp     Ax,Pianhr
                Jb      Setco
                Cmp     Ax,Pianer
                Ja      Setco
                Mov     Al,Co
                Add     Al,Left_Mark    ; 加上左边界->Co
                Cmp     Al,Pianhc
                Jb      Setco
                Cmp     Al,Pianec
                Ja      Setco
                Mov     Dx,0607h        ; 片棕底白字
Setco:          Call    Setcolor
                Mov     Dl,Co
                Mov     Dh,Rw
                Call    Setpos
                Mov     Es,Siseg
                Mov     Dl,Es:[Si]
                Or      Dl,Dl
                Jz      Shlrt           ; 遇零结束
                push    ax
                mov     al,screenwide
                cmp     co,al
                pop     ax
                Jb      Srt
                Call    CorectSi
                Jnz     Srt
                Call    Si_dec
                Mov     Dl,20h
Srt:            Call    To_Scr
                Inc     Co
                push    ax
                mov     al,screenwide
                cmp     co,al
                pop     ax
                Ja      Shlrt80         ; 到最后一列亦结束
                Cmp     Dl,1ah
                Jz      Shlrt
                Call    Si_inc
                Jmp     Shown1
Shlrt:          Push    Dx
                Mov     Dl,Co
                Mov     Dh,Rw
                Call    Setpos          ; 清除回车后面的列
                Mov     Al,20h          ; Al = ' '
                Mov     ah,Attr
                cmp     ah,47h          ; 是块颜色,继续显示此颜色
                jz      short blckok
                mov     ah,Lv
blckok:
                xor     ch,ch
                mov     cl,screenwide
                inc     cl
                Sub     Cl,Co
                call    stosw_sub
                Pop     Dx
Shlrt80:        Mov     Ax,FileRow
                Inc     Ax
                Cmp     Ax,FileRowt
                Jb      Shlrt_80        ; 显示到结束行返回
                Mov     Dl,1ah
Shlrt_80:       Cmp     Dl,1ah          ; 到文件尾须清除下部屏幕
                Jnz     Shlrt1
                Cmp     Editing,1       ; 不是编辑时的显示,转
                Jz      Shlrt1
                Mov     Ch,Rw
                Cmp     Ch,EditHigh
                Jae     Shlrt1
                Inc     Ch
                Xor     Cl,Cl
                mov     dl,screenwide
                Mov     Dh,EditHigh     ; Row=22, Col=79
                Mov     Ax,0600h
                Mov     Bh,Lv
                Int     10h
                Mov     Dl,1ah
Shlrt1:         Cmp     Dl,1ah
                Jz      Shlrt1_ok
                Xor     Dh,Dh
                Mov     Dl,EditWide
                Mov     Si,Dx
                Mov     Ax,FileRow
                Call    Calps
                Mov     Es,Siseg
                Mov     Dl,BYTE PTR Es:[Si]     ; Dl 取本编辑行最右一个字符
Shlrt1_ok:      Ret

SHOW:
; -------------------------------------------------------------------
; 功能: 显示当前整个编辑屏幕;
; 进入: WinRow: 当前窗口顶行所在的文件行
;       ShWinr: 当要求从其他行开始显示时, 放入文件行号; 调用 SHOWCALL
;       CH    : 当要求从其他行开始显示时, 放入屏幕行号; 调用 SHOWCALL
; 返回: 无
; -------------------------------------------------------------------
                Mov     Ax,WinRow
                Mov     ShWinr,Ax
                Mov     Ch,2
SHOWCALL:       Mov     Ax,FileRow
                PuSh    Ax
                Mov     Ax,ShWinr
                Mov     FileRow,Ax
                Mov     Al,Ch
Nextchr:        Mov     Rw,Al
                PuSh    Ax
                Call    Showline
                Pop     Ax
                Inc     Al
                Inc     FileRow
                Cmp     Dl,1ah
                Jz      Show_end
                Cmp     Al,EditHigh     ; 显示到最底行返回
                Jbe     Nextchr
Show_end:       Pop     Ax
                Mov     FileRow,Ax
                Ret

NEWSH:
; -------------------------------------------------------------------
; 功能: 插入或删除后决定该重新显示的是一行或多行
; 进入: Crif  : 若有过换行, 则置1, 应重新显示多行
;       Row   : 显示的屏幕行
; 返回: 无
; -------------------------------------------------------------------
                Cmp     Crif,0
                Jnz     Showall
                Mov     Editing,1
                Mov     Al,Row
                Mov     Rw,al
                Mov     Ax,FileRow
                Push    Ax
Nxnoend:        Call    Showline
                Or      Dl,Dl
                Jz      Allretok        ; 本行最末取到0,结束
                Cmp     Dl,0dh
                Jz      Allretok
                Cmp     Dl,1ah
                Jz      Allretok
                Inc     Rw
                Inc     FileRow
                Mov     Al,Rw
                Cmp     Al,EditHigh
                Jbe     Nxnoend
Allretok:       Pop     Ax
                Mov     FileRow,Ax
                Ret
SHOWALL:        Mov     Ax,FileRow
                Mov     Ch,Row
                Mov     ShWinr,Ax
                Mov     Editing,0
                Jmp     SHORT Showcall

CLS:
; -------------------------------------
; 功能: 清除整个屏幕, 光标到屏幕左上角
; 进入: 无
; 返回: 无
; -------------------------------------
                Mov     Al,' '
                Mov     Bl,7
                Call    Putc
                Mov     Ax,0600h
                Mov     Bh,7
                Xor     Cx,Cx
                mov     dl,screenwide
                Mov     Dh,EditHigh
                Add     Dh,2
                Int     10h
                Mov     Ah,2
                Mov     Bh,Mode
                Xor     Dx,Dx
                Int     10h
                Ret

EARSE:
; -----------------
; 功能: 清除对话框
; 进入: 无
; 返回: 无
; -----------------
                Pushf
                Push    Ax
                Push    Bx
                Push    Cx
                Push    Dx
                Call    Setpos_dh
                Mov     Cx,41
                Mov     Al,20h
                mov     ah,bl
                call    stosw_sub
                Pop     Dx
                Pop     Cx
                Pop     Bx
                Pop     Ax
                Popf
                Ret

stosw_sub:
                cmp     byte ptr d_video,0
                jnz     short sts_d
                mov     bl,ah
                mov     bh,mode
                mov     ah,9
                int     10h
                retn
sts_d:
                push    es
                push    di
                mov     di,VBUF         ;0b800h
                mov     es,di
                mov     di,vbuf_offset
                repz    stosw
                pop     di
                pop     es
                ret

SCROLL:
                Mov     Al,1
                Mov     Bh,Lv
                Mov     Cx,0200h
                mov     dl,screenwide
                Mov     Dh,EditHigh
                Int     10h
                Ret

SCROLLDOWN1:
                Mov     Ah,7
                Call    Scroll
                Mov     Rw,2
                Call    Showline
                Dec     WinRow
                Ret

SCROLLUP1:
                Mov     Ah,6
                Call    Scroll
                Mov     Ah,EditHigh
                Mov     Rw,Ah
                Call    Showline
                Inc     WinRow
                Ret

ShA&Hhz:
                mov     byte ptr to_bjpm,1
                Mov     Dx,014dh
                Call    Setpos
                Mov     Si,OFFSET AllHz
                Mov     Bl,Onc
                Cmp     IfFast,0
                Jz      Dis2
                Mov     Si,OFFSET HalfHz
                Mov     Bl,Offc
                Jmp     SHORT Dis2

SHINS:
                mov     byte ptr to_bjpm,1
                Mov     Dx,003dh
                Call    Setpos
                Mov     Si,OFFSET Insertoff
                Mov     Bl,Offc
                push    ds
                xor     cx,cx
                mov     ds,cx
                mov     cx,ds:[485h]
                pop     ds
                Cmp     Inswp,0
                Jz      Dis1
                Mov     Si,OFFSET Inserton
                Mov     Bl,Onc
                Mov     Cx,0b0fh
                cmp     VBUF,0b800h
                jnz     short Dis1
                mov     ch,0dh
Dis1:           Mov     Ah,01
                Int     10h
                Call    Dispatr
                Mov     Dx,013dh
                Call    Setpos
                Mov     Si,OFFSET Inserton
                Mov     Bl,onc
                Cmp     Insdent,0
                Jnz     Dis2
                Mov     Si,OFFSET Insertoff
                Mov     Bl,offc
Dis2:           Call    Dispatr
                mov     byte ptr to_bjpm,0
                Ret

SEELINE:
                mov     byte ptr to_bjpm,1
                mov     byte ptr if_color,0
                Mov     Al,Col
                Inc     Al
                Xor     Ah,Ah
                Call    Calcnum         ; 计算出当前列号 -> lines 缓冲区
                Mov     Al,linec
                Mov     Attr,Al
                Mov     Bp,76           ; 列号显示起始列
                Mov     Si,OFFSET Lines+1
SeeCol:         Mov     Dx,Bp
                Xor     Dh,Dh           ; 第 0 行
                Call    Setpos
                Mov     Dl,[Si]
                Call    To_Scr
                Inc     Si
                Inc     Bp
                Cmp     Bp,78
                Jbe     SeeCol
                Mov     Ax,FileRow
                Inc     Ax
                Call    Calcnum         ; 计算出当前行号 -> lines 缓冲区
                Mov     Bp,68           ; 行号显示起始列
                Mov     Si,OFFSET Lines
Seenext:        Mov     Dx,Bp
                Xor     Dh,Dh           ; 第 0 行
                Call    Setpos
                Mov     Dl,[Si]
                Call    To_Scr
                Inc     Si
                Inc     Bp
                Cmp     Bp,71
                Jbe     Seenext
                Xor     Ax,Ax
                Call    Calcnum         ; 缓冲区置 '0000'
                Mov     Si,OFFSET Lines
                Xor     Bp,Bp
                Mov     Ax,FileRow      ; 取当前行
                Inc     Ax
Muln:           Xor     Dx,Dx
                Mov     Cx,FileRowt
                Div     Cx              ; 除以总行数
                Add     Al,30h
                Mov     [Si+Bp],Al
                Inc     Bp
                Cmp     Bp,4
                Jz      Clok
                Or      Dx,Dx
                Jz      Clok
                Mov     Cx,10
                Mov     Ax,Dx
                Xor     Dx,Dx
                Mul     Cx
                Jmp     SHORT Muln
Clok:           Mov     Bp,70           ; 比例值显示起始列
                Mov     Si,OFFSET lines
Seenext1:       Mov     Dx,Bp
                Mov     Dh,1            ; 第 1 行
                Call    Setpos
                Mov     Dl,[Si]
                Call    To_Scr
                Inc     Si
                Inc     Bp
                Cmp     Bp,72
                Jbe     Seenext1
                Mov     Dx,014ah
                Call    Setpos          ; 小数点后一位
                Mov     Dl,[Si]
                Call    To_Scr
                Mov     Al,EditWide
                Inc     Al
                Xor     Ah,Ah
                Xor     Dx,Dx
                Mul     FileRowt
                Mov     Cx,16
                Div     Cx              ; 已使用空间 = FileRowt*(EditWide+1)/16
                Mov     CX,EachCircle
                Xor     Dx,Dx
                Div     Cx
                Mov     Cx,Ax           ; 得圆圈个数
                Or      Cl,Cl
                Jz      Nowarning
                Mov     Warning,0
                Cmp     Cl,27*2
                Jb      Nowarning
                Mov     Ah,2            ; 内存不够警告
                Mov     Dl,7
                Int     21h
                Mov     Cl,27*2
                Mov     Warning,1
Nowarning:      Cmp     Cl,Oldmem
                Jz      Shccok
                Mov     Oldmem,Cl
                Push    Cx
                Call    Showmenu4
                Pop     Cx
SeeC:           Or      Cl,Cl
                Jz      Shccok
                Mov     Dx,0100h
                Xor     Ch,Ch
Nxtcc:          Call    Setpos
                Mov     Bl,Lv1
                shl     bl,4            ; And     Bl,0f0h
                Or      Bl,0Fh
                Mov     Si,OFFSET Circle
                Push    Dx
                Push    Cx
                Call    Dispatr         ; 显示方框个数(内存情况)
                Pop     Cx
                Pop     Dx
                Inc     Dl
                Loop    Nxtcc
Shccok:         mov     byte ptr if_color,1
                mov     byte ptr to_bjpm,0
                Ret

WINDOW:
                mov     byte ptr to_bjpm,1
                mov     byte ptr if_color,0
                Xor     Dx,Dx
                Call    Setpos
                Mov     Si,OFFSET Menu1
                Mov     Bl,Menuc
                Call    Dispatr
                Mov     Dx,0137h
                Call    Setpos
                Mov     Bl,Menuc
                Mov     Si,OFFSET Menu5
                Call    Dispatr
                Mov     Dx,0037h
                Call    Setpos
                Mov     Bl,Menuc
                Mov     Si,OFFSET Menu6
                Call    Dispatr
                Call    Showmenu4
                Mov     Dx,0036h
                Call    Setpos
                Mov     Al,20h  ; ' '
                Mov     Bl,Lv1
                shl     bl,4
                Call    Putc
                Call    Shins
                Call    ShA&Hhz
                Mov     Dh,EditHigh
                Add     Dh,2
                Xor     Dl,Dl
                Call    Setpos
                Mov     Si,OFFSET Menu2
                Mov     Bl,70h
                Call    Dispatr
                Mov     Cx,10
                Mov     Si,OFFSET F1_F10
                Mov     Di,OFFSET F1_F10_Col
F_Nxt:          Mov     Dl,[Di]
                Mov     Dh,EditHigh
                Add     Dh,2
                Call    Setpos
                Mov     Bl,74h
;                Or      Bl,4
                Push    Di
                Push    Cx
                Call    Dispatr
                Pop     Cx
                Pop     Di
                Inc     Di
                Inc     Si
                Loop    F_Nxt
                Mov     Bl,Menuc
                Mov     Dh,EditHigh
                Inc     Dh
                Xor     Dl,Dl
                Call    Setpos
                xor     ch,ch
                mov     cl,screenwide
                inc     cl
                Mov     ah,Menuc
                Mov     Al,20h
                call    stosw_sub        ; 清对话行
                Xor     Dl,Dl
                Mov     Dh,EditHigh
                Inc     Dh
                Call    Setpos
                Cmp     Newf,0
                Jz      Old
                Mov     Si,OFFSET Nfileasc
                Mov     Bl,Menuc
                Call    Dispatr
Old:            Mov     Si,OFFSET Fileasc
                Call    Dispatr
                Mov     Si,OFFSET Filename
                call    Dispatr
                cmp     screenwide,90
                jae     short se100
                mov     byte ptr if_color,1
                mov     byte ptr to_bjpm,0
                ret
se100:
                mov     dh,edithigh
                add     dh,2
                mov     dl,80
                call    setpos
                mov     si,offset china_dragon
                mov     bl,0ceh
                call    dispatr
                mov     dx,80
                call    setpos
                mov     si,offset address
                mov     bl,08eh
                call    dispatr
                mov     dx,0150h
                call    setpos
                mov     si,offset tele
                mov     bl,08fh
                call    dispatr
                mov     byte ptr if_color,1
                mov     byte ptr to_bjpm,0
                ret

Showmenu4:
                Mov     Dx,0100h
                Call    Setpos
                Mov     Al,20h  ;' '
                Mov     ah,Lv1
                shl     ah,4
                Mov     Cx,55
                call    stosw_sub
                Ret

Sold:
                Mov     Ax,WinRow
                Mov     Dl,Row
                Xor     Dh,Dh
                Add     Ax,Dx
                Dec     Ax
                Dec     Ax
                Cmp     Ax,FileRowt
                Jb      Shed_ok         ; 光标未超过最底行, 转
                Mov     Ax,FileRowt
                Dec     Ax              ; FileRow = FileRowt - 1
                Push    Ax
                Sub     Ax,WinRow
                Inc     Ax
                Inc     Ax
                Mov     Row,Al          ; 新的光标行=(Filerowt-1)-Winrow+2
                Pop     Ax
Shed_ok:        Mov     FileRow,Ax
                Mov     Editing,0
                Call    Show
                Ret

Show_Num:
                mov     byte ptr if_color,0
                Push    Bp              ; Bp = 起始显示列
                Push    Ax
                Mov     Al,linec
                Mov     Attr,Al         ; 置颜色
                Mov     Dx,Bp
                Call    Setpos1_dh
                Mov     Dl,'['
                Call    To_Scr
                Mov     Dx,Bp
                Add     Dl,5
                Call    Setpos1_dh
                Mov     Dl,']'
                Call    To_Scr
                Pop     Ax
                Call    Calcnum         ; 转换 AX 宽度值
                Mov     Si,OFFSET lines
                Pop     Bp
                Inc     Bp
                Mov     Cx,4            ; 显示 4 位数
See_num:        Mov     Dx,Bp
                Push    Cx
                Call    Setpos1_dh
                Mov     Dl,[Si]
                Call    To_Scr          ; 显示一位数字
                Inc     Si
                Inc     Bp
                Pop     Cx
                Loop    See_num
                Mov     Dx,Bp
                Call    Setpos1_dh      ; 光标置于数字后
                mov     byte ptr if_color,1
                Ret
