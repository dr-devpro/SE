; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_FILE.INC         ┃
; ┃ 版本号   : 2.0                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1991 年 12 月       ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于文件操作的子程序, 有以下几个:
; -----------------------------------------------------------------------------
; InputName     -  在对话栏, 以 PianFileName 为缓冲区, 接收 25 个字符
; WritePianBuf  -  将 PianBuf 中的一个字节写入文件号为 HandlePian 的文件
; SaveFile      -  将缓冲区 FileSeg:0 开始的长 ReadSize 的数据写入文件号 Handle
; Sok           -  将整个编辑文件存盘
; OpenFile      -  打开主编辑文件, 并转换进编辑区
; Tuin          -  文件存盘时, 删除每行行尾的空格符
; -----------------------------------------------------------------------------

Open_Se_Cfg:
                Mov     Ax,3d00h        ; 读方式打开配置文件
                Mov     Dx,OFFSET Se_Cfg
                Int     21h
                Jc      Open_Ret
                Mov     Cx,79
                Mov     Dx,OFFSET Cfg_Msg
                Mov     Bx,Ax
                Mov     Ah,3fh
                Int     21h             ; 读入 79 字节
Cfg_ok:         Jc      Open_Ret
                Mov     Ah,3eh
                Int     21h
Open_Ret:       Ret

Save_Se_Cfg:
                cmp     word ptr cs:VBUF,0B000h
                jnz     short nof0lv
                mov     byte ptr cs:lv,0F0h
nof0lv:
                Mov     Ah,3ch          ; 建立新配置文件
                Mov     Dx,OFFSET Se_Cfg
                Xor     Cx,Cx
                Int     21h
                Jc      Open_Ret
                Mov     Cx,79
                Mov     Dx,OFFSET Cfg_Msg
                Mov     Bx,Ax
                Mov     Ah,40h
                Int     21h             ; 写入 79 字节
                Jmp     SHORT Cfg_ok

INPUTNAME:
                mov     byte ptr cs:if_color,0
                Call    Dispatr
                Mov     Si,OFFSET PianFileName
                Mov     Cl,23   ;25
                Call    Input   ; 输入读盘文件名
                mov     byte ptr cs:if_color,1
                Ret

WRITEPIANBUF:
                Mov     Ah,40h
                Mov     Bx,HandlePian
                Mov     Cx,1
                Mov     Dx,OFFSET PianBuf
                Int     21h
                Ret

SAVEFILE:
                PuSh    Ds              ; Readsize = 缓冲区长度
                Mov     Ah,40h
                Mov     Bx,Handle
                Mov     Cx,ReadSize
                Mov     Dx,FileSeg
                Mov     Ds,Dx
                Xor     Dx,Dx
                Int     21h
                Pop     Ds
                Ret

SOK:
                Xor     Si,Si
                Mov     Es,Filesto
                Mov     Siseg,Es
                Mov     Ax,4200h
                Mov     Bx,Handle
                Xor     Cx,Cx
                Xor     Dx,Dx
                Int     21h             ; 移指针到文件头
Sok1:           Xor     Di,Di
Sok11:          Mov     Es,Siseg        ; Siseg:Si 已指向起始存盘处
                Mov     Al,Es:[Si]
                Cmp     Al,' '
                Jnz     Not_Chg_Tab
                Call    Chg_To_Tab      ;判断并转换为TAB
                                        ;返回AL=TAB,则转换成功,且SI改变
                                        ;否则, Al 与 SI 都不变
Not_Chg_Tab:
                Cmp     Al,0ah
                Jnz     No0a
                Mov     Al,0dh
No0a:
                Cmp     Al,7fh
                Jnz     No_7fh
                Xor     Al,Al           ; 7FH转换为0
No_7fh:
                Mov     Es,Fileseg
                Mov     Es:[Di],Al
                cmp     al,0FFh
                jnz     short no1ae
                mov     byte ptr es:[di],1Ah     ; 0FFh 转换为 1Ah
no1ae:
                Inc     Di
                Call    Si_inc
                Cmp     Al,1ah
                Jz      Sokret
                Cmp     Di,0ffffh
                Jnz     Cppd
Saveblock:
                Mov     Readsize,0ffffh         ; 64K 内容存盘
                PuSh    Ax                      ; 最后一个写盘字符入栈
                Call    SaveFile
                Pop     Ax                      ; 出栈, 是否回车
                Cmp     Al,0dh
                Jnz     Sok1                    ; 是则要进行下一行处理
                Xor     Di,Di
Cppd:           Cmp     Al,0dh
                Jnz     Sok11
                Call    Tuin
                Inc     Di
                Mov     BYTE PTR Es:[Di],0ah
Sok3:           Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Or      Al,Al
                Jz      Sok2
                Inc     Di
                Cmp     Di,0ffffh
                Jz      Saveblock
                Jmp     SHORT Sok11
Sok2:           Call    Si_inc
                Jmp     SHORT Sok3
Sokret:         Call    Tuin
                Inc     Di
                Mov     Readsize,Di
                Call    SaveFile
                Mov     Ah,40h
                Mov     Bx,Handle
                Xor     Cx,Cx
                Xor     Dx,Dx
                Int     21h             ; 截掉文件后面的内容
                Ret

OPENFILE:
                Mov     Ax,3d02h        ; 读写方式打开文件
                Mov     Dx,OFFSET Filename
                Int     21h
                Jnc     Openok
                Cmp     Al,2            ; AX = erreor code
                Jnz     Error           ; AX = 2 is File not exist
                Xor     Cx,Cx
                Mov     Dx,OFFSET Filename
                Mov     Ah,5bh
                Int     21h             ; 建立该文件
                Jc      Error
                Mov     Newf,1          ; 置新文件标记
                Jmp     Openok          ; 正确转
Error:          Mov     Dx,0200h
                Push    Ax
                Call    Setpos
                Pop     Ax
                Call    Geterrmsg
                Jmp     Over
Openok:         Mov     Fileseg,Ds
                Add     Fileseg,1000h   ; 缓冲区段 = DS+1000h
                Mov     Diseg,Ds
                Add     Diseg,2000h     ; 编辑区段 = DS+2000h
                Mov     Filesto,Ds
                Add     Filesto,2000h
                Xor     Di,Di
                Mov     Handle,Ax
AllRight:       Mov     Ax,4202h
                Mov     Bx,Handle
                Xor     Cx,Cx
                Xor     Dx,Dx
                Int     21h             ; 取文件长度 -> Dx:Ax
                mov     Filesl,1
                mov     Filesh,0
                jc      short nflh
                add     ax,1
                adc     dx,0            ; 设文件长度+1. 考虑一些文件末尾
                                        ; 不一定以 1Ah 文件结束符结束,
                                        ; 那么此时应该正确读入最后一个字符
                Mov     Filesl,Ax
                Mov     Filesh,Dx
nflh:
                Mov     Ax,4200h
                Mov     Bx,Handle
                Xor     Cx,Cx
                Xor     Dx,Dx
                Int     21h             ; 移文件指针到文件首
                Mov     Cl,EditWide     ; CL=编辑屏幕宽度+1
                Inc     Cl
Again:          PuSh    Cx
                Mov     Ax,Filesh
                Or      Ax,Ax
                Jnz     Largef          ; 超过 64K 的文件, 转
                Mov     Ax,Filesl
                Jmp     SHORT Readok
Largef:
                Mov     Ax,0ffffh
Readok:
                Mov     Readsize,Ax
                PuSh    Ds
                Mov     Ah,3fh
                Mov     Bx,Handle
                Mov     Cx,Readsize
                Mov     Dx,Fileseg
                Mov     Ds,Dx
                Xor     Dx,Dx
                Int     21h                     ; 读入文件
                Pop     Ds
                Mov     Ax,Filesh
                Or      Ax,Ax
                Jnz     Mov_seg
                Mov     Si,Readsize
                or      si,si
                jz      short ndsi
                dec     si              ;mov     si,1
ndsi:
                Mov     Es,Fileseg
                Mov     BYTE PTR Es:[Si],1ah    ; 文件尾填入结束符-1
Mov_seg:
                Mov     Ax,Filesl
                Mov     Dx,Filesh
                Sub     Ax,Readsize
                Sbb     Dx,0
                Mov     Filesl,Ax
                Mov     Filesh,Dx
                Xor     Si,Si
                Pop     Cx
                Or      Cl,Cl
                Jnz     Mvmv
                Mov     Cl,EditWide
                Inc     Cl
Mvmv:           Cmp     Cl,EditWide             ; 宽度够,
                Jbe     Mov_begin
Mov_start:
                Inc     FileRowt                ; 则文件行加1
Mov_begin:
                Mov     Es,Fileseg
                Mov     Al,Es:[Si]
                Cmp     Filesh,0
                Ja      n_fend
                Cmp     Si,Readsize
                Jb      n_fend
                mov     al,1ah
                jmp     short not_1a
n_fend:
                Cmp     Al,1ah
                Jnz     Not_1a
;                Cmp     Filesh,0
;                Ja      Yes_1a
;                Cmp     Si,Readsize
;                Jae     Not_1a
;Yes_1a:
                Mov     Al,0FFh ;0dh    ; 若未满读入长度, 1Ah 转为 0FFh
Not_1a:
                Cmp     Al,0ah
                Jnz     Exp0a
                Cmp     Fal0a,15
                Jnz     Exp0a2
No0aok2:        Jmp     Mov_next
Exp0a2:         Cmp     Dcont,0
                Jz      No0aok2
Exp0a:          Cmp     Al,Tab
                Jz      Noexptab
                Jmp     Mov2
NoExpTab:       Push    Cx
                Mov     Al,EditWide
                Inc     Al
                Xor     Ah,Ah
                Xor     Ch,Ch
                Sub     Ax,Cx
                Call    Caltab          ; 计算 TAB 制表位置 -> AL
                Cmp     Al,EditWide
                Jbe     Stsp
                Mov     Al,EditWide
Stsp:           Mov     Cl,Al
                Mov     Al,EditWide
                Inc     Al
                Xor     Ah,Ah
                Xor     Ch,Ch
                Sub     Ax,Cx
                Pop     Cx
Stnsp:          Call    CmpAbsAdress
                jae     short A0000_ret
                Jmp     Stnsp1
A0000_ret:      Push_si
                Mov     Es,FileSeg
                Mov     WORD PTR Es:[0fffeh],0
                Push    Ax
                Dec     FileRowt
                Mov     Ax,FileRowt
                Dec     Ax
                Mov     FileRow,Ax
                Mov     Col,0
                Call    GotoLineEnd
                Mov     BYTE PTR Es:[Si],1ah
                Mov     Ax,FileRowt
                Cmp     Block,2
                Jnz     A_r1
                Cmp     Ax,Blocke
                Ja      A_r1
                Mov     Blocke,Ax
                Cmp     Ax,Blockh
                Ja      A_r1
                Mov     Blockh,Ax
A_r1:
                Cmp     Pian,2
                Jnz     A_r
                Cmp     Ax,Pianer
                Ja      A_r
                Mov     Pianer,Ax
                Cmp     Ax,Pianhr
                Ja      A_r
                Mov     Pianhr,Ax
A_r:            Pop     Ax
                Pop_si
                Ret
Stnsp1:
                Mov     Es,Diseg
                Mov     BYTE PTR Es:[Di],20h
                Call    Di_inc
                Dec     Cl
                Cmp     Cl,Al
                Jb      jmn     ;Mov_next
                Jmp     Stnsp
jmn:
                jmp     mov_next

Mov2:
                Call    CmpAbsAdress
                Jb      N1_A0000
                Jmp     A0000_ret
N1_A0000:
                Or      Al,Al
                Jnz     Zero_No
                Mov     Al,7fh          ; 0 转换为7FH
Zero_No:
                Mov     Es,Diseg
                Mov     Es:[Di],Al      ; 转换进编辑区
                Dec     Cl
                Cmp     Al,0dh
                Jnz     Cont1
                Cmp     Fal0a,0
                Jz      Exp0a1
                Inc     Dcont
                Cmp     Dcont,15
                Jb      No0dokok
                Mov     Dcont,0
                Jmp     SHORT Exp0a1
No0dokok:       Inc     Cl
                Jmp     SHORT Mov_next
Exp0a1:         Call    StorEsp
                Jmp     SHORT Mov_next
Cont1:
                Cmp     Al,1ah
                Jnz     Mov_next1
dsin:
                call    di_dec
                inc     cl
                Cmp     Cl,EditWide             ; 宽度够,
                ja      cffok
                mov     es,diseg
                cmp     byte ptr es:[di],0FFh
                jz      short dsin
cffok:
                call    di_inc
                dec     cl
                mov     es,diseg
                mov     byte ptr es:[di],01Ah

                Push    Es
                Mov     Es,FileSeg
                Mov     WORD PTR Es:[0fffeh],0
                Pop     Es
StorEsp:        Call    CmpAbsAdress
                Jb      N_A0000
                Jmp     A0000_ret
N_A0000:
                Call    Di_inc
                Or      Cl,Cl
                Jz      Stor
                Mov     Es,Diseg
                Mov     BYTE PTR Es:[Di],0
                Dec     Cl
                Jmp     SHORT Storesp
Stor:
                Retn

Mov_next1:      Call    Di_inc
Mov_next:
                Inc     Si
                Cmp     Si,Readsize
                Jb      Mov_next2
                Jmp     Again
Mov_next2:      Or      Cl,Cl
                Jnz     Mvvv
                Mov     Cl,EditWide
                Inc     Cl
                Jmp     Mov_start
Mvvv:           Jmp     Mov_begin

TUIN:
                Dec     Di
                Cmp     Di,0ffffh
                Jz      Tuiok2          ; 退到本段头, 结束返回
TUIN1:          Or      Di,Di
                Jz      Tuiok
                Dec     Di
                Mov     Dl,Es:[Di]
                Cmp     Dl,20h          ; 截掉回车前面的空格或TAB
                Jz      Tuin1
                Cmp     Dl,TAB
                Jz      Tuin1
TUIOK1:         Cmp     Al,1ah          ; 截掉文件结束符前的回车
                Jnz     Tuiok2
                cmp     dl,1Ah
                Jz      Tuin1
                Cmp     Dl,0dh
                Jz      Tuin1
                Cmp     Dl,0ah
                Jz      Tuin1
TUIOK2:         Inc     Di
Tuiok:          cmp     al,1ah
                jnz     tuiok_ture
                mov     word ptr es:[di],0a0dh
                add     di,2
tuiok_ture:
                Mov     Es:[Di],Al      ; 回退的最后一个字符
                Retn
