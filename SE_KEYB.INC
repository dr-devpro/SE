; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_KEYB.INC         ┃
; ┃ 版本号   : 2.0                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1991 年 12 月       ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于键盘操作的子程序, 有以下几个:
; ----------------------------------------------------
; Inkey         -  无显示等待输入一个字符
; Input         -  输入一行字符
; Dispatr&Inkey -  显示一字串, 按任意键返回
; ----------------------------------------------------

INKEY:
; ---------------------------------
; 功能: 等待键盘输入一个字符
; 进入: 无
; 返回: AL: ASCII 码
;       ZF: = 00h 为扩展键 (JZ)
;           = 01h 为正常 ASCII 码 (JNZ)
; ---------------------------------

wait_key:
                int     28h
                mov     ah,11h
                int     16h
                jz      short wait_key
                mov     ah,10h
                int     16h
                or      ah,ah
                jz      noe0
                cmp     al,0e0h
                jnz     noe0
                xor     al,al
noe0:
                or      al,al
                jnz     short ascr
                xchg    al,ah
ascr:
                retn

; 检测 SHIFT 键是否按下
; 返回 JNZ= 按下
;      JZ = 未按下
test_shift:
                push    ds
                push    ax
                xor     ax,ax
                mov     ds,ax
                test    byte ptr ds:[417h],3
                pop     ax
                pop     ds
                retn

INPUT:
; -----------------------------------------------------------
; 功能: 等待输入一行字符 (不包括扩展键), 按 ENTER 或 ESC 返回
;       当输入长度为零时, 输入缓冲区原有数据不被破坏
; 进入: CL: 输入字符个数
;       DX: 屏幕显示起始位置
;       SI: 输入后存放缓冲区
; 返回: CH: 实际输入字符个数
;       SI: 指向缓冲区尾部, [SI]=ENTER
; -----------------------------------------------------------
                mov     byte ptr to_bjpm,1
                Xor     Ch,Ch
Npos:           Call    Set_bios_pos
                call    setpos
                Call    Inkey
                Jnz     Nkey1           ; 扩展键去掉
                Cmp     Al,Cross_Right
                Jnz     Nk
                Mov     Al,[Si]
                Or      Al,Al
                Jz      Npos
                Cmp     Al,0dh
                Jnz     Nkey
                Jz      Npos
Nk:             Cmp     Al,Cross_left   ; 只用左移键
                Jnz     Npos
                Jmp     SHORT Leftok
Nkey1:          Cmp     Al,Backspace    ; 回退删除 "<-"
                Jnz     Nkey
Leftok:         Or      Ch,Ch           ; 若到最左边则停止
                Jz      Npos
                Dec     Dl
                Call    Setpos
                Mov     Al,20h
                Mov     Bl,Menuc
                PuSh    Cx
                Call    Putc            ; back a space
                Pop     Cx
                Dec     Si              ; 指针减一
                Dec     Ch              ; 总数减一
                Inc     Cl              ; 计数加一
                Jmp     SHORT Npos
Nkey:           Cmp     Al,1bh
                Jz      Inputr
                Cmp     Al,0dh
                Jz      Inputr
                Or      Cl,Cl
                jnz     short nobeep
                mov     ax,0e07h        ; 长度够,响铃提示
                int     10h
                Jmp     short Npos
nobeep:
                Mov     Bl,Menuc
                And     Bl,0f0h
                Or      Bl,0ah
                Cmp     Al,1ah
                Jz      Npos
                Mov     [Si],Al
                PuSh    Cx
                Call    Putc            ; print the ascii chr
                Pop     Cx
                Dec     Cl              ; 指定长度减一
                Inc     Dl              ; Col+1
                Inc     Ch              ; totle+1
                Inc     Si              ; si+1
                Jmp     SHORT Npos
Inputr:         Or      Ch,Ch           ; 若输入长度为零, 则不破坏原有输入
                Jz      Retdir
                Mov     BYTE PTR [Si],Enter     ; Ch = 总数, 若不等于 0 ,
Retdir:         Call    Earse
                mov     byte ptr to_bjpm,0
                Ret                     ; 字串以回车结束.

DISPATR&INKEY:
; ------------------------------------------------
; 功能: 显示一字串, 并等待按任意键, 然后清除对话栏
; 进入: SI: 显示字串的首地址
; 返回: AX: 调用 INKEY 后的码值
; -------------------------------------------------
                mov     byte ptr if_color,0
                Call    Dispatr
                Call    Inkey
                Call    Earse
                mov     byte ptr if_color,1
                Ret

Input_Num:
                mov     byte ptr if_color,0
                Call    Setpos1_dh
                Mov     Si,OFFSET Gotobuf
                Mov     Cl,4
                Call    Input                   ; 输入新数(4位)
                Cmp     Al,Esca
                mov     byte ptr if_color,1
                Ret
