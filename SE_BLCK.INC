; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_BLCK.INC         ┃
; ┃ 版本号   : 2.3                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1991,12 - 92, 8     ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于块操作的子程序, 有以下几个:
; -----------------------------------------
; AddBlock      -  块拷贝时公用子程序
; CalBlockHigh  -  算出块的高度在 BX
; BlockCopy     -  块拷贝到当前行前面
; BlockDel      -  块删除
; BlockDefin    -  块定义
; BlockMove     -  块移动到当前行前面
; BlockRead     -  读块文件到当前行前面
;------------------------------------------

ADDBLOCK:
                Add     Blockh,Bx
                Add     Blocke,Bx
                Inc     Blockh
                Inc     Blocke
                Ret

CALBLOCKHIGH:
                Mov     Ax,Blocke
                Sub     Ax,Blockh
                Mov     Bx,Ax           ; 块的高度 = 块尾-块首 -> BX
                Mov     Ax,FileRow
                Cmp     Ax,Blockh
                Ret

BLOCKCOPY:
                Push    Bx              ; 保存高度计数
                Mov     Chang,1
                Add     Bx,FileRowt     ; The last new line
                Mov     Ax,FileRowt
                Sub     Ax,FileRow
                Or      Ax,Ax
                Jnz     Nxokk
                Mov     Ax,1
Nxokk:          Xor     Dx,Dx
                Mov     Cl,EditWide
                Inc     Cl
                Xor     Ch,Ch
                Mul     Cx              ; DX:AX 中放全部要移动的数据
                PuSh    Ax
                PuSh    Dx
                Mov     Ax,Bx
                Call    EW_TO_BP
                Mov     Si,BP
                Dec     Si
                Call    Calps           ; calc the last new line's PS
                PuSh_si                 ; --> DI
                Mov     Ax,FileRowt
                Dec     Ax
                Call    EW_TO_BP
                Mov     Si,BP
                Dec     Si
                Call    Calps           ; cals the last old line's PS
                Pop_di                  ; --> SI
                Pop     Dx
                Pop     Cx
                Call    Mv_nx           ; Call to move it
                Mov     FileRowt,Bx
                Inc     FileRowt        ; FileRowt = BX+1
                Mov     Ax,FileRow
                Xor     Si,Si
                Call    Calps
                PuSh_si
                Mov     Ax,Blockh
                Xor     Si,Si
                Call    Calps
                Pop_di
                Pop     Cx              ; 恢复计数
Movnl:          Xor     Bp,Bp
Movnx:          Call    Addsi
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Call    CmpAbsAdress
                Jb      CopyMemok
                Inc     Cx
                Mov     Ax,FileRowt
                Sub     Ax,Cx
                Mov     FileRowt,Ax
                Mov     Ax,Blocke
                Sub     Ax,Cx
                Mov     Blocke,Ax
                Call    A0000_ret
                Jmp     SHORT Copyr
CopyMemok:      Call    Subsi
                Cmp     Al,1ah
                Jnz     Copyr2
                Call    Adddi
                Mov     Es,Diseg
                Mov     BYTE PTR Es:[Di],0dh
                Call    Subdi
                Call    Mov_di_to_si
                Call    Addsi
                Call    Si_inc
                PuSh_si
                Call    Caline
                Mov     Cx,Si
                Xor     Ch,Ch
                Pop_si
                Call    Jdnx
                Jmp     SHORT Copyr
Copyr2:         Call    Adddi
                Mov     Es,Diseg
                Mov     BYTE PTR Es:[Di],Al
                Call    Subdi
                Inc     Bp
                Mov     Al,EditWide
                Inc     Al
                Xor     Ah,Ah
                Cmp     Bp,Ax
                Jnb     Copyrrr
                Jmp     Movnx
Copyrrr:        Or      Cx,Cx
                Jz      Copyr
                Dec     Cx
                Call    Addsi
                Call    Adddi
                Jmp     Movnl
Copyr:          Mov     Crif,1
                Ret

BLOCKDEL:
                Mov     Chang,1
                Mov     Ax,FileRowt
                Mov     FileRowtOld,Ax
                Dec     Ax
                Sub     Ax,Blocke       ; 文件总行数减去块尾=需后移的行数
                Mov     Bx,Blocke
                Sub     Bx,Blockh
                Inc     Bx
                Sub     FileRowt,Bx     ; 文件总行数减掉块的高度
                Or      Ax,Ax
                Jnz     Nx111
                Mov     Ax,1
Nx111:          Xor     Dx,Dx
                Mov     Cl,EditWide
                Inc     Cl
                Xor     Ch,Ch
                Mul     Cx              ; Totel BYTEs of totle linEs
                PuSh    Ax              ; --> DX:AX
                PuSh    Dx
                Mov     Ax,Blockh
                Xor     Si,Si
                Call    Calps           ; Block head's PS --> DI
                PuSh_si
                Mov     Ax,Blocke
                Inc     Ax
                Cmp     Ax,FileRowtOld
                Jb      Nx333
                Dec     Ax
                Mov     Block,3
Nx333:          Xor     Si,Si
                Call    Calps           ; Block end+1's PS --> SI
                Pop_di
                Pop     Dx
                Pop     Cx
                Call    Mv_bx           ; Move (del) block
                Mov     Ax,Blockh
                Mov     FileRow,Ax
                call    midel_line      ; 局中
                Mov     WinRow,Ax
                Cmp     Block,3
                Jnz     Nx444
                Mov     Col,0
                Call    Surelast
                Inc     FileRowt
Nx444:          Mov     Block,0
                Ret

BLOCKDEFIN:
                Mov     Ax,FileRow
                Cmp     Block,0
                Jnz     Haveblock       ; 已定义过块, 转
                Mov     Block,2
                Mov     Blockh,Ax       ; 没定义过块, 则置块标志
Blockok:        Mov     Blocke,Ax       ; 块首,块尾等于 Filerow
                Jmp     SHORT Comp
Haveblock:      Cmp     Ax,Blockh       ; 在原块首的下面吗? 是转定义块尾
                Jae     Blockok
                Push    Ax              ; 否则与原块首互换
                Mov     Ax,Blockh
                Mov     Blocke,Ax
                Pop     Ax
                Mov     blockh,Ax
Comp:           Inc     Ax
                Cmp     Ax,FileRowt     ; 块尾与文件总行比较
                Jb      Ckok            ; 小于, 转返回
                Xor     Si,Si
                Call    Calps
                Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],1ah
                Inc     FileRow
                Call    Calps1
                Dec     FileRow
                Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],0dh
                Call    Si_inc
                Mov     Cl,Col
                Inc     Cl
                Xor     Ch,Ch
                Call    Jdnx
Ckok:           Mov     Ax,FileRow
                Push    Ax
                Mov     Al,Col
                Push    Ax
                Mov     Ax,Blockh
                Mov     FileRow,Ax
                Call    GotoLineBegin
                Call    StFileRow&Col
                Mov     Ax,FileRow
                Mov     Blockh,Ax
                Mov     Ax,Blocke
                Mov     FileRow,Ax
                Call    GotoLineEnd
                Call    StFileRow&Col
                Mov     Ax,FileRow
                Mov     Blocke,Ax
                Pop     Ax
                Mov     Col,Al
                Pop     Ax
                Mov     FileRow,Ax
                Call    Show
                Ret

BLOCKMOVE:
                Call    CalBlockHigh    ; 计算块高度 -> BX
                Mov     Isnew,0
                Jae     Nx80            ; 当前行在块之后, 转
                Call    AddBlock        ; 否则块亦往后移
                Mov     Isnew,1
                Jmp     SHORT Nx81
Nx80:           Cmp     Ax,Blocke
                Jbe     Ckokr1          ; 在块中间亦返回
Nx81:           Push    Bx
                Mov     Ax,Filerow
                Push    Ax
                Mov     Ax,WinRow
                Push    Ax
                Mov     Al,Col
                Mov     Ah,Row
                Push    Ax
                Call    BlockCopy       ; 拷贝块
                Call    BlockDel        ; 删除原块
                Pop     Bx
                Mov     Col,Bl
                Mov     Row,Bh
                Pop     Bx
                Mov     WinRow,Bx
                Pop     Bx
                Mov     FileRow,Bx
                Mov     Blockh,Bx       ; 置新块的起止标记
                Mov     Blocke,Bx
                Pop     Bx
                Add     Blocke,Bx
                Cmp     Isnew,1
                Jz      Adok            ; 若当前行在原快之后, 则
                Mov     Ax,Blockh       ; 块首应在减调高度之后的一行
                Dec     Ax
                Inc     Bx
                Mov     Blocke,Ax
                Sub     Blockh,Bx
Adok:           Mov     Block,2
                Mov     Editing,0
                Call    Show
Ckokr1:         Ret

BLOCKREAD:
                Mov     BYTE PTR [Si],0 ; 文件名以 0 结束
                Mov     Ax,3d02h
                Mov     Dx,OFFSET PianFileName
                Int     21h             ; 打开该文件
                Jnc     ReadOpenOk
                Jmp     PianErr
ReadOpenOk:     Mov     Chang,1
                Mov     Bx,Handle
                Push    Bx              ; 保存原文件号
                Mov     Handle,Ax
                Mov     Bx,FileRow
                Push    Bx              ; 保存原文件行
                Mov     Bl,Col
                Mov     Bh,Row
                Push    Bx              ; 保存原光标行列
                Call    InsLastLine     ; 最后一行变为空白行, 得到读入地址
                Mov     Ax,FileRowt
                Mov     Blockh,Ax
                Call    AllRight        ; 读入并转换文件, 然后定义为块
                Mov     Ax,FileRowt
                Dec     Ax
                Mov     Blocke,Ax
                Mov     Block,2
                Mov     Ah,3eh
                Mov     Bx,Handle
                Int     21h             ; 关闭文件
                Pop     Bx
                Mov     Col,Bl
                Mov     Row,Bh
                Pop     Bx
                Mov     FileRow,Bx
                Pop     Bx
                Mov     Handle,Bx
                Mov     Bx,FileRow
                Push    Bx
                Mov     Bx,WinRow
                Push    Bx
                Mov     Bl,Col
                Mov     Bh,Row
                Push    Bx
                Mov     Bx,Blocke
                Sub     Bx,Blockh
                Push    Bx              ; 高度入栈
                Call    AddBlock
                Call    BlockCopy       ; 拷贝块
                Cmp     Isnew,1
                Jnz     Nodecline       ; 若在第一列, 则应减调插入之新行
                Dec     Blockh          ; 即加到块中, 一起删除
Nodecline:      Call    BlockDel        ; 删除块
                Pop     Ax              ; 高度出栈 -> AX
                Pop     Bx
                Mov     Col,Bl
                Mov     Row,Bh
                Pop     Bx
                Mov     WinRow,Bx
                Pop     Bx
                Mov     FileRow,Bx
                Mov     Blockh,Bx       ; 块首为文件行
                Mov     Blocke,Bx
                Add     Blocke,Ax       ; 块尾为文件行 + 块高度
                Mov     Block,2         ; 置块标记
                Mov     Editing,0
                Mov     Undelete,0      ; 清恢复标记
                Jmp     Shed_old        ; 返回

PaiBlock:
                Mov     PaiBan,1
                Mov     DLIf,1
                Mov     Al,Inswp
                Mov     Ah,Insdent
                Push    Ax
                Mov     Ax,FileRow      ; Alt B 取文件当前行
                Mov     Inswp,1
                Cmp     AltBIf,0
                Jnz     PaiNLOk
                Mov     Insdent,0
                Mov     Ax,Blockh       ; Ctrl B 置文件行为块首
PaiNLOk:        Mov     FileRow,Ax
                Push    Ax
                Mov     Col,0
                Call    Calps1
                Mov     Cl,File_Wide    ; 排版宽度(汉字)
                Shl     Cl,1            ; 乘 2 得西文宽度
                Xor     Ch,Ch
SkipNextChr:    Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],0dh
                Jnz     SNC1
                Jmp     PaiNextLine1
SNC1:           Cmp     BYTE PTR Es:[Si],1ah
                Jnz     SNC2
                Jmp     PaiNextLine
SNC2:           Call    Si_inc
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],0dh
                Jnz     SNC3
                Jmp     PaiNextLine1
SNC3:           Cmp     BYTE PTR Es:[Si],1ah
                Jz      PaiNextLine
                Loop    SkipNextChr
                Call    StFileRow&Col
                Mov     Ax,FileRow
                Push    Ax
                Call    CorectCol       ; 对准光标位置
                Call    Calps1
                Mov     Es,Siseg
                Mov     Al,Es:[Si]      ; 判断末尾标点符号
                Cmp     Al,0a1h
                Jae     PaiHZ
                Mov     Di,OFFSET MoXW  ; 西文末字符
NxMXW:          Cmp     BYTE PTR [Di],0
                Jz      PaiBiaook
                Cmp     Al,[Di]
                Jz      Colaok1
                Inc     Di
                Jmp     SHORT NxMXW
PaiHZ:          Call    Si_inc
                Mov     Es,Siseg
                Mov     Ah,Es:[Si]
                Call    Si_dec          ; 退回原来位置
                Mov     Di,OFFSET MoHZ  ; 汉字末字符
NxMHZ:          Cmp     BYTE PTR [Di],0
                Jz      PaiBiaook
                Cmp     Ax,[Di]
                Jz      Colaok
                Inc     Di
                Inc     Di
                Jmp     SHORT NxMHZ
Colaok:         Call    Si_inc
Colaok1:        Call    Si_inc
                Call    StFileRow&Col
                Pop     Ax
                Mov     Ax,FileRow
                Push    Ax
PaiBiaook:      Call    InsEnter
                Pop     Ax
                Pop     Bx
                Cmp     AltBIf,0
                Jz      NoAltB
                Inc     Ax              ; Alt B 排下一行
                Jmp     PaiNLOk
NoAltB:         Push    Ax
PaiNextLine:    Pop     Ax
                Cmp     AltBIf,0
                Jnz     ABEnd
                Inc     Ax
                Cmp     Ax,Blocke
                Jae     ABEnd
                Jmp     PaiNLOk
ABEnd:          Pop     Ax
                Cmp     AltBIf,0
                Jnz     ABEnd1
                Mov     Insdent,Ah
ABEnd1:         Mov     Inswp,Al
                Mov     PaiBan,0
                Mov     DLIf,0
                Ret
PaiNextLine1:   Push_si
                Call    StFilerow&Col
                Inc     FileRow
                Mov     Col,0
                Call    Calps1
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Pop_si
                Cmp     Al,' '
                Jnz     ABok
                Cmp     AltBIf,0
                Jz      PaiNextLine
                Push_si
                Call    DelCurChr
                Pop_si
                Jmp     SHORT PaiNextLine1
ABok:           Cmp     Al,0dh
                Jnz     DBOKOK
                Pop     Ax
                Inc     Ax
                Push    Ax
                Jmp     PaiNextLine
DBOKOK:         Call    StFileRow&Col
                Call    DelCurChr
                Pop     Ax
                Jmp     PaiNLOk

Make_Tab:
                Mov     Chang,1
                Mov     Ax,FileRow
                Push    Ax
                Mov     Al,Col
                Mov     Ah,Inswp
                Push    Ax
                Mov     Inswp,1                 ; 置插入状态开
                Mov     Ax,Blockh
Next_Block_Lin: Mov     FileRow,Ax
                Xor     Si,Si
                Call    Calps
                Mov     Cl,EditWide
                Inc     Cl
                Xor     Ch,Ch
Next_Blk_Chr:   Mov     Es,Siseg
                Mov     Al,Es:[Si]
;                Cmp     Al,' '
;                Jbe     Next_Tab_Chg            ; 跳过控制符
                Cmp     Al,0a1h
                Jae     Next_Tab_Chg            ; 跳过汉字
                Mov     Di,OFFSET Tab_Graph
Next_Block_Chr: Cmp     Al,[Di]
                Jz      Tab_ok
                Inc     Di
                Cmp     BYTE PTR [Di],0
                Jnz     Next_Block_Chr
Next_Tab_Chg:   Call    Si_inc
                Loop    Next_Blk_Chr
                Mov     Ax,FileRow
                Inc     Ax
                Cmp     Ax,Blocke
                Jbe     Next_Block_Lin
                Pop     Ax
                Mov     Col,Al
                Mov     Inswp,Ah
                Pop     Ax
                Mov     FileRow,Ax
                Ret
Tab_ok:         Push_si
                Cmp     Al,16h                  ;'='
                Jz      Qtcr
                Cmp     Al,1Ch                  ;'-'
                Jnz     FQt
Qtcr:           Call    Si_inc
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],16h    ;'='
                Jz      FQt
                Cmp     BYTE PTR Es:[Si],1Ch    ;'-'
                Jz      FQt
                Pop_si
                Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],1Ch    ;'-'    ; 以 '-' 补上断裂处
                Jmp     SHORT Next_Tab_Chg
FQt:            Mov     Si,OFFSET Tab_Graph
                Sub     Di,Si
                Shl     Di,1
                Mov     Si,OFFSET Tab_9
                Add     Di,Si
                Pop_si
                Mov     Al,[Di]
                Mov     Es,Siseg
                Mov     Es:[Si],Al
                Call    Si_inc
                Mov     Al,[Di+1]
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],16H    ;'='    ; 后续 '='、'_' 及 ' ' 被覆盖
                Jz      No_HHZZ
                Cmp     BYTE PTR Es:[Si],1Ch    ;'-'
                Jz      No_HHZZ
                Cmp     BYTE PTR Es:[Si],' '
                Jz      No_HHZZ
                Mov     Inschr,Al               ; 其它的则插入
                Push_si
                Push    Cx
                Call    Insert_rep
                Pop     Cx
                Pop_si
                Jmp     SHORT N_1111
No_HHZZ:        Mov     Es:[Si],Al
N_1111:         Cmp     Cl,1
                Jbe     J_N_T
                Dec     Cl
J_N_T:          Jmp     Next_Tab_Chg
