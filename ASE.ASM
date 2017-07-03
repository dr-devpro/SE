; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : ASE.ASM             ┃
; ┃ 版本号   : 1.x                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1991,2-92,4-93,3    ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved  JJ          ┃
; ┣━━━━━━━━━━━━━━━━┫
; ┃  编译方法:                     ┃
; ┃  C>MASM SE; ←┘               ┃
; ┃  C>LINK SE; ←┘               ┃
; ┃  C>EXE2BIN SE.EXE SE.COM ←┘  ┃
; ┃  C>DEL SE.EXE ←┘             ┃
; ┗━━━━━━━━━━━━━━━━┛
.286p

                INCLUDE Se_Macr.inc             ; 宏文件

CODE            SEGMENT
                ASSUME CS:CODE,DS:CODE,SS:CODE,ES:CODE

                ORG     100h                    ; 建立 COM 文件

Begin:          Jmp     Start

                INCLUDE Se_Data.inc             ; 数据文件
                INCLUDE Se_Proc.inc             ; 子程序文件
Start:
                push    ds
                xor     dx,dx
                mov     ds,dx
                mov     dl,ds:[484h]            ; 取系统当前屏幕行
                sub     dl,2
                pop     ds
                mov     edithigh,dl
                Call    Open_Se_Cfg             ; 取配置
                Mov     Ah,0fh
                Int     10h
                Mov     Mode,Bh                 ; 取当前显示页 -> Mode
                cmp     al,7
                jnz     short no_hgc
                mov     VBUF,0b000h             ; HGC 单显
                mov     lv,70h
no_hgc:
                mov     al,bh
                mov     bx,100h
                mul     bx
                add     VBUF,ax                 ; 指向当前页面地址
                push    ds
                xor     dx,dx
                mov     ds,dx
                mov     dx,ds:[44ah]
                pop     ds
                dec     dl
                mov     screenwide,dl           ; 当前屏幕列数-1
                Call    Cls
                Mov     Cl,Es:[80h]
                Or      Cl,Cl
                Jz      Ov1                     ; 无 PSP, 转提示退出
                Jmp     Okpsp

set_chinese:
                mov     ax,9502h
                xor     bh,bh
                int     10h                     ; 取消整字识别
                mov     ax,95fdh
                int     10h
                cmp     al,0FDh
                jz      short dv1               ; 无中国龙系统, 转
                mov     ax,95feh
                int     10h
                cmp     bh,2
                jae     short biosok
dv1:
                mov     byte ptr d_video,1
biosok:
                retn

Ov1:
                Mov     Si,OFFSET Menu1         ; 显示版权
                Mov     Bl,Menuc
                Call    Dispatr
                Mov     Dx,0200h
                Call    Setpos
                Mov     Si,OFFSET Nopsp         ; 提示用法
Over:           Mov     Bl,0ch
                Call    Dispatr
                Jmp     SHORT Exitnocro         ; 转退出

Quit:                                           ; Alt_X 正常退出入口
                Cmp     Chang,0
                Jnz     Ov2                     ; 修改过转
Dlf1:           Mov     Ah,3eh                  ; 未修改过, 关闭文件
                Mov     Bx,Handle
                Int     21h
                Cmp     Newf,0
                Jz      Qk                      ; 不是新文件, 转直接退出
                Mov     Ah,41h                  ; 是新文件, 同时未修改过
                Mov     Dx,OFFSET FilenaMe
                Int     21h                     ; 则删除之
                Jmp     SHORT Qk
Ov2:
                Call    Setpos_dh
                Mov     Si,OFFSET Saveif
                Call    Dispatr&inkey           ; 选择是否存盘?
                Jz      Noext
                Call    LowCase1
                Cmp     Al,'y'                  ; 'Y' 键存盘
                Jz      SaveFile1
                Cmp     Al,'n'                  ; 'N' 键, 需判断是否新文件
                Jz      Dlf1
Noext:          Jmp     Editbegin               ; 其他键, 返回编辑
SaveFile1:
                Call    Sok
                Mov     Newf,0
                Jmp     SHORT Dlf1
Qk:
                Call    Save_Se_Cfg             ; 存配置文件
                Mov     inswp,1
                call    shins
                Call    Cls
Exitnocro:
                Mov     Ax,4c00h                ; 正常退出
                Int     21h

Okpsp:
                push    cx
                mov     ax,1003h
                xor     bl,bl
                int     10h
;                Mov     Al,0f3h                 ; 高速键盘
;                Out     60h,Al
;                Mov     Cx,0ffffh
;KEYDELAY:       Loop    Keydelay
;                Xor     Al,Al
;                Out     60h,Al
                pop     cx
                Mov     Si,80h
Nxtspace:
                Inc     Si
                Cmp     BYTE PTR [Si],32
                Jnz     Nospace1
                Dec     Cl
                Jmp     SHORT Nxtspace          ; 跳过空格符
Nospace1:
                Cmp     BYTE PTR [Si],'/'
                Jz      Nx_Set
                Jmp     short N_Spc
Nx_Set:
                Dec     Cl                      ; '/' 后的参数判别
                Inc     Si
                Mov     Al,[Si]
                Call    LowCase1
                Cmp     Al,'w'
                Jz      Set_Wide                ; 参数"W"设定编辑宽度
                jmp     n_spc

Set_Wide:
                Dec     Cl
                Mov     Ch,3
                Mov     Di,OFFSET GotoBuf       ; 宽度值移到 GotoBuf
                Inc     Si
NxHiS_W:        Mov     Al,[Si]
                Cmp     Al,'0'
                Jb      No___1
                Cmp     Al,'9'
                Ja      No___1
                Mov     [Di],Al
                Dec     Cl
                Inc     Di
                Inc     Si
                Dec     Ch
                Jnz     NxHiS_W
No___1:         Mov     Al,3
                Sub     Al,Ch
                Jz      No___2
                Mov     Ch,Al
                Push    Si
                Push    Cx
                Call    Get_Num                 ; 计算出 GotoBuf -> Ax
                Pop     Cx                      ; 得宽度 (80-255)
                Pop     Si
                Cmp     Ax,80
                Jb      No___2
                Cmp     Ax,255
                Ja      No___2
                Dec     Al
                Mov     EditWide,Al
No___2:         Inc     Cl
                Dec     Si
                Jmp     short Nxtspace  ;Nx_Set
N_spc:
                Cmp     BYTE PTR [Si],32
                Jz      Np1
                Mov     Al,FalOk
                Cmp     Al,[Si]                 ; 列回车
                Jnz     Okpsp1
                Mov     Fal0a,1
Np1:            Inc     Si
                Dec     Cl
                Jmp     SHORT N_spc
Okpsp1:         Or      Cl,Cl
                Jnz     NojH
                Jmp     Ov1
NojH:           Cmp     Cl,31                   ; PSP 剩余长度不得超出32
                Jbe     Okname                  ; (方便在对话栏显示)
                Mov     Dx,0200h
                Call    Setpos
                Mov     Si,OFFSET Toolong
                Jmp     Over
Okname:
                Mov     Di,OFFSET Filename
                Push    Di
Nextpsp:        Mov     Al,[Si]
                Cmp     Al,20h
                Jz      Nmov
                Cmp     Al,0dh
                Jnz     Okeent
                Xor     Al,Al
Okeent:         Mov     [Di],Al         ; 转换文件名到 Filename
                Inc     Di
Nmov:           Inc     Si
                Dec     Cl
                Jnz     Nextpsp
                Pop     Si
                Cmp     BYTE PTR [SI],32        ; 文件名第一个字符
                Jae     Filenok
                Jmp     Ov1             ; 若非字母, 转错误退出
Filenok:
; -------
                call    set_chinese
; -------
                Call    OpenFile        ; 打开并转换文件进编辑区
                Call    Window          ; 显示屏幕窗口
                Mov     Ax,Filesto
                Mov     Es,Ax
                Mov     Siseg,Ax
                Mov     Diseg,Ax
                Call    CalCircle       ; 计算可使用空间比例
                Mov     Row,2           ; 置光标位置初试化
                Mov     Col,0
Shed_old:
                Call    Sold            ; 刷新编辑屏幕
Editbegin:
                Cmp     Warning,0       ; 编辑开始入口
                Jz      Noinswp0
                Mov     Inswp,0         ; 若有内存不够警告, 则强制插入关
                Call    Shins           ; 显示插入、缩进开关状态
Noinswp0:       Mov     Crif,0
                Call    Seeline         ; 显示行、列号及比例值
                Call    Calps1
                Mov     Es,Siseg
                Mov     Ah,Es:[Si]
                Or      Ah,Ah
                Jnz     Noind
                Push_si
Ncur:           Mov     Es,Siseg        ; 光标前的 0 全置为空列
                Mov     Dl,Es:[Si]      ; (保证光标的全屏幕游动)
                Or      Dl,Dl
                Jnz     CuRok
                Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],20h
                Call    Si_dec
                Jmp     SHORT Ncur
CuRok:          Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],20h
                Pop_si
                Mov     Es,Siseg
                Mov     Es:[Si],Dl      ; 行末回车或文件结束符
Noind:          Call    CorectCol       ; 置正确汉字光标位 Col
                Mov     Dl,Col
                Mov     Dh,Row
                Cmp     Dl,Left_Mark
                Jb      Need_Show1      ; 显示左半屏幕 (右滚)
                Mov     Al,Left_Mark
                add     al,screenwide
                Cmp     Dl,Al
                Ja      Need_Show       ; 显示右半屏幕 (左滚)
                Jmp     SHORT Po_ok
Need_Show1:     Push    Dx
                Cmp     Dl,20
                Jbe     ZrDl
                Sub     Dl,20
                Jmp     SHORT OkDl1
ZrDl:           Xor     Dl,Dl
OkDl1:          Mov     Left_Mark,Dl
                Call    Show
                Pop     Dx
                Jmp     SHORT Po_ok
Need_Show:      Push    Dx
                Sub     Dl,20
                Mov     Al,Dl
                add     al,screenwide
                Jc      Ns1_ok
                Cmp     Al,EditWide
                Jb      NS_ok
Ns1_ok:         Mov     Dl,EditWide
                sub     dl,screenwide
NS_ok:          Mov     Left_Mark,Dl
                Call    Show
                Pop     Dx
Po_ok:          Sub     Dl,Left_Mark    ; 光标位置要减去左边界
                pusha
                cmp     iffast,0
                jnz     bjok
                call    cmpcurasc
                mov     ax,'长'
                jae     qjok
bjok:
                mov     ax,'短'
qjok:
                int     10h
                popa
                Call    Set_bios_pos          ; 置编辑光标位
                Mov     IfChr1,0
                Call    Inkey           ; 接收一字符
                pushf
                push    ax
                mov     ax,'短'
                int     10h
                pop     ax
                popf
                Jnz     IsWords
                Jmp     Ctrl_key        ; 转扩展功能键
IsWords:        Jmp     Words           ; 转控制功能键

GoNs&Ed:
                Call    Newsh           ; 各功能返回后,刷新显示,转编辑开始
                Jmp     Editbegin

User_Buffer:

CODE            ENDS
                END     Begin

;
; ┌─────────────┐
; │    Eof: End of program   │
; └─────────────┘
;
