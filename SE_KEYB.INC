; ������������������������������������
; �� �ļ���   : SE_KEYB.INC         ��
; �� �汾��   : 2.0                 ��
; �� ��  ��   : ��                ��
; �� ��  ��   : 1991 �� 12 ��       ��
; ��                                ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved in ShenZhen  ��
; ������������������������������������
;
; ���ļ��������ڼ��̲������ӳ���, �����¼���:
; ----------------------------------------------------
; Inkey         -  ����ʾ�ȴ�����һ���ַ�
; Input         -  ����һ���ַ�
; Dispatr&Inkey -  ��ʾһ�ִ�, �����������
; ----------------------------------------------------

INKEY:
; ---------------------------------
; ����: �ȴ���������һ���ַ�
; ����: ��
; ����: AL: ASCII ��
;       ZF: = 00h Ϊ��չ�� (JZ)
;           = 01h Ϊ���� ASCII �� (JNZ)
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

; ��� SHIFT ���Ƿ���
; ���� JNZ= ����
;      JZ = δ����
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
; ����: �ȴ�����һ���ַ� (��������չ��), �� ENTER �� ESC ����
;       �����볤��Ϊ��ʱ, ���뻺����ԭ�����ݲ����ƻ�
; ����: CL: �����ַ�����
;       DX: ��Ļ��ʾ��ʼλ��
;       SI: ������Ż�����
; ����: CH: ʵ�������ַ�����
;       SI: ָ�򻺳���β��, [SI]=ENTER
; -----------------------------------------------------------
                mov     byte ptr to_bjpm,1
                Xor     Ch,Ch
Npos:           Call    Set_bios_pos
                call    setpos
                Call    Inkey
                Jnz     Nkey1           ; ��չ��ȥ��
                Cmp     Al,Cross_Right
                Jnz     Nk
                Mov     Al,[Si]
                Or      Al,Al
                Jz      Npos
                Cmp     Al,0dh
                Jnz     Nkey
                Jz      Npos
Nk:             Cmp     Al,Cross_left   ; ֻ�����Ƽ�
                Jnz     Npos
                Jmp     SHORT Leftok
Nkey1:          Cmp     Al,Backspace    ; ����ɾ�� "<-"
                Jnz     Nkey
Leftok:         Or      Ch,Ch           ; �����������ֹͣ
                Jz      Npos
                Dec     Dl
                Call    Setpos
                Mov     Al,20h
                Mov     Bl,Menuc
                PuSh    Cx
                Call    Putc            ; back a space
                Pop     Cx
                Dec     Si              ; ָ���һ
                Dec     Ch              ; ������һ
                Inc     Cl              ; ������һ
                Jmp     SHORT Npos
Nkey:           Cmp     Al,1bh
                Jz      Inputr
                Cmp     Al,0dh
                Jz      Inputr
                Or      Cl,Cl
                jnz     short nobeep
                mov     ax,0e07h        ; ���ȹ�,������ʾ
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
                Dec     Cl              ; ָ�����ȼ�һ
                Inc     Dl              ; Col+1
                Inc     Ch              ; totle+1
                Inc     Si              ; si+1
                Jmp     SHORT Npos
Inputr:         Or      Ch,Ch           ; �����볤��Ϊ��, ���ƻ�ԭ������
                Jz      Retdir
                Mov     BYTE PTR [Si],Enter     ; Ch = ����, �������� 0 ,
Retdir:         Call    Earse
                mov     byte ptr to_bjpm,0
                Ret                     ; �ִ��Իس�����.

DISPATR&INKEY:
; ------------------------------------------------
; ����: ��ʾһ�ִ�, ���ȴ��������, Ȼ������Ի���
; ����: SI: ��ʾ�ִ����׵�ַ
; ����: AX: ���� INKEY �����ֵ
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
                Call    Input                   ; ��������(4λ)
                Cmp     Al,Esca
                mov     byte ptr if_color,1
                Ret
