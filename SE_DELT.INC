; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_DELT.INC         ┃
; ┃ 版本号   : 2.0                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1991 年 12 月       ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于删除操作的子程序, 有以下几个:
; -------------------------------------------
; Delete      - 删除光标位字符
; DelLine     - 删除光标所在当前文件行
; StoreDelChr - 将删除字符存入删除缓冲区
; -------------------------------------------

DELETE:
                Call    Calps1
DELETE_REP:
                Mov     Chang,1
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,1ah  ; 删除文件结束符吗? 是则返回
                Jnz     Dnr8
                Ret
Dnr8:           Cmp     Al,0dh  ; 删除回车, 转
                Jz      Unx1
                Jmp     Delnext
UNX1:           PuSh_si
                Xor     Si,Si
                Mov     Ax,FileRow
                Inc     Ax
                Call    Calps
                Pop_di
                Mov     Ll,0
UNX:            Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Mov     Es,Diseg
                Mov     Es:[Di],Al      ; 搬入新行
                Cmp     Al,1ah
                Jnz     Dnr4
                Dec     FileRowt        ; 文件结束, 总行减一
                Mov     Ll,1
                Jmp     SHORT dnr6
DNR4:           Cmp     Al,0dh
                Jz      Dnr6
                Call    Si_inc
                Call    Di_inc
                Jmp     SHORT Unx
DNR6:           PuSh_si
                PuSh_di
                Call    Mov_di_to_si
                Call    Si_inc
                PuSh_si
                Call    Caline
                Mov     Cx,Si
                Xor     Ch,Ch
                Pop_si
                Call    Jdnx
                Pop_si
                Call    Caline
                Mov     Bx,Ax          ; 新行行数 -> BX
                Pop_si
                Call    Caline
                Cmp     Ax,Bx          ; 老行行数 -> AX
                Jnz     Need
                Cmp     Ll,0
                Jz      Nneed
                Inc     FileRowt
NNEED:          Ret
NEED:           Mov     Bx,FileRow
                PuSh    Bx
                Mov     FileRow,Ax
                Call    Delline
                Pop     Ax
                Mov     FileRow,Ax
                Ret
DELNEXT:        Call    Si_inc
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Call    Si_dec
                Mov     Es,Siseg
                Mov     Es:[Si],Al
                Call    Si_inc
                Cmp     Al,0dh
                Jz      Dnr5
                Cmp     Al,1ah
                Jnz     Delnext
DNR5:           PuSh_si
                PuSh    Ax
                Call    Caline
                Pop     Bx              ; AX 返回行数, 不可破坏
                Cmp     Si,0
                Jnz     Dnr             ; 在第一列的情况下
                Cmp     Bl,1ah          ; 又是结束符
                Jnz     Nolastline
                Dec     FileRowt        ; 直接删除最底行
                Mov     Crif,1
                Mov     Chang,1
                Jmp     SHORT Lastokr
NOLASTLINE:     Call    Need
LASTOKR:        Pop_si
                Ret
DNR:            Pop_si
                Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],0
                Ret

DELLINE:
                Mov     Crif,1
                Mov     Chang,1
                Cmp     Block,2
                Jnz     Ncok
                Mov     Ax,FileRow
                Cmp     Ax,Blockh
                Jae     Nc1
                Dec     Blockh
                Dec     Blocke
                Jmp     SHORT Ncok
NC1:            Cmp     Ax,Blocke
                Ja      Ncok
                Mov     Ax,Blocke
                Cmp     Ax,Blockh
                Ja      Dbok
                Mov     Block,0
                Jmp     SHORT Ncok
DBOK:           Dec     Blocke
NCOK:           Cmp     Pian,2
                Jnz     Ncok1
                Mov     Ax,FileRow
                Cmp     Ax,Pianhr
                Jae     Nc2
                Dec     Pianhr
                Dec     Pianer
                Jmp     SHORT Ncok1
NC2:            Cmp     Ax,Pianer
                Ja      Ncok1
                Mov     Ax,Pianer
                Cmp     Ax,Pianhr
                Ja      Dbok1
                Mov     Pian,0
                Jmp     SHORT Ncok1
DBOK1:          Dec     Pianer
NCOK1:          Mov     Ax,FileRow
                Inc     Ax
                Cmp     Ax,FileRowt
                Jb      Dllok
                Call    Calps1          ; 文件末行处理
                Push_si
                Xor     Ch,Ch
                Call    Fullz
                Pop_si
                Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],1ah
                Ret
DLLOK:          Dec     Ax
                Xor     Si,Si
                Call    Calps
NEXTLINE:       Mov     Cl,EditWide
                Inc     Cl
NEXTDEL:        Call    EW_TO_BP
                Call    Addsi
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Call    EW_TO_BP
                Call    Subsi
                Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],Al
                Cmp     Al,1ah
                Jz      Delr
                Call    Si_inc
                Dec     Cl
                Or      Cl,Cl
                Jnz     Nextdel
                Jmp     SHORT Nextline
DELR:           Or      Cl,Cl
                Jz      Delr1
                Call    Si_inc
                Dec     Cl
                Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],0
                Jmp     SHORT Delr
DELR1:          Dec     FileRowt
                Ret

StorDelChr:
                Cmp     Al,1ah
                Jz      Nostrt
                Push_si
                Push    Es
                Mov     Es,Fileseg
                Mov     Si,Undelete
                Mov     Es:[Si],Al
                Inc     Undelete
                Pop     Es
                Pop_si
Nostrt:         Ret

BlockSTD:
                Mov     Blockd,1
                Mov     HanZi,1
                Mov     Ax,FileRow
                Push    Ax
                Mov     Al,Col
                Push    Ax
                Mov     Ax,Blockh
                Mov     FileRow,Ax
StNBL:          Mov     Col,0
                Call    StorLine
                Inc     FileRow
                Mov     Ax,FileRow
                Cmp     Ax,Blocke
                Jbe     StNBL
                Pop     Ax
                Mov     Col,Al
                Pop     Ax
                Mov     FileRow,Ax
                Mov     Blockd,0
                Xor     Al,Al
                Jmp     StorDelChr

StorLine:
                Push_si
                Mov     Al,Col
                Push    Ax
                Call    Calps1
StorLNC:        Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Or      Al,Al
                Jz      StorLret
                Cmp     Al,0dh
                Jnz     DirStor
                Cmp     HanZi,0            ; Ctrl \ 不删除回车 ( HanZi = 0 )
                Jz      Nostcr
DirStor:        Call    StorDelChr
Nostcr:         Inc     Col
                Mov     Al,Col
                Cmp     Al,EditWide
                Ja      StorLret
                Call    Si_inc
                Jmp     SHORT StorLNC
StorLret:       Cmp     Blockd,0
                Jnz     SL1
                Xor     Al,Al
                Call    StorDelChr
SL1:            Pop     Ax
                Mov     Col,Al
                Pop_si
                Ret

DelCurChr:
                Call    Calps1
DelCur:         Mov     Es,Siseg
                Mov     Al,BYTE PTR Es:[Si]
                Cmp     PaiBan,0
                Jnz     Nost1aor0
                Cmp     Al,1ah
                Jz      Nost1aor0
                Call    StorDelChr
Nost1aor0:      Push    Ax              ; 保存当前字符
                Push_si
                Call    Delete_rep
                Pop_si
                Pop     Ax
                Cmp     Iffast,0
                Jnz     Engdok
                Cmp     Al,0a1h          ; 当前字符非汉字, 转
                Jb      Engdok
                cmp     al,0FFh
                Jz      Engdok
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],0a1h
                Jb      Engdok          ; 不属中文, 转
                Cmp     BYTE PTR Es:[Si],0FFh
                Jz      Engdok          ; 不属中文, 转
                Mov     Al,BYTE PTR Es:[Si]
                Call    StorDelChr
                Call    Delete_rep
Engdok:
                Cmp     DLif,0          ; DLIF <> 0 不存 0
                Jnz     Engokret
                Xor     Al,Al
                Call    StorDelChr
Engokret:       Retn

UndeltCall:
                Mov     Al,Inswp
                Push    Ax
                Mov     Inswp,1
                Cmp     Undelete,0
                Jz      NodecUD
                Dec     Undelete
UndelNext:      Cmp     Undelete,0
                Jz      Undelok
                Dec     Undelete
                Mov     Di,Undelete
                Push    Ds
                Mov     Ds,FileSeg
                Mov     Al,[Di]
                Pop     Ds
                Or      Al,Al
                Jz      UnDelok
                Mov     Inschr,Al
                Cmp     Al,0dh
                Jnz     IsAscChr
                Mov     Al,Col
                Push    Ax
                Call    InsEnter
                Pop     Ax
                Mov     Col,Al
                Jmp     SHORT UndelNext
IsAscChr:       Call    Insert
                Jmp     SHORT UndelNext
Undelok:        Cmp     Undelete,0
                Jz      NodecUD
                Inc     Undelete
NodecUD:        Pop     Ax
                Mov     Inswp,Al
                Ret

DelWordAx:      Push    Ax              ; Ax = 计数
                Call    Calps1
                Pop     Ax
CBnext:         Or      Ax,Ax
                Jz      CBok
                Push    Ax
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Call    StorDelChr
                Push_si
                Call    Delete_Rep
                Pop_Si
                Pop     Ax
                Dec     Ax
                Jmp     SHORT CBnext
CBok:           Cmp     DLif,1
                Jz      CBrt
                Xor     Al,Al
                Call    StorDelChr
CBrt:           Ret

DelLeftPart:
                Mov     Ax,FileRow
                Push    Ax
                Call    GotoLineBegin   ; 返回本行首的 Siseg:Si 及计数 Cx
                Or      Cx,Cx
                Jz      OLORT
                Push    Cx
                Call    StFileRow&Col
                Pop     Ax
                Mov     DLif,1
                Call    DelWordAx
                Xor     Al,Al
                Call    StorDelChr
OLORT:          Mov     DLif,0
                Pop     Bx              ; 原 FileRow 文件行
                Jmp     RowBackUp
