; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_CALC.INC         ┃
; ┃ 版本号   : 2.0                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1991 年 12 月       ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于计算方面的子程序, 有以下几个:
; -------------------------------------------------------------------------
;  B10asb1    -  计算从 [SI] 开始, 长度为 BX 的一串字符数据, 结果在 Binval
;  CalcNum    -  将 AX 的值转换为字符数据, 放在 Lines 缓冲区中; 默认长 4 位
;  Mzn        -  同上, 但可指定缓冲区在 SI, 长度在 CL
;  CalTab     -  计算 AL 值的制表位, 返回在 AL
;  Si_inc\ Si_dec\ Di_inc\ Di_dec\ Addsi\ Subsi\ Adddi\ Subdi\ Mov_di_to_si
;  CalPs1     -  计算第 FileRow 行, 第 Col 列的编辑文件指针; 返回在 Siseg:SI
;  CaLine     -  计算指针 Siseg:SI 在第几行, 第几列; 返回在 AX:SI
;  LowCase    -  将 AL 中的字符转换为小写; 返回在 AL
;  CalCircle  -  计算每个圈代表的节数
; -------------------------------------------------------------------------

B10ASB1:
                Mov     Binval,0
                Mov     Mult10,1
                Mov     Cx,10
B20:            Mov     Al,[Si+Bx-1]
                And     Ax,000fh
                Mul     Mult10
                Add     Binval,Ax
                Mov     Ax,Mult10
                Mul     Cx
                Mov     Mult10,Ax
                Dec     Bx
                Jnz     B20
                Ret

; 进入时, AX=欲转换数字
CALCNUM:
                Mov     Si,OFFSET lines
                Mov     Cl,4
MZN:            Mov     BYTE PTR [Si],30h
                Dec     Cl
                Inc     Si
                Or      Cl,Cl
                Jnz     Mzn     ; 首先将 lines 缓冲区全置 '0'
                Dec     Si
                Mov     Cx,10
C20:            Cmp     Ax,10
                Jb      C30     ; 处理到个位, 返回
                Xor     Dx,Dx
                Div     Cx
                Or      Dl,30h
                Mov     [Si],dl
                Dec     Si
                Jmp     SHORT C20
C30:            Or      Al,30h
                Mov     [Si],Al
                Ret

Chg_To_Tab:
                Cmp     Insdent,0
                Jnz     Cr_ind
                Cmp     Fal0a,0
                Jz      Chg_TT
Cr_ind:         Ret                     ; 缩进开(关)及列回车方式, 不转换
Chg_TT:         Push    Di
                Push_si
                Call    Caline
                Cmp     Block,2
                Jnz     Pi2
                Cmp     Ax,Blockh
                Jb      Pi2
                Cmp     Ax,Blocke
                Ja      Pi2
                Jmp     Chg_ret         ; 在块中间, 不转换
Pi2:            Cmp     Pian,2
                Jnz     Chg_ok          ; 无片定义,全转换
                Cmp     Ax,Pianhr
                Jb      Chg_ok          ; 片首行以上, 转换
                Cmp     Ax,Pianer
                Ja      Chg_ok          ; 片尾行以下, 转换
                Mov     Ax,Si
                Cmp     Al,Pianhc
                Jb      Chg_ok          ; 片首列以左, 转换
                Cmp     Al,Pianec
                Jbe     Chg_ret         ; 在片中间, 不转换
Chg_ok:         Mov     Ax,Si           ; Ax=列
                Call    CalTab          ; 计算此列TAB位
                Cmp     Al,EditWide
                Jbe     OKTab
                Mov     Al,EditWide     ; 超出编辑宽度,则取编辑宽度为TAB位
OKTAB:          Xor     Ah,Ah
                Mov     Cx,Ax
                Sub     Cx,Si           ; 长度 --> Cx
                Inc     Cl
                Pop_si
                Push_si
Nt_tab:         Call    Si_inc
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,' '
                Jnz     Nook_tab
                Loop    Nt_tab
Nook_tab:       Cmp     Cl,1
                Jbe     OKOK_tab        ; 可以转换为TAB, 转
Chg_ret:        Mov     Al,' '
                Pop_si                  ; 恢复原Siseg:Si
                Pop di                  ; 恢复原Diseg:Di
                Ret
OKOK_tab:       Mov     Al,TAB
                Call    Si_dec
                Pop_di                  ; 保留现有Siseg:Si
                Pop di                  ; 恢复原Diseg:Di
                Ret

CALTAB:
                Xor     Ah,Ah
                Mov     Cl,8
                Div     Cl
                Xor     Ah,Ah
                Mov     Cl,8
                Mul     Cl
                Add     Al,Cl
                Dec     Al
                Ret

SI_INC:
                Inc     Si
                Jnz     Siir
SIIR1:          Add     Siseg,1000h      ; siseg = siseg+1000h
SIIR:           Ret

ADDSI:
                Clc
                Adc     Si,Bp           ; SI = SI + BP
                Jc      Siir1
                Ret

SI_DEC:
                Dec     Si
                Cmp     Si,0ffffh       ; SI = SI-1
                Jnz     Sidr
SIDR1:          Sub     Siseg,1000h      ; siseg = siseg-1000h
SIDR:           Ret

SUBSI:
                Clc
                Sbb     Si,Bp           ; SI = SI - BP
                Jc      Sidr1
                Ret

DI_INC:
                Inc     Di
                Jnz     Diir
DIIR1:          Add     Diseg,1000h      ; diseg = diseg+1000h
DIIR:           Ret

ADDDI:
                Clc
                Adc     Di,Bp           ; DI = DI + BP
                Jc      Diir1
                Ret

DI_DEC:
                Dec     Di
                Cmp     Di,0ffffh
                Jnz     Didr
DIDR1:          Sub     Diseg,1000h
DIDR:           Ret

SUBDI:
                Clc
                Sbb     Di,Bp           ; DI = DI - BP
                Jc      Didr1
                Ret

MOV_DI_TO_SI:
                Mov     Si,Diseg
                Mov     Siseg,Si        ; siseg = diseg
                Mov     Si,Di           ;    si = di
                Ret

EW_TO_BP:
                Push    Ax
                Mov     Al,EditWide
                Inc     Al
                Xor     Ah,Ah
                Mov     Bp,Ax
                Pop     Ax
                Ret

;------------------------------------------------------
; 绝对地址 = FileRow * ( EditWide + 1 ) + Col --> Dx:Ax
;            Dx 左移 12 位, 得相应段地址
;            Ax 即为该段地址偏移量
; Dx 再加上 FileSto, 便得最终段地址
;------------------------------------------------------
CALPS1:
                Mov     Al,Col
                Xor     Ah,Ah
                Mov     Si,Ax           ; Si = Col
                Mov     Ax,FileRow      ; Ax = FileRow
CALPS:
                Push    Dx
                Push    Cx
                Mov     Cl,EditWide
                Inc     Cl
                Xor     Ch,Ch
                Xor     Dx,Dx
                Mul     Cx              ; FileRow*(EditWide+1) --> Dx:Ax
                Add     Ax,Si           ; Dx:Ax + Col (Si)
                Jnc     No_adx
                Inc     Dx
No_adx:         Mov     Cl,12
                Shl     Dx,Cl           ; Dx 中为段地址, Ax 中为偏移量
                Mov     Si,Ax
                Add     Dx,FileSto      ; 段址+FileSto --> DX
                Mov     Siseg,Dx        ; --> Siseg
                Pop     Cx
                Pop     Dx
                Ret

;------------------------------------------------------------------------
; 对应 Siseg:Si 的绝对偏移量指针 Dx:Ax
;               = Shr ( Siseg - FileSto ), 12 --> Dx, Si --> Ax, 得 Dx:Ax
; 此指针 Dx:Ax / (EditWide+1), 的行数 -- Ax, 余数 --> Dx
; 余数 Dx 即最终的剩余列数
;------------------------------------------------------------------------
CALINE:
                Push    Dx
                Push    Cx
                Mov     Ax,Si           ; Si --> Ax
                Mov     Dx,Siseg
                Sub     Dx,FileSto
                Mov     Cl,12
                Shr     Dx,Cl           ; Shr (Siseg-FileSto),12 --> Dx
                Mov     Cl,EditWide
                Inc     Cl
                Xor     Ch,Ch
                Div     Cx              ; Dx:Ax / (EditWide+1) --> Ax(行数)
                Mov     Si,Dx           ; SI=Dx=余数=最后的列数
                Pop     Cx
                Pop     Dx
                Ret

LOWCASE:
                Cmp     Uorl,0
                Jnz     Lowok
LOWCASE1:
                Cmp     Al,'A'
                Jb      Lowok
                Cmp     Al,'Z'
                Ja      Lowok
                Add     Al,32
LOWOK:          Ret

lowctrl:
                cmp     al,20h
                jae     lowcok
                add     al,60h
lowcok:
                ret

CalCircle:
                Mov     Ax,0a000h
                Sub     Ax,FileSto
                Mov     Cx,27*2
                Xor     Dx,Dx
                Div     Cx
                Mov     EachCircle,Ax
                Ret

To_Binval:
                Xchg    Ch,Cl
                Mov     Bx,Cx
                Xor     Bh,Bh
                Call    B10asb1
                Dec     Binval
                Ret

Get_Num:
                Mov     Si,OFFSET Gotobuf
                Call    To_Binval
                Mov     Ax,Binval
                Inc     Ax
                Ret
