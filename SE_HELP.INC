; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_HELP.INC         ┃
; ┃ 版本号   : 2.1                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1992,1 - 92,5       ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved              ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括 SE 帮助功能文件
; --------------------------

;HELP_KW         db      'ASE',0

Help:
;                mov     ax,95fdh
;                int     10h
;                test    bl,20h
;                jz      short noachelp          ; ACHELP没有驻留,转
;                mov     ax,'关'
;                int     10h
;                mov     ax,9535h
;                mov     bx,0601h
;                mov     si,offset HELP_KW
;                int     10h
;                mov     ax,'开'
;                int     10h
;                retn
;noachelp:
                mov     byte ptr to_bjpm,1
                Mov     Cl,Oldmem               ; 针对 Ctrl_F7 引起的屏幕转换
                Call    SeeC
                Mov     Dx,0200h
                Mov     Si,OFFSET Help_Se
Nx_Help:        Push    Dx
                Call    Setpos
                Mov     Bl,lv
                Push    Si
                Call    Dispatr
                Pop     Si
                Pop     Dx
                Inc     Dh
                Add     Si,81
                Cmp     Dh,26
                Ja      Wan                     ; 帮助屏幕高度(26)显示完毕
                Cmp     Dh,EditHigh
                Jbe     Nx_Help                 ; 或者,一直显示到编辑屏幕高度
Wan:            Call    Setpos_dh
                Mov     Si,OFFSET Use_U&D       ; 显示提示对话
                mov     byte ptr if_color,0
                Call    Dispatr
                mov     byte ptr if_color,1
                Xor     Bp,Bp                   ; BP = 帮助屏幕顶行, 初始=0
Wt_K:           Call    Inkey
                Jnz     Esc_Help
                Cmp     Al,Cross_Down           ; 下移
                Jnz     If_C_U
                Mov     Al,26                   ; Al=帮助共有行数
                Sub     Al,EditHigh
                Jc      Wt_K
                Xor     Ah,Ah
                Cmp     Bp,Ax
                Jae     Wt_K
                Inc     Bp
                Mov     Ah,6                    ; 上滚一行帮助屏幕
                Call    Scroll
                Mov     Dh,EditHigh             ; Dx=最后一行座标
                Xor     Dl,Dl
                Mov     Ax,Bp
                Add     Al,EditHigh
                Dec     Al
                Dec     Al
                Jmp     SHORT Dp_Help
If_C_U:         Cmp     Al,Cross_Up             ; 上移
                Jnz     Wt_K
                Cmp     Bp,0
                Jz      Wt_K
                Dec     Bp
                Mov     Ah,7
                Call    Scroll
                Mov     Dx,0200h
                Mov     Ax,Bp
Dp_Help:
                Push    Dx
                Mov     Bx,81
                Mul     Bx
                Mov     Si,Ax
                Add     Si,OFFSET Help_Se       ; 计算 SI 指针
                Mov     Bl,lv                   ; BL=帮助屏幕颜色
                Pop     Dx
                Call    SetPos
                Call    Dispatr
                Jmp     SHORT Wt_K
Esc_Help:       Cmp     Al,Esca                 ; ESC 返回
                Jnz     Wt_K
                Call    Earse                   ; 清对话栏
                Mov     Editing,0
                Call    Show                    ; 刷新编辑屏幕
                Mov     Editing,1
                mov     byte ptr to_bjpm,0
                Ret
