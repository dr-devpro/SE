; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_FIND.INC         ┃
; ┃ 版本号   : 2.0                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1991 年 12 月       ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于字串查找/替换的子程序, 有以下几个:
; ------------------------------------------------------
; Find&Replace  -  查找/替换的中心程序
; AnykeyRet     -  按任意键返回 (包含在 Find&Replace 中)
; Search        -  搜寻子程序
; ------------------------------------------------------

FIND&REPLACE:
                mov     byte ptr if_color,0
                Call    Calps1
                PuSh_si                 ; 当前指针入栈
                Call    Setpos_dh
                Mov     Si,OFFSET Find
                Call    Dispatr
                Mov     Si,OFFSET Findbuf
                Mov     Cl,30
                Call    Input           ; 输入查找字串
                Cmp     Al,Esca         ; ESC 键返回
                Jnz     Finr
Findr:          Pop_si
                mov     byte ptr if_color,1
                Jmp     Editbegin
Finr:           Or      Ch,Ch           ; 字串长度为零,则返回
                Jz      Findr
                Mov     Finded,Ch       ; 长度 -> Finded
                Mov     Si,OFFSET Replbuf
                Mov     BYTE PTR [Si],0dh
                Xor     Ch,Ch
                Cmp     CtrlQF,0
                Jnz     Repr            ; 若是 ^QF 查找, 则不输入查找字串
                Call    Setpos_dh
                Mov     Si,OFFSET Repl
                Call    Dispatr
                Mov     Si,OFFSET Replbuf
                Mov     Cl,30
                Call    Input           ; 输入替换字串
                Cmp     Al,Esca
                Jz      Findr
Repr:           Mov     RePled,Ch       ; 替换字串长度->RePled (为0则不替换)
                Call    Setpos_dh
                Mov     Si,OFFSET FanShi
                Call    Dispatr
                Mov     Si,OFFSET Fanbuf
                Mov     Cl,5
                Call    Input           ; 输入查找方式
                mov     byte ptr if_color,1
                Pop_di                  ; 弹出当前指针
                Cmp     Al,Esca         ; ESC 返回
                Jz      Findr1ok
                Mov     Uorl,0          ; 初始化, 预置忽略大小写,问答方式
                Mov     Ask,0           ; 非块内, 及第一次
                Mov     Inblck,0
                Mov     IfFind,0
                Mov     IfrePl,0
                Mov     Binval,0
                Or      Ch,Ch           ; 无输入则开始查找
                Jnz     Findr55
                Jmp     Sbegin
Findr1ok:       Jmp     Findr1
Findr55:        Mov     Si,OFFSET Fanbuf
Findr5:         Mov     Al,[Si]
                Cmp     Al,0dh
                Jnz     HaveFanShi
                Jmp     Sbegin
HaveFanShi:     Cmp     Al,'G'          ; 全程
                Jz      Bbe
                Cmp     Al,'g'
                Jnz     Sbegin1         ; 无 'G' 选项转
Bbe:            Mov     Di,Filesto      ; 置查找起始段为 FileSto
                Mov     Diseg,Di
                Xor     Di,Di
                Inc     Si
                Dec     Ch              ; 下一方式字
                Or      Ch,Ch
                Jz      Sbegin          ; 没有了,转开始搜寻
Sbegin1:        Mov     Al,[Si]
                Cmp     Al,'U'          ; 忽略大小写
                Jz      Nou_l
                Cmp     Al,'u'
                Jnz     Sbegin2         ; 无 'U' 选项转
Nou_l:          Mov     Uorl,1          ; 置不忽略
                Inc     Si
                Dec     Ch              ; 下一方式字
                Or      Ch,Ch
                Jz      Sbegin          ; 没有,则开始搜寻
Sbegin2:        Mov     Al,[Si]
                Cmp     Al,'N'          ; 自动/问答 'N'
                Jz      Noask
                Cmp     Al,'n'
                Jz      Noask
                Cmp     Al,'I'          ; 块内查找 'I'
                Jz      Inbok
                Cmp     Al,'i'
                Jnz     Segin
Inbok:          Cmp     Block,2
                Jnz     Nextchri
                Mov     Inblck,1
                Mov     Ax,Blockh
                Push    Si
                Xor     Si,Si
                Call    Calps           ; 从块首开始
                Mov     Di,Siseg
                Mov     Diseg,Di
                Mov     Di,Si
                Pop     Si
                Jmp     SHORT Nextchri
Segin:          Cmp     Al,'A'
                Ja      Nextchri
                Jmp     SHORT Sbegin22  ; 无字母选项转数字判断
Noask:          Mov     Ask,1           ; 置不问答
Nextchri:       Inc     Si
                Dec     Ch              ; 下一方式字
                Or      Ch,Ch
                Jz      Sbegin          ; 没有,则开始搜寻
                Jmp     Findr5          ; 查下一个方式字
Sbegin22:       Xchg    Ch,Cl
                Mov     Bx,Cx
                Xor     Bh,Bh
                Call    B10asb1         ; 计算输入之数字
                Dec     Binval
Sbegin:                                 ; ^L 入口处
                Call    Search          ; 搜寻一行
                Cmp     Al,0dh          ; 结束符为回车吗? 不是则已到文件尾
                Jnz     Findr4
                Cmp     Binval,0
                Jz      Findr2          ; 计数为零,满足要求
                Dec     Binval
                Call    Di_inc
                Jmp     SHORT Sbegin
Findr4:         Mov     Si,OFFSET Nofind
                Cmp     IfFind,0
                Jz      Anykeyret
                Cmp     IfrePl,0        ; 替换过才重新显示屏幕
                Jz      NorePlace
                Call    Show
NorePlace:      Mov     Si,OFFSET Eoffindok
                Mov     Dl,45
                Call    Setpos1_dh
                Jmp     SHORT Anykey1
Anykeyret:                              ; 按任意键返回进口
                Call    Setpos_dh
Anykey1:        Call    Dispatr&inkey   ; 按任意键返回
                Jmp     Editbegin
Findr2:
                Mov     IfFind,1        ; 置搜寻到标志
                Mov     Si,Diseg        ; 修改当前指针为刚找到处
                Mov     Siseg,Si
                Mov     Si,Di
                Cmp     RePled,0
                Jnz     Haverep1
                Cmp     CtrlQA,0        ; 若是 ^QA 替换, 则继续
                Jnz     Haverep1
                Jmp     Notaskok        ; 无替换字串转
Haverep1:       Cmp     Ask,1
                Jnz     Hav
                Jmp     Haverep         ; 自动替换转
Hav:            Push_si
                Call    Caline          ; 计算出在第几行
                Mov     FileRow,Ax
                call    midel_line
                Mov     WinRow,Ax       ; 修改窗口行及文件行
;                Mov     FileRow,Ax

;                Mov     Row,2
                mov     al,Cure_find_pos
                mov     Row,al

                Mov     Ax,Si
                Mov     Col,Al          ; 光标停在对应指针处
                Mov     Editing,0
                Call    Show            ; 显示该字串
                Mov     Si,OFFSET Findbuf
                Mov     Bl,Lv
                And     Bl,0fh          ; 屏闭高位
                Mov     Cl,4
                Shl     Bl,Cl           ; 左移四位
                Mov     Bh,Lv
                Shr     Bh,Cl
                Or      Bl,Bh           ; 交换 Lv 的左右各四位, 即前背景色互换
                Mov     Cl,Finded       ; 搜寻字串的长度
                Xor     Ch,Ch
                Mov     Dl,Col
;                Mov     Dh,2
                mov     dh,Cure_find_pos

Nrep:           Call    Setpos
                Mov     Al,[Si]
                Push    Cx
                Call    Putc
                Pop     Cx
                Inc     Dl
                Inc     Si
                Cmp     Dl,79
                Jbe     Nonextl
                Xor     Dl,Dl           ; 转下一行
                Inc     Dh
Nonextl:        Loop    Nrep
                Call    Setpos_dh
                Mov     Si,OFFSET Ifreplace
                Call    Dispatr&inkey   ; 用户选择
                Pop_si
                Jz      Endask
                Cmp     Al,'Y'          ; 要替换转
                Jz      Haverep
                Cmp     Al,'y'
                Jz      Haverep
                cmp     al,Esca         ; 按 ESC 返回
                jz      short Endask
                jmp     ctrllok         ; 否则,执行 Ctrl_L
Endask:         Jmp     Editbegin       ; 搜寻结束,返回
Haverep:        Push_si                 ; 保留原指针
                Mov     Di,OFFSET ReplBuf
                Mov     Cl,inswp
                PuSh    Cx              ; 保存 INSERT 状态
                Mov     Inswp,1         ; 置为插入
RePlnext:       Mov     Al,[Di]
                Cmp     Al,0dh
                Jz      EndofrePl       ; 遇插入字串的回车则结束
                Mov     Inschr,Al
                PuSh    Di
                PuSh_si
                Call    Insert_rep      ; 将替换字符逐一插入
                Pop_si
                Call    Si_inc
                Pop     Di
                Inc     Di
                Jmp     SHORT RePlnext
EndofrePl:      Pop     Cx              ; INSERT 出栈
                Mov     Inswp,Cl        ; 恢复
                Mov     Cl,Finded       ; 原字串长度
                Xor     Ch,Ch
Delrepnx:       PuSh    Cx
                PuSh_si                 ; 在原位置连续
                Call    Delete_rep      ; 删除原字串
                Pop_si
                Pop     Cx
                Loop    Delrepnx
                Pop_di                  ; 弹出原指针
                Call    Mov_di_to_si
                Inc     IfrePl          ; 替换计数加一
                Cmp     Ask,1
                Jz      Delre           ; 若自动方式, 则继续
                Jmp     SHORT Notaskok
Delre:          Mov     Cl,Repled
                Or      Cl,Cl           ; 替换词长度为0, 则不必跳
                Jz      No_Nextpos
Nextpos:        Call    Di_inc
                Loop    Nextpos         ; 指针跳过替换词
No_Nextpos:     Mov     Ax,IfrePl
                Call    Calcnum         ; 转换计数值
                Mov     Al,linec
                Mov     Attr,Al         ; 置颜色
                Mov     Si,OFFSET lines
                Mov     Bp,40
Seencont:       Mov     Dx,Bp
                Call    Setpos1_dh
                mov     byte ptr if_color,0
                Mov     Dl,[Si]
                Call    To_Scr          ; 显示计数
                Inc     Si
                Inc     Bp
                Cmp     Bp,43
                Jbe     Seencont
                Call    Setpos_dh
                Mov     Dl,'('
                Call    To_Scr
                Mov     Dl,2ch
                Call    Setpos1_dh
                Mov     Dl,')'
                Call    To_Scr
                Mov     Dl,2eh
                Call    Setpos1_dh
                Mov     Si,OFFSET Pleaswait
                Mov     Bl,Menuc
                Call    Dispatr
                mov     byte ptr if_color,1
                Jmp     Sbegin          ; 自动时继续下一个
Notaskok:
                Call    Caline          ; 计算出在第几行
Gotolinenum:
                Mov     FileRow,Ax
                call    midel_line
                Mov     WinRow,Ax       ; 修改窗口行及文件行
;                Mov     FileRow,Ax

;                Mov     Row,2
                mov     al,Cure_find_pos
                mov     Row,al

                Mov     Ax,Si
                Mov     Col,Al          ; 光标停在对应指针处
                Mov     Editing,0
                Call    Show
                cmp     CtrlQA,0        ; 仅搜索时, 转返回
                jz      short Findr1
;                cmp     ask,1
;                jz      Findr1          ; 自动替换时, 返回
                jmp     ctrllok         ; 否则执行 Ctrl_L
Findr1:
                mov     byte ptr if_color,1
                Jmp     Editbegin       ; 搜寻结束,返回

;-----------
; 搜寻子程序
;-----------
SEARCH:
                Mov     Si,OFFSET Findbuf
SE2:
;
                Push    Si
                Call    Mov_di_to_si
                Call    Caline
                Pop     Si
                Cmp     Ax,FileRowt
                Jb      See2            ; 超出文件底行, 则结束
                Mov     Al,1ah
                Jmp     Searchr
;
See2:           Mov     Es,Diseg
                Mov     Al,Es:[Di]      ; 取出目标字
                Cmp     Block,2
                Jnz     ContCmp
                Cmp     Inblck,0        ; 是块内搜寻吗? 非零是
                Jz      ContCmp
                Push    Ax
                Push    Si
                Call    Mov_di_to_si
                Call    Caline          ; 是, 计算当前行 -> AX
                Pop     Si
                Pop     Bx
                Cmp     Ax,Blockh
                Jb      ZeroNcmp        ; 在块首前, 不比较
                Cmp     Ax,Blocke
                Mov     Ax,Bx
                Jbe     ContCmp         ; 已到块尾, 则返回
                Mov     Al,1ah
ContCmp:        Cmp     Al,1ah
                Jz      Searchr
                Or      Al,Al           ; 取到零, 下一个
                Jz      ZeroNcmp
                Call    Secomp
                Jz      Onok            ; 先找到第一个相等的字符
ZeroNcmp:       Call    Di_inc          ; 不等,目标加一
                Jmp     SHORT Se2
ONOK:           PuSh_di
ONOK1:          Mov     Al,[Si+1]
                Cmp     Al,0dh          ; 字串找到(比较到结束), 返回 AL=0DH
                Jz      Searchok
                Call    Di_inc          ; 目标加一
                Inc     Si              ; 源加一
                Mov     Es,Diseg
                Mov     Al,Es:[Di]
                Cmp     Al,1ah
                Jz      Searchok
                Call    Secomp
                Jz      Onok1
                Pop_di
                Call    Di_inc
                Jmp     Search
SEARCHOK:       Pop_di
SEARCHR:        Ret
SECOMP:         Call    Lowcase         ; 统一小写
                Mov     Ah,Al
                Mov     Al,[Si]         ; 取出输入字
                Call    Lowcase
                Cmp     Al,Ah           ; 两者比较
                Ret

Cure_find_pos   db      0

midel_line:
                mov     Cure_find_pos,2
                mov     dl,EditHigh
                shr     dl,1
                xor     dh,dh
ml_r1:
                cmp     ax,dx
                jae     short ml_r
                dec     dl
                jmp     short ml_r1
ml_r:
                add     Cure_find_pos,dl
                sub     ax,dx
                retn
