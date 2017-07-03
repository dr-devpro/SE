; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_CROS.INC         ┃
; ┃ 版本号   : 2.0                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1991 年 12 月       ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于光标操作的子程序, 有以下几个:
; ---------------------------------------------------
; SetPos     -  置光标位置到 DH 行, DL 列
; SetPos_dh  -  置对话栏光标位置
; ---------------------------------------------------

set_bios_pos:
                Mov     Bh,Mode
                mov     ah,2
                int     10h
                ret

SETPOS:
; -----------------
; 功能: 置光标位置
; 进入: DL: 光标列
;       DH: 光标行
; 返回: 无
; ----------------
                cmp     byte ptr d_video,0
                jnz     short sp_d
                Mov     Bh,Mode         ; DL = 列
                Mov     Ah,2            ; DH = 行
                Int     10h
                retn
sp_d:
                push    ax
                push    bx
                push    dx
                mov     al,screenwide
                inc     al
                xor     ah,ah
                mov     bx,dx
                xor     bh,bh           ; BX = DL
                mov     dl,dh
                xor     dh,dh           ; DX = DH
                mul     dx
                add     ax,bx           ; AX = 80D * DH + DL
                shl     ax,1
                mov     vbuf_offset,ax
                pop     dx
                pop     bx
                pop     ax
                Ret

SETPOS_DH:
; --------------------------------------------------------
; 功能: 对话信息显示时, 先置光标位置 (第24行, 第39列)
; 进入: 当列数不在39列时, DL=要求列数, 然后调用SETPOS1_DH
; 返回: BL: 菜单颜色 Menuc
; --------------------------------------------------------
                Mov     Dl,39
SETPOS1_DH:     Mov     Dh,EditHigh
                Inc     Dh
                Call    Setpos
                Mov     Bl,Menuc
                Ret
