; File: SE_MTAB.INC
; =================
; ����ֶ��Ʊ�����, ������˫��. �� ALT �������� ����
; --------------------------------------------------


S&D_line        db      0       ; ��˫�߿���
Last_ZF         db      0
Last_FX         db      0       ; 1,2,3,4
ZF_tab          db      '����������'

PM_tab          db      '����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'

Up_tab          db      '  ��'
                db      '��  '
                db      ' � �'
                db      ' �� '
                db      '�  �'
                db      '� � '
                db      '�� �'
                db      '��� '
                db      '� ��'
                db      ' ���'
                db      '����'

Down_tab        db      '  ��'
                db      '��  '
                db      ' � �'
                db      ' �� '
                db      '�  �'
                db      '� � '
                db      '�� �'
                db      '��� '
                db      '� ��'
                db      ' ���'
                db      '����'

Left_tab        db      '  '
                db      '��  '
                db      ' � '
                db      ' � '
                db      '�  '
                db      '�  '
                db      '�� '
                db      '�� '
                db      '� '
                db      ' �'
                db      '��'

Right_tab       db      '  '
                db      '��  '
                db      ' � '
                db      ' � '
                db      '�  '
                db      '�  '
                db      '�� '
                db      '�� '
                db      '� '
                db      ' �'
                db      '��'

ZF_tab2         db      '����������'

PM_tab2         db      '����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'
                db      '�����������'

Up_tab2         db      '  ��'
                db      '��  '
                db      ' � �'
                db      ' �� '
                db      '�  �'
                db      '� � '
                db      '�� �'
                db      '��� '
                db      '� ��'
                db      ' ���'
                db      '����'

Down_tab2       db      '  ��'
                db      '��  '
                db      ' � �'
                db      ' �� '
                db      '�  �'
                db      '� � '
                db      '�� �'
                db      '��� '
                db      '� ��'
                db      ' ���'
                db      '����'

Left_tab2       db      '  '
                db      '��  '
                db      ' � '
                db      ' � '
                db      '�  '
                db      '�  '
                db      '�� '
                db      '�� '
                db      '� '
                db      ' �'
                db      '��'

Right_tab2      db      '  '
                db      '��  '
                db      ' � '
                db      ' � '
                db      '�  '
                db      '�  '
                db      '�� '
                db      '�� '
                db      '� '
                db      ' �'
                db      '��'

Ascok_call1:
                mov     cs:Last_ZF,al      ; �ϻ�ʹ���ַ�
                mov     cs:Last_FX,ah      ; �ϻؽ���ʱ����
                call    get_equ_di
                push    di
                Call    Calps1
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                call    get_equ_di
                pop     si
                mov     al,cs:Last_ZF
                cmp     cx,0ffffh
                jz      isnok
                mov     ax,si
                mov     bx,11
                mul     bx
                add     ax,di
                mov     si,ax
                mov     ax,offset PM_tab
                cmp     cs:S&D_line,0
                jz      short Sj1_ok
                mov     ax,offset PM_tab2
Sj1_ok:
                add     si,ax           ;offset PM_tab
                mov     al,cs:[si]
isnok:
                mov     cs:Last_ZF,al      ; �ϻ�ʹ�����յ��ַ�
                push    word ptr cs:Inswp
                mov     cs:Inswp,0
                call    Ascok_call
                pop     word ptr cs:Inswp
                ret

get_equ_di:
                mov     cx,11
                mov     bx,offset ZF_tab
                cmp     cs:S&D_line,0
                jz      short equnx1
                mov     bx,offset ZF_tab2
equnx1:
                mov     di,bx
equnx:
                cmp     al,cs:[di]
                jz      short equok
                inc     di
                loop    equnx
                mov     cx,0ffffh
equok:
                sub     di,bx           ;offset ZF_tab
                ret

PD_FX:
                push    cs
                pop     ds
                push    cs
                pop     es
                mov     al,cs:Last_ZF
                call    get_equ_di
                mov     ax,di
                shl     al,2
                add     si,ax
                mov     bl,cs:Last_FX
                dec     bl
                xor     bh,bh
                mov     al,cs:[si+bx]
                ret
