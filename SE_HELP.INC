; ������������������������������������
; �� �ļ���   : SE_HELP.INC         ��
; �� �汾��   : 2.1                 ��
; �� ��  ��   : ��                ��
; �� ��  ��   : 1992,1 - 92,5       ��
; ��                                ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved              ��
; ������������������������������������
;
; ���ļ����� SE ���������ļ�
; --------------------------

;HELP_KW         db      'ASE',0

Help:
;                mov     ax,95fdh
;                int     10h
;                test    bl,20h
;                jz      short noachelp          ; ACHELPû��פ��,ת
;                mov     ax,'��'
;                int     10h
;                mov     ax,9535h
;                mov     bx,0601h
;                mov     si,offset HELP_KW
;                int     10h
;                mov     ax,'��'
;                int     10h
;                retn
;noachelp:
                mov     byte ptr to_bjpm,1
                Mov     Cl,Oldmem               ; ��� Ctrl_F7 �������Ļת��
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
                Ja      Wan                     ; ������Ļ�߶�(26)��ʾ���
                Cmp     Dh,EditHigh
                Jbe     Nx_Help                 ; ����,һֱ��ʾ���༭��Ļ�߶�
Wan:            Call    Setpos_dh
                Mov     Si,OFFSET Use_U&D       ; ��ʾ��ʾ�Ի�
                mov     byte ptr if_color,0
                Call    Dispatr
                mov     byte ptr if_color,1
                Xor     Bp,Bp                   ; BP = ������Ļ����, ��ʼ=0
Wt_K:           Call    Inkey
                Jnz     Esc_Help
                Cmp     Al,Cross_Down           ; ����
                Jnz     If_C_U
                Mov     Al,26                   ; Al=������������
                Sub     Al,EditHigh
                Jc      Wt_K
                Xor     Ah,Ah
                Cmp     Bp,Ax
                Jae     Wt_K
                Inc     Bp
                Mov     Ah,6                    ; �Ϲ�һ�а�����Ļ
                Call    Scroll
                Mov     Dh,EditHigh             ; Dx=���һ������
                Xor     Dl,Dl
                Mov     Ax,Bp
                Add     Al,EditHigh
                Dec     Al
                Dec     Al
                Jmp     SHORT Dp_Help
If_C_U:         Cmp     Al,Cross_Up             ; ����
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
                Add     Si,OFFSET Help_Se       ; ���� SI ָ��
                Mov     Bl,lv                   ; BL=������Ļ��ɫ
                Pop     Dx
                Call    SetPos
                Call    Dispatr
                Jmp     SHORT Wt_K
Esc_Help:       Cmp     Al,Esca                 ; ESC ����
                Jnz     Wt_K
                Call    Earse                   ; ��Ի���
                Mov     Editing,0
                Call    Show                    ; ˢ�±༭��Ļ
                Mov     Editing,1
                mov     byte ptr to_bjpm,0
                Ret
