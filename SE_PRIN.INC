; ������������������������������������
; �� �ļ���   : SE_PRIN.INC         ��
; �� �汾��   : 2.1                 ��
; �� ��  ��   : ��                ��
; �� ��  ��   : 1991,12 - 92,4      ��
; ��                                ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved              ��
; ������������������������������������

Print_File:
                mov     no_P48,0
                Call    PageLayout
                cmp     no_P48,0
                jnz     short notps
                call    if_to_spt
notps:
                Call    SetPos_dh
                Mov     Si,OFFSET PleaseReady
                Call    Dispatr&Inkey
                Cmp     Al,27
                Jnz     Print_Ov
                Jmp     ESC_Over        ; �� ESC ����
Print_Ov:
                Call    Setpos_Dh
                Mov     Si,OFFSET ESCret
                mov     byte ptr if_color,0
                Call    DispAtr
                mov     byte ptr if_color,1
                mov     ax,9517h
                mov     bx,06f0h
                int     10h
Q2_Next:
                Mov     Col,0
                Call    Calps1
Q1_Next:
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Call    Si_inc
                inc     col
                Cmp     Al,0dh          ; �޳��س����� 0
                Jz      CrLfok
                Cmp     Al,0ah
                Jz      CrLfok
                cmp     al,0FFh
                jz      short nff1a
                Cmp     Al,7fh
                Jnz     Nop@
nff1a:
                mov     Al,20h
Nop@:
                Or      Al,Al
                Jz      Q3_Next         ; ���� 0
                cmp     al,1ah
                jnz     short Yesp@
                Mov     dx,FileRow
                Cmp     dx,FileRowt     ; ��ӡ����
                jae     short One_End
                jmp     short Q3_Next
Yesp@:
                Call    PrintChr        ; ��ӡһ�ַ� AL
Q3_Next:
                mov     al,col
                cmp     al,editwide
                ja      short incpl
                Jmp     SHORT Q1_Next
;-----------------------------------------
CrLfok:
                Call    Cr&Lf
                Call    PressESC        ; ESC ����
                jz      short one_end
incpl:
                Inc     FileRow         ; �Ѵ�ӡ��������
                Mov     Ax,FileRow
                Cmp     Ax,FileRowt     ; ��ӡ����
                jb      short Q2_Next
One_End:
                Mov     Al,0Ch
                Call    PrintChr        ; ��ҳ
                mov     ax,9517h
                mov     bh,4
                int     10h
                or      al,al
                jz      short ESC_over  ;al0c
                mov     ax,0e07h
                int     10h
                call    inkey
                mov     ax,9514h
                mov     bh,4
                int     10h             ; �ָ���Ļ
ESC_over:
                Call    Earse
                call    spt_over
                Call    SetPos_dh
                Mov     Si,OFFSET PrintOver
                Call    Dispatr&Inkey
                mov     ax,9517h
                mov     bx,060fh
                int     10h
                Retn

PrintChr:
                Xor     Ah,Ah
                Xor     Dx,Dx
                Int     17h
                Retn

Cr&Lf:
                Mov     Al,0dh
                Call    PrintChr
                Mov     Al,0ah
                Call    PrintChr
                Retn

PressESC:
                Mov     Ah,1
                Xor     Al,Al
                Int     16h
                Jz      NoKp
                Xor     Ah,Ah
                Int     16h
Nokp:
                Cmp     Al,27d
Escrt:
                Retn

if_to_spt:
                Call    Setpos_Dh
                mov     si,offset if_SPTfile
                Call    Dispatr&inkey
                and     al,0DFh
                cmp     al,'Y'
                jz      short if_to_spt1
                retn
if_to_spt1:
                Call    Setpos_Dh
                mov     si,offset SPT_file
                call    inputname       ; ����SPT�ļ���
                or      ch,ch
                jnz     short spt_yes   ; ֱ�Ӱ��س�, ����ֱ�Ӵ�ӡ
                retn
spt_yes:
                Mov     BYTE PTR [Si],0 ; �ļ����� 0 ����
                xor     cx,cx
                Mov     Ah,5Bh
                Mov     Dx,OFFSET PianFileName
                Int     21h             ; �����ļ�
                jnc     short oksptfile
                cmp     al,50h
                jz      short fileexist ; �ļ��Ѵ���, ת
                Call    Geterrmsg
                Call    Setpos_dh
                Call    Dispatr&inkey   ; �����������
                jmp     short if_to_spt1
fileexist:
                mov     ax,0e07h
                int     10h
                Call    Setpos_dh
                mov     si,offset spt_er1
                Call    Dispatr&inkey
                and     al,0DFh
                cmp     al,'Y'
                jnz     short if_to_spt1
                xor     cx,cx
                xor     cx,cx
                Mov     Ah,3Ch
                Mov     Dx,OFFSET PianFileName
                Int     21h             ; �����ļ�
oksptfile:
                mov     word ptr spt_wide,0
                mov     word ptr spt_high,0
                mov     word ptr cure_top,0
                mov     Handlepian,ax
                mov     bx,ax
                mov     cx,64d
                mov     dx,offset SPT_head
                mov     ah,40h          ; ��д��SPT�ļ�ͷ����
                int     21h             ; (Ԥ��64���ֽڵ��ļ�ͷ)
                mov     dx,offset spt_entry
                mov     ax,9535h
                mov     bh,3
                int     10h             ; �Ǽǰ������ݴ��������ַ
                mov     if_spt,1
                retn

spt_over:
                cmp     if_spt,0
                jz      short no_spt
                mov     bx,handlepian
                xor     cx,cx
                xor     dx,dx
                mov     ax,4200h
                int     21h             ; ָ�������ļ�ͷ
                mov     bx,handlepian
                mov     cx,64d
                mov     dx,offset SPT_head
                mov     ah,40h
                int     21h             ; д������SPT�ļ�ͷ����
                mov     bx,handlepian
                mov     ah,3eh
                int     21h
                mov     ax,9535h
                mov     bh,3
                xor     dx,dx
                mov     ds,dx
                int     10h             ; ȡ�����洦������
                push    cs
                pop     ds
                mov     if_spt,0
no_spt:
                retn

Spt_entry:
                cmp     ax,9517h
                jz      short ok17h
                iret
ok17h:
                cmp     bh,4
                jnz     short no17_4
                mov     al,1
                iret
no17_4:
                cmp     bh,1
                jnz     short no17_1
                pusha
                shl     cx,3            ; �����ֽ�תΪ���ȵ�
                mov     cs:spt_wide,cx
                add     cs:spt_high,dx
                mov     ax,dx
                add     cx,7
                shr     cx,3
                mul     cx
                mov     cx,ax
                mov     dx,si
                push    cx
xncx:
                xor     byte ptr ds:[si],0FFh   ; ����������(SPTҪ��)
                inc     si
                loop    xncx
                pop     cx
                mov     bx,cs:handlepian
                mov     ah,40h
                int     21h             ; ����д���ļ�
                popa
iret17:
                iret

no17_1:
                cmp     bh,2
                jnz     short iret17
                push    ds
                pusha
                push    cs
                pop     ds
                add     cure_top,cx
                mov     ax,cure_top
                cmp     ax,spt_high
                jbe     short paret
                sub     ax,spt_high
                add     spt_high,ax
                mov     cx,ax           ; ����������
                or      cx,cx
                jz      short paret
                push    cx
                push    di
                mov     cx,dx
                mov     al,0FFh
                cld
                repz    stosb
                pop     di
                pop     cx
                push    es
                pop     ds
nzisp:
                push    cx
                push    dx
                mov     cx,dx           ; ÿ�п����ֽ�
                mov     bx,cs:handlepian
                mov     dx,di
                mov     ah,40h
                int     21h
                pop     dx
                pop     cx
                loop    nzisp
paret:
                popa
                pop     ds
                iret

PageLayout:
                mov     ax,95fdh
                int     10h
                cmp     ax,95fdh
                jz      short nop48
                cmp     ax,400h
                jb      short nop48
                test    bl,00000001b
                jnz     short yes_acp48
nop48:
                mov     no_p48,1
                mov     ax,0e07h
                int     10h
                Call    Setpos_Dh
                mov     si,offset no_acp48
                call    dispatr&inkey
yes_acp48:
                retn
