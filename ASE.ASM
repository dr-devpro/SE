; ������������������������������������
; �� �ļ���   : ASE.ASM             ��
; �� �汾��   : 1.x                 ��
; �� ��  ��   : ��                ��
; �� ��  ��   : 1991,2-92,4-93,3    ��
; ��                                ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved  JJ          ��
; �ǩ���������������������������������
; ��  ���뷽��:                     ��
; ��  C>MASM SE; ����               ��
; ��  C>LINK SE; ����               ��
; ��  C>EXE2BIN SE.EXE SE.COM ����  ��
; ��  C>DEL SE.EXE ����             ��
; ������������������������������������
.286p

                INCLUDE Se_Macr.inc             ; ���ļ�

CODE            SEGMENT
                ASSUME CS:CODE,DS:CODE,SS:CODE,ES:CODE

                ORG     100h                    ; ���� COM �ļ�

Begin:          Jmp     Start

                INCLUDE Se_Data.inc             ; �����ļ�
                INCLUDE Se_Proc.inc             ; �ӳ����ļ�
Start:
                push    ds
                xor     dx,dx
                mov     ds,dx
                mov     dl,ds:[484h]            ; ȡϵͳ��ǰ��Ļ��
                sub     dl,2
                pop     ds
                mov     edithigh,dl
                Call    Open_Se_Cfg             ; ȡ����
                Mov     Ah,0fh
                Int     10h
                Mov     Mode,Bh                 ; ȡ��ǰ��ʾҳ -> Mode
                cmp     al,7
                jnz     short no_hgc
                mov     VBUF,0b000h             ; HGC ����
                mov     lv,70h
no_hgc:
                mov     al,bh
                mov     bx,100h
                mul     bx
                add     VBUF,ax                 ; ָ��ǰҳ���ַ
                push    ds
                xor     dx,dx
                mov     ds,dx
                mov     dx,ds:[44ah]
                pop     ds
                dec     dl
                mov     screenwide,dl           ; ��ǰ��Ļ����-1
                Call    Cls
                Mov     Cl,Es:[80h]
                Or      Cl,Cl
                Jz      Ov1                     ; �� PSP, ת��ʾ�˳�
                Jmp     Okpsp

set_chinese:
                mov     ax,9502h
                xor     bh,bh
                int     10h                     ; ȡ������ʶ��
                mov     ax,95fdh
                int     10h
                cmp     al,0FDh
                jz      short dv1               ; ���й���ϵͳ, ת
                mov     ax,95feh
                int     10h
                cmp     bh,2
                jae     short biosok
dv1:
                mov     byte ptr d_video,1
biosok:
                retn

Ov1:
                Mov     Si,OFFSET Menu1         ; ��ʾ��Ȩ
                Mov     Bl,Menuc
                Call    Dispatr
                Mov     Dx,0200h
                Call    Setpos
                Mov     Si,OFFSET Nopsp         ; ��ʾ�÷�
Over:           Mov     Bl,0ch
                Call    Dispatr
                Jmp     SHORT Exitnocro         ; ת�˳�

Quit:                                           ; Alt_X �����˳����
                Cmp     Chang,0
                Jnz     Ov2                     ; �޸Ĺ�ת
Dlf1:           Mov     Ah,3eh                  ; δ�޸Ĺ�, �ر��ļ�
                Mov     Bx,Handle
                Int     21h
                Cmp     Newf,0
                Jz      Qk                      ; �������ļ�, תֱ���˳�
                Mov     Ah,41h                  ; �����ļ�, ͬʱδ�޸Ĺ�
                Mov     Dx,OFFSET FilenaMe
                Int     21h                     ; ��ɾ��֮
                Jmp     SHORT Qk
Ov2:
                Call    Setpos_dh
                Mov     Si,OFFSET Saveif
                Call    Dispatr&inkey           ; ѡ���Ƿ����?
                Jz      Noext
                Call    LowCase1
                Cmp     Al,'y'                  ; 'Y' ������
                Jz      SaveFile1
                Cmp     Al,'n'                  ; 'N' ��, ���ж��Ƿ����ļ�
                Jz      Dlf1
Noext:          Jmp     Editbegin               ; ������, ���ر༭
SaveFile1:
                Call    Sok
                Mov     Newf,0
                Jmp     SHORT Dlf1
Qk:
                Call    Save_Se_Cfg             ; �������ļ�
                Mov     inswp,1
                call    shins
                Call    Cls
Exitnocro:
                Mov     Ax,4c00h                ; �����˳�
                Int     21h

Okpsp:
                push    cx
                mov     ax,1003h
                xor     bl,bl
                int     10h
;                Mov     Al,0f3h                 ; ���ټ���
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
                Jmp     SHORT Nxtspace          ; �����ո��
Nospace1:
                Cmp     BYTE PTR [Si],'/'
                Jz      Nx_Set
                Jmp     short N_Spc
Nx_Set:
                Dec     Cl                      ; '/' ��Ĳ����б�
                Inc     Si
                Mov     Al,[Si]
                Call    LowCase1
                Cmp     Al,'w'
                Jz      Set_Wide                ; ����"W"�趨�༭����
                jmp     n_spc

Set_Wide:
                Dec     Cl
                Mov     Ch,3
                Mov     Di,OFFSET GotoBuf       ; ����ֵ�Ƶ� GotoBuf
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
                Call    Get_Num                 ; ����� GotoBuf -> Ax
                Pop     Cx                      ; �ÿ��� (80-255)
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
                Cmp     Al,[Si]                 ; �лس�
                Jnz     Okpsp1
                Mov     Fal0a,1
Np1:            Inc     Si
                Dec     Cl
                Jmp     SHORT N_spc
Okpsp1:         Or      Cl,Cl
                Jnz     NojH
                Jmp     Ov1
NojH:           Cmp     Cl,31                   ; PSP ʣ�೤�Ȳ��ó���32
                Jbe     Okname                  ; (�����ڶԻ�����ʾ)
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
Okeent:         Mov     [Di],Al         ; ת���ļ����� Filename
                Inc     Di
Nmov:           Inc     Si
                Dec     Cl
                Jnz     Nextpsp
                Pop     Si
                Cmp     BYTE PTR [SI],32        ; �ļ�����һ���ַ�
                Jae     Filenok
                Jmp     Ov1             ; ������ĸ, ת�����˳�
Filenok:
; -------
                call    set_chinese
; -------
                Call    OpenFile        ; �򿪲�ת���ļ����༭��
                Call    Window          ; ��ʾ��Ļ����
                Mov     Ax,Filesto
                Mov     Es,Ax
                Mov     Siseg,Ax
                Mov     Diseg,Ax
                Call    CalCircle       ; �����ʹ�ÿռ����
                Mov     Row,2           ; �ù��λ�ó��Ի�
                Mov     Col,0
Shed_old:
                Call    Sold            ; ˢ�±༭��Ļ
Editbegin:
                Cmp     Warning,0       ; �༭��ʼ���
                Jz      Noinswp0
                Mov     Inswp,0         ; �����ڴ治������, ��ǿ�Ʋ����
                Call    Shins           ; ��ʾ���롢��������״̬
Noinswp0:       Mov     Crif,0
                Call    Seeline         ; ��ʾ�С��кż�����ֵ
                Call    Calps1
                Mov     Es,Siseg
                Mov     Ah,Es:[Si]
                Or      Ah,Ah
                Jnz     Noind
                Push_si
Ncur:           Mov     Es,Siseg        ; ���ǰ�� 0 ȫ��Ϊ����
                Mov     Dl,Es:[Si]      ; (��֤����ȫ��Ļ�ζ�)
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
                Mov     Es:[Si],Dl      ; ��ĩ�س����ļ�������
Noind:          Call    CorectCol       ; ����ȷ���ֹ��λ Col
                Mov     Dl,Col
                Mov     Dh,Row
                Cmp     Dl,Left_Mark
                Jb      Need_Show1      ; ��ʾ�����Ļ (�ҹ�)
                Mov     Al,Left_Mark
                add     al,screenwide
                Cmp     Dl,Al
                Ja      Need_Show       ; ��ʾ�Ұ���Ļ (���)
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
Po_ok:          Sub     Dl,Left_Mark    ; ���λ��Ҫ��ȥ��߽�
                pusha
                cmp     iffast,0
                jnz     bjok
                call    cmpcurasc
                mov     ax,'��'
                jae     qjok
bjok:
                mov     ax,'��'
qjok:
                int     10h
                popa
                Call    Set_bios_pos          ; �ñ༭���λ
                Mov     IfChr1,0
                Call    Inkey           ; ����һ�ַ�
                pushf
                push    ax
                mov     ax,'��'
                int     10h
                pop     ax
                popf
                Jnz     IsWords
                Jmp     Ctrl_key        ; ת��չ���ܼ�
IsWords:        Jmp     Words           ; ת���ƹ��ܼ�

GoNs&Ed:
                Call    Newsh           ; �����ܷ��غ�,ˢ����ʾ,ת�༭��ʼ
                Jmp     Editbegin

User_Buffer:

CODE            ENDS
                END     Begin

;
; ������������������������������
; ��    Eof: End of program   ��
; ������������������������������
;
