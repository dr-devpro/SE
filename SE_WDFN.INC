; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_WDFN.INC         ┃
; ┃ 版本号   : 2.1                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1991,12 - 92,4      ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved              ┃
; ┗━━━━━━━━━━━━━━━━┛

Words:
                Cmp     Al,Esca
                Jnz     Nx0
                Jmp     Editbegin
Nx0:
                Cmp     Al,Ctrl_u
                Jnz     Nx1
Ctrl_U_Call:    Call    UndeltCall
                Jmp     GoNs&Ed
Nx1:
                Cmp     Al,Tab
                Jz      Nx1_ok
                Jmp     Nx2
Nx1_ok:         Mov     Al,Col
                Mov     Dlif,Al         ; 保存老的光标位在 Dlif
                Call    Caltab          ; 计算新的 TAB 位 -> AL
                Mov     Col,Al
                Inc     Col
Tokokok:        Cmp     Insdent,0
                Jz      T_ret           ; 缩进关, 直接返回
                Mov     Cl,Col
                Mov     Al,Dlif
                Mov     Dlif,Cl         ; 新位置 -> Dlif
                Sub     Cl,Al           ; 多出的长度 -> Cl
                Mov     Col,Al
                Mov     Al,Inswp
                Push    Ax
                Xor     Ch,Ch
                Mov     Inswp,1
I_tab:          Mov     Inschr,' '
                Push    Cx
                Call    Insert
                Pop     Cx
                Loop    I_tab
                Mov     Al,Dlif
                Cmp     Al,EditWide     ; 超出编辑宽度, 转 0 列
                Jb      I_2
                Xor     Al,Al
I_2:            Mov     Col,Al
                Pop     Ax
                Mov     Inswp,Al
I_1:            Call    Calps1
                Mov     Dlif,0
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],' '
                Jnz     T_ret1          ; 新的位置上若是空格,
                Jmp     Dinds           ; 则转 Dinds 删至下个字符
T_ret1:         Jmp     GoNs&Ed
T_ret:          Cmp     Al,EditWide
                Jb      T_ret_1
                Mov     Col,0
T_ret_1:        Call    Cmpcurasc
                Jb      T_ret_ok
                Inc     Col             ; 若正好在汉字右半部, 则前移一个位置
                Mov     Al,Col
                Cmp     Al,EditWide
                Jb      T_ret_ok
                Mov     Col,0
T_ret_ok:       Jmp     Editbegin
Nx2:
                Cmp     Al,Ctrl_y
                Jnz     Nx3
                Call    DelLeftPart     ; 删前半部
                Pushf
                Call    SureLast        ; 删后半部
                Call    DelCurChr       ; 删回车符
                Popf
                Jae     Ok_Ns1
                Jmp     Shed_old
Nx3:
                Cmp     Al,Enter
                Jnz     Nx4
                Call    InsEnter
                Call    Downc
                Jmp     Editbegin
Nx4:
                Cmp     Al,Ctrl_n
                Jnz     Nx5
                Mov     Ax,FileRow
                Push    Ax
                Call    GotoLineBegin
                Call    StFileRow&Col
                Call    InsEnter
                Pop     Bx
                Call    RowBackUp
Ok_Ns1:         Jmp     GoNs&Ed

Backr:          Jmp     Editbegin
Nx5:
                Cmp     Al,Backspace
                Jnz     Nx6
;                Call    RetnspcNosh
                Call    Calps1
                Call    FileTopIf
                Jbe     Backr
                Mov     Ax,FileRow
                Push    Ax
OvZr:           Call    FileTopIf
                Jbe     Engbok
                Call    Si_dec
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],0
                Jz      OvZr
                Cmp     Iffast,1
                Jz      Engbok
                Mov     Es,Siseg
                Call    CorectSi
                Jbe     Engbok                  ; 不属中文, 转
                Call    Si_dec
                Call    Si_dec
Engbok:         Call    StFileRow&Col
                Call    Calps1
                Mov     Es,Siseg
;                Cmp     BYTE PTR Es:[Si],0dh    ; 不删回车
;                Jz      NDCCOK
                Call    DelCurChr
;NDCCOK:
                Pop     Bx
                Call    RowBackUp
                Jmp     GoNs&Ed
Nx6:
                Cmp     Al,Ctrl_bs              ; Ctrl \
                Jnz     Nx7
Ctrl_Bs_Call:   Mov     Chang,1
                Call    Surelast
Ok_Ns:          Jmp     GoNs&Ed
Nx7:
                Cmp     Al,Ctrl_cb              ; Ctrl ]
                Jnz     Nx8
Ctrl_Bsp:       Mov     Chang,1
                Call    DelLeftPart
                Jae     Ok_Ns
                Jmp     Shed_old
Nx8:
                Cmp     Al,Ctrl_Backspace
                Jz      Ctrl_Bsp

                Cmp     Al,Ctrl_v
                Jnz     Nx9
                Mov     Al,1
                Sub     Al,Insdent
                Mov     Insdent,Al
                mov     byte ptr if_color,0
                Call    Shins
                mov     byte ptr if_color,1
                Jmp     Editbegin
Nx9:
                Cmp     Al,Ctrl_c
                Jnz     Nx9a
                Jmp     Pg_Dn_Call
Nx9a:
                Cmp     Al,Ctrl_R
                Jnz     Nx10
                Jmp     Pg_Up_Call
Nx10:
                Cmp     Al,Ctrl_d
                Jnz     Nx10a
                Jmp     Goright
Nx10a:
                Cmp     Al,Ctrl_S
                Jnz     Nx10b
                Jmp     Ctrl_C_l
Nx10b:
                Cmp     Al,Ctrl_E
                Jnz     Nx10c
                Jmp     C_Up_Call
Nx10c:
                Cmp     Al,Ctrl_X
                Jnz     Nx10d
                Jmp     C_Dn_Call
Nx10d:
                Cmp     Al,Ctrl_A
                Jnz     Nx10e
                Cmp     Col,40
                Jb      Zr_Col
                Sub     Col,40
Scr40:          Jmp     Editbegin
Zr_Col:         Mov     Col,0
                Jmp     SHORT Scr40
Nx10e:
                Cmp     Al,Ctrl_F
                Jnz     Nx11
                Mov     Al,Col
                Add     Al,40
                Jc      Scr40_2
                Cmp     Al,EditWide
                Jb      Scr40_1
Scr40_2:        Mov     Al,EditWide
Scr40_1:        Mov     Col,Al
                Jmp     SHORT Scr40
Nx11:
                Cmp     Al,Ctrl_q
                Jz      CQok
                Jmp     Nx12
CQok:           Mov     Si,OFFSET Quick
                Call    Setpos_dh
                Call    Dispatr&Inkey
                Jnz     HPok
                Jmp     Cqrr
HPok:           Call    Lowcase1
                call    lowctrl
                Cmp     Al,'e'          ; Ctrl_Hm
                Jnz     Nq1
                Jmp     Ctrl_Home_Call
Nq1:            Cmp     Al,'x'          ; Ctrl_Ed
                Jnz     Nq2
                Jmp     Ctrl_End_Call
Nq2:            Cmp     Al,'s'          ; Home
                Jnz     Nq3
                Jmp     Hmk_Call
Nq3:            Cmp     Al,'d'          ; End
                Jnz     Nq4
                Jmp     Edk_Call
Nq4:            Cmp     Al,'r'          ; Ctrl_Pu
                Jnz     Nq5
                Jmp     Ctrl_pu_Call
Nq5:            Cmp     Al,'c'          ; Ctrl_Pd
                Jnz     Nq6
                Jmp     Ctrl_pd_Call
Nq6:            Cmp     Al,'b'          ; 到片首
                Jnz     Nq7
                Cmp     Pian,2
                Jnz     Cqrr
                Mov     Al,Pianhc
                Mov     Col,Al
                Mov     Ax,Pianhr
                Jmp     Gtok
Nq7:            Cmp     Al,'k'          ; 到片尾
                Jnz     Nq8
                Cmp     Pian,2
                Jnz     Cqrr
                Mov     Al,Pianec
                Mov     Col,Al
                Mov     Ax,Pianer
                Jmp     Gtok
Nq8:            Cmp     Al,'h'          ; Ctrl_]
                Jnz     Nq9
                Jmp     Ctrl_Bsp
Nq9:            Cmp     Al,'y'          ; Ctrl_\
                Jnz     Nq10
                Jmp     Ctrl_Bs_Call
Nq10:           Cmp     Al,'l'
                Jnz     Nq11
                Jmp     Ok14            ; Ctrl_G
Nq11:           Cmp     Al,'a'
                Jnz     Nq12            ; Ctrl_QA 查找且替换
                Mov     CtrlQA,1        ; 置 1 → CtrlQA
                Mov     CtrlQF,0        ; 置 0 → CtrlQF
                Jmp     Find&Replace
Nq12:           Cmp     Al,'f'
                Jnz     Nq13            ; Ctrl_QF 仅查找,不替换
                Mov     CtrlQF,1        ; 置 1 → CtrlQF
                Mov     CtrlQA,0        ; 置 0 → CtrlQA
                Jmp     Find&Replace
Nq13:           Cmp     Al,'0'
                Jb      Cqrr
                Cmp     Al,'9'
                Ja      Cqrr
                And     Al,0fh
                Mov     Ah,Al
                Shl     Al,1
                Add     Al,Ah           ; Al * 3 --> Al
                Xor     Ah,Ah
                Mov     Di,OFFSET Work_Mark
                Add     Di,Ax
                Mov     Al,[Di]
                Mov     Col,Al          ; Col 取第一字节
                Mov     Ax,[Di+1]       ; Ax 取第二、三字节
                Jmp     Gtok
Cqrr:
                Jmp     Editbegin
Nx12:
                Cmp     Al,Ctrl_k
                Jz      Nx12ok
                Jmp     Nx13
Nx12ok:         Mov     Si,OFFSET FileOpra
                Call    Setpos_dh
                Call    Dispatr&Inkey
                Jz      Cqrr
                Call    Lowcase1
                call    lowctrl
                Cmp     Al,'b'
                Jnz     Nk1
                Mov     Ax,FileRow
                Mov     Blockh,Ax
                Cmp     Block,2
                Jz      Hvb
                Mov     Block,2
Blkkok:         Mov     Blocke,Ax
Blkkok1:        Call    Ckok
                Jmp     Editbegin
Hvb:            Mov     Bx,Blocke
                Cmp     Ax,Bx
                Jbe     Blkkok1
                Ja      Blkkok
Nk1:            Cmp     Al,'k'
                Jnz     Nk1A
                Mov     Ax,FileRow
                Mov     Blocke,Ax
                Cmp     Block,2
                Jz      Hvb1
                Mov     Block,2
Blbbok:         Mov     Blockh,Ax
Blbbok1:        Call    Ckok
                Jmp     Editbegin
Hvb1:           Mov     Bx,Blockh
                Cmp     Ax,Bx
                Jae     Blbbok1
                Jb      Blbbok
Nk1A:           Cmp     Al,'h'
                Jnz     Nk2
                Cmp     Block,2
                Jnz     Cqr
                Jmp     IsBlock
Nk2:            Cmp     Al,'c'
                Jnz     Nk3
                Jmp     Alt_C_Call
Nk3:            Cmp     Al,'v'
                Jnz     Nk4
                Jmp     Alt_V_Call
Nk4:            Cmp     Al,'y'
                Jnz     Nk5
                Jmp     Alt_Y_Call
Nk5:            Cmp     Al,'r'
                Jnz     Nk6
                Jmp     ReadBlockFile
Nk6:            Cmp     Al,'w'
                Jnz     Nk7
                Cmp     Block,2         ; 有块, 块存盘
                Jz      F3sok
                Mov     Blockh,0        ; 无块, 文件换名存盘
                Mov     Ax,FileRowt
                Dec     Ax
                Mov     Blocke,Ax
F3sok:          Jmp     F3_Save
Nk7:            Cmp     Al,'s'
                Jnz     Nk8
                Jmp     F2_Call
Nk8:            Cmp     Al,'x'
                Jnz     Nk9
                Jmp     Quit
Nk9:            Cmp     Al,'q'
                Jnz     Nk10
                Call    Sok
                Jmp     Qk
Nk10:           Cmp     Al,'0'
                Jb      Cqr
                Cmp     Al,'9'
                Ja      Cqr
                And     Al,0fh
                Mov     Ah,Al
                Shl     Al,1
                Add     Al,Ah
                Xor     Ah,Ah
                Mov     Di,OFFSET Work_Mark
                Add     Di,Ax
                Mov     Al,Col
                Mov     [Di],Al
                Mov     Ax,FileRow
                Mov     [Di+1],Ax
                Jmp     Editbegin
Cqr:            Jmp     Editbegin
Nx13:
                Cmp     Al,Ctrl_l
                Jnz     Nx14
ctrllok:
                Cmp     Finded,0        ; 是否曾搜寻过?
                Jz      Cqr
                Mov     Binval,0        ; 计数置0 (因为重新开始下一个)
                Call    Retnspc
                Call    Calps1
                PuSh_si
                Pop_di
                Call    Di_inc          ; 从当前光标位置+1开始
                Mov     IfrePl,0        ; 初始计数 0
                Jmp     Sbegin          ; 调用搜寻
Nx14:
                Cmp     Al,Ctrl_g
                Jz      Ok14
                Jmp     Nx15
Ok14:           Call    Retnspc
                Call    Calps1
                Push_si
                mov     byte ptr if_color,0
                Call    Setpos_dh
                Mov     Si,OFFSET Gotoline
                Call    Dispatr
                Call    Input_Num       ; 输入转向行号
                mov     byte ptr if_color,1
                Pop_di
                Jnz     Gotor1
Gfr1:           Jmp     Findr1
Gotor1:         Or      Ch,Ch
                Jz      Gfr1
                Call    Get_Num
                Dec     Ax
                Cmp     Ax,FileRowt
                Jb      Gotook
                Mov     Ax,FileRowt
                Dec     Ax
                Mov     Binval,Ax
Gotook:         Mov     FileRow,Ax
                Call    Calps1
                Call    Caline
                Mov     Ax,Binval
                Mov     Binval,0
                Jmp     Gotolinenum

Allret1:        Jmp     Editbegin

Nx15:
                Cmp     Al,Ctrl_p
                Jnz     Nx16
                Call    Inkey
                Jz      Allret1
                Cmp     Al,'@'          ; 0 以 7FH 替代, 显示 '@'
                Jnz     NxCtr1
@ok:
                Mov     Al,7fh
                Jmp     Ascok
NxCtr1:
                Cmp     Al,'2'
                Jz      @ok
                Cmp     Al,'6'
                Jnz     NxCtr2
@6ok:           Mov     Al,1eh  ;'^'
NxCtr2:         Cmp     Al,'-'
                Jnz     NxCtr3
@_ok:           Mov     Al,1fh  ;'_'
NxCtr3:
                Call    Lowcase1
                call    lowctrl
                Cmp     Al,60h
                Jb      Allret1
                Cmp     Al,'z'
                Jnz     short npz
                mov     al,0FFh
                Jmp     Ascok
npz:
                Cmp     Al,'m'
                Jz      Allret1
Lowasc:
                Sub     Al,60h
                jz      @ok
                Jmp     Ascok
Nx16:
                Cmp     Al,Ctrl_t
                Jnz     Nx17
                Call    Calps1
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],0dh
                Jz      Allret1
                Cmp     BYTE PTR Es:[Si],1ah
                Jz      Allret1
                Mov     Al,Col
                Push    Ax
                Mov     Al,Es:[Si]
                Call    GotoNextWord_t
;                Mov     Es,Siseg
;                Cmp     BYTE PTR Es:[Si],0dh
;                Jz      okdsi
;                Cmp     BYTE PTR Es:[Si],1ah
;                Jz      okdsi
;                call    si_dec
;                dec     cx
;                Cmp     BYTE PTR Es:[Si],20h
;                jnz     short okdsi
;                call    si_inc
;                inc     cx
;okdsi:
                Push    Cx
                Call    CorectCol
                Pop     Cx
                Mov     Ax,Cx
                Call    DelWordAx
                Pop     Ax
                Mov     Col,Al
                Jmp     GoNs&Ed
Nx17:
                Cmp     Al,Ctrl_b
                Jnz     Nx18
                Jmp     PB
Nx18:
                Cmp     Al,Ctrl_w
                Jnz     Nx19
up_scrl:
                Mov     Ax,WinRow
                Or      Ax,Ax
                Jz      Upscok
                Call    RetnspcNosh
                Mov     Ax,FileRow
                Push    Ax
                Mov     Ax,WinRow
                Dec     Ax
                Mov     FileRow,Ax
                Call    ScrollDown1
                Pop     Ax
                Dec     Ax
SetokNx19:      Mov     FileRow,Ax
Upscok:         Jmp     Editbegin

Nx19:
                Cmp     Al,Ctrl_z
                Jnz     Nx20
dn_scrl:
                Mov     Ax,FileRowt
                Sub     Ax,WinRow
                Mov     Cl,EditHigh
                Dec     Cl
                Xor     Ch,Ch
                Cmp     Ax,Cx
                Jbe     Upscok
                Call    RetnspcNosh
                Mov     Ax,FileRow
                Push    Ax
                Mov     Ax,WinRow
                Add     Ax,Cx
                Mov     FileRow,Ax
                Call    ScrollUp1
                Pop     Ax
                Inc     Ax
                Jmp     SetokNx19
Nx20:
                Cmp     Al,20h          ; 不接收控制字符
                Jae     Ascok
Allret:         Jmp     Editbegin

Ascok:
                call    Ascok_call
                Jmp     Goright         ; 光标右移

Ascok_call:
                Mov     Inschr,Al
                Cmp     IfFast,1
                Jz      English         ; 西文方式, 转
                Cmp     Inswp,1
                Jz      English         ; 插入方式, 转
                Cmp     Inschr,0a1h
                Jae     HZpro
                Mov     HanZi,0
                Jmp     SHORT NHZpro    ; 输入的字符不是汉字, 转字母处理
HZpro:          Cmp     HanZi,0
                Jz      NHZpro1         ; 前半字直接覆盖
                Mov     Hanzi,0
                Mov     Inswp,1
                Call    Insert          ; 后半字插入
                Mov     Inswp,0
                Jmp     SHORT Enlok     ; 转显示返回
NHZpro1:        Mov     HanZi,1
NHZpro:         Call    CmpCurasc
                Jb      English
                Cmp     BYTE PTR Es:[Si+1],0a1h
                Jb      English         ; 下个字符非汉字, 转
                Call    Delete          ; 删除汉字左半字符, 再覆盖右半字符
English:        Call    Insert          ; 接收输入字符
Enlok:          Call    NewSh           ; 显示在屏幕
                Mov     IfChr1,1
                ret
