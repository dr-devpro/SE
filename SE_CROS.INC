; ������������������������������������
; �� �ļ���   : SE_CROS.INC         ��
; �� �汾��   : 2.0                 ��
; �� ��  ��   : ��                ��
; �� ��  ��   : 1991 �� 12 ��       ��
; ��                                ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved in ShenZhen  ��
; ������������������������������������
;
; ���ļ��������ڹ��������ӳ���, �����¼���:
; ---------------------------------------------------
; SetPos     -  �ù��λ�õ� DH ��, DL ��
; SetPos_dh  -  �öԻ������λ��
; ---------------------------------------------------

set_bios_pos:
                Mov     Bh,Mode
                mov     ah,2
                int     10h
                ret

SETPOS:
; -----------------
; ����: �ù��λ��
; ����: DL: �����
;       DH: �����
; ����: ��
; ----------------
                cmp     byte ptr d_video,0
                jnz     short sp_d
                Mov     Bh,Mode         ; DL = ��
                Mov     Ah,2            ; DH = ��
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
; ����: �Ի���Ϣ��ʾʱ, ���ù��λ�� (��24��, ��39��)
; ����: ����������39��ʱ, DL=Ҫ������, Ȼ�����SETPOS1_DH
; ����: BL: �˵���ɫ Menuc
; --------------------------------------------------------
                Mov     Dl,39
SETPOS1_DH:     Mov     Dh,EditHigh
                Inc     Dh
                Call    Setpos
                Mov     Bl,Menuc
                Ret
