; ������������������������������������
; �� �ļ���   : SE_FILE.INC         ��
; �� �汾��   : 2.0                 ��
; �� ��  ��   : ��                ��
; �� ��  ��   : 1991 �� 12 ��       ��
; ��                                ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved in ShenZhen  ��
; ������������������������������������
;
; ���ļ����������ļ��������ӳ���, �����¼���:
; -----------------------------------------------------------------------------
; InputName     -  �ڶԻ���, �� PianFileName Ϊ������, ���� 25 ���ַ�
; WritePianBuf  -  �� PianBuf �е�һ���ֽ�д���ļ���Ϊ HandlePian ���ļ�
; SaveFile      -  �������� FileSeg:0 ��ʼ�ĳ� ReadSize ������д���ļ��� Handle
; Sok           -  �������༭�ļ�����
; OpenFile      -  �����༭�ļ�, ��ת�����༭��
; Tuin          -  �ļ�����ʱ, ɾ��ÿ����β�Ŀո��
; -----------------------------------------------------------------------------

Open_Se_Cfg:
                Mov     Ax,3d00h        ; ����ʽ�������ļ�
                Mov     Dx,OFFSET Se_Cfg
                Int     21h
                Jc      Open_Ret
                Mov     Cx,79
                Mov     Dx,OFFSET Cfg_Msg
                Mov     Bx,Ax
                Mov     Ah,3fh
                Int     21h             ; ���� 79 �ֽ�
Cfg_ok:         Jc      Open_Ret
                Mov     Ah,3eh
                Int     21h
Open_Ret:       Ret

Save_Se_Cfg:
                cmp     word ptr cs:VBUF,0B000h
                jnz     short nof0lv
                mov     byte ptr cs:lv,0F0h
nof0lv:
                Mov     Ah,3ch          ; �����������ļ�
                Mov     Dx,OFFSET Se_Cfg
                Xor     Cx,Cx
                Int     21h
                Jc      Open_Ret
                Mov     Cx,79
                Mov     Dx,OFFSET Cfg_Msg
                Mov     Bx,Ax
                Mov     Ah,40h
                Int     21h             ; д�� 79 �ֽ�
                Jmp     SHORT Cfg_ok

INPUTNAME:
                mov     byte ptr cs:if_color,0
                Call    Dispatr
                Mov     Si,OFFSET PianFileName
                Mov     Cl,23   ;25
                Call    Input   ; ��������ļ���
                mov     byte ptr cs:if_color,1
                Ret

WRITEPIANBUF:
                Mov     Ah,40h
                Mov     Bx,HandlePian
                Mov     Cx,1
                Mov     Dx,OFFSET PianBuf
                Int     21h
                Ret

SAVEFILE:
                PuSh    Ds              ; Readsize = ����������
                Mov     Ah,40h
                Mov     Bx,Handle
                Mov     Cx,ReadSize
                Mov     Dx,FileSeg
                Mov     Ds,Dx
                Xor     Dx,Dx
                Int     21h
                Pop     Ds
                Ret

SOK:
                Xor     Si,Si
                Mov     Es,Filesto
                Mov     Siseg,Es
                Mov     Ax,4200h
                Mov     Bx,Handle
                Xor     Cx,Cx
                Xor     Dx,Dx
                Int     21h             ; ��ָ�뵽�ļ�ͷ
Sok1:           Xor     Di,Di
Sok11:          Mov     Es,Siseg        ; Siseg:Si ��ָ����ʼ���̴�
                Mov     Al,Es:[Si]
                Cmp     Al,' '
                Jnz     Not_Chg_Tab
                Call    Chg_To_Tab      ;�жϲ�ת��ΪTAB
                                        ;����AL=TAB,��ת���ɹ�,��SI�ı�
                                        ;����, Al �� SI ������
Not_Chg_Tab:
                Cmp     Al,0ah
                Jnz     No0a
                Mov     Al,0dh
No0a:
                Cmp     Al,7fh
                Jnz     No_7fh
                Xor     Al,Al           ; 7FHת��Ϊ0
No_7fh:
                Mov     Es,Fileseg
                Mov     Es:[Di],Al
                cmp     al,0FFh
                jnz     short no1ae
                mov     byte ptr es:[di],1Ah     ; 0FFh ת��Ϊ 1Ah
no1ae:
                Inc     Di
                Call    Si_inc
                Cmp     Al,1ah
                Jz      Sokret
                Cmp     Di,0ffffh
                Jnz     Cppd
Saveblock:
                Mov     Readsize,0ffffh         ; 64K ���ݴ���
                PuSh    Ax                      ; ���һ��д���ַ���ջ
                Call    SaveFile
                Pop     Ax                      ; ��ջ, �Ƿ�س�
                Cmp     Al,0dh
                Jnz     Sok1                    ; ����Ҫ������һ�д���
                Xor     Di,Di
Cppd:           Cmp     Al,0dh
                Jnz     Sok11
                Call    Tuin
                Inc     Di
                Mov     BYTE PTR Es:[Di],0ah
Sok3:           Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Or      Al,Al
                Jz      Sok2
                Inc     Di
                Cmp     Di,0ffffh
                Jz      Saveblock
                Jmp     SHORT Sok11
Sok2:           Call    Si_inc
                Jmp     SHORT Sok3
Sokret:         Call    Tuin
                Inc     Di
                Mov     Readsize,Di
                Call    SaveFile
                Mov     Ah,40h
                Mov     Bx,Handle
                Xor     Cx,Cx
                Xor     Dx,Dx
                Int     21h             ; �ص��ļ����������
                Ret

OPENFILE:
                Mov     Ax,3d02h        ; ��д��ʽ���ļ�
                Mov     Dx,OFFSET Filename
                Int     21h
                Jnc     Openok
                Cmp     Al,2            ; AX = erreor code
                Jnz     Error           ; AX = 2 is File not exist
                Xor     Cx,Cx
                Mov     Dx,OFFSET Filename
                Mov     Ah,5bh
                Int     21h             ; �������ļ�
                Jc      Error
                Mov     Newf,1          ; �����ļ����
                Jmp     Openok          ; ��ȷת
Error:          Mov     Dx,0200h
                Push    Ax
                Call    Setpos
                Pop     Ax
                Call    Geterrmsg
                Jmp     Over
Openok:         Mov     Fileseg,Ds
                Add     Fileseg,1000h   ; �������� = DS+1000h
                Mov     Diseg,Ds
                Add     Diseg,2000h     ; �༭���� = DS+2000h
                Mov     Filesto,Ds
                Add     Filesto,2000h
                Xor     Di,Di
                Mov     Handle,Ax
AllRight:       Mov     Ax,4202h
                Mov     Bx,Handle
                Xor     Cx,Cx
                Xor     Dx,Dx
                Int     21h             ; ȡ�ļ����� -> Dx:Ax
                mov     Filesl,1
                mov     Filesh,0
                jc      short nflh
                add     ax,1
                adc     dx,0            ; ���ļ�����+1. ����һЩ�ļ�ĩβ
                                        ; ��һ���� 1Ah �ļ�����������,
                                        ; ��ô��ʱӦ����ȷ�������һ���ַ�
                Mov     Filesl,Ax
                Mov     Filesh,Dx
nflh:
                Mov     Ax,4200h
                Mov     Bx,Handle
                Xor     Cx,Cx
                Xor     Dx,Dx
                Int     21h             ; ���ļ�ָ�뵽�ļ���
                Mov     Cl,EditWide     ; CL=�༭��Ļ����+1
                Inc     Cl
Again:          PuSh    Cx
                Mov     Ax,Filesh
                Or      Ax,Ax
                Jnz     Largef          ; ���� 64K ���ļ�, ת
                Mov     Ax,Filesl
                Jmp     SHORT Readok
Largef:
                Mov     Ax,0ffffh
Readok:
                Mov     Readsize,Ax
                PuSh    Ds
                Mov     Ah,3fh
                Mov     Bx,Handle
                Mov     Cx,Readsize
                Mov     Dx,Fileseg
                Mov     Ds,Dx
                Xor     Dx,Dx
                Int     21h                     ; �����ļ�
                Pop     Ds
                Mov     Ax,Filesh
                Or      Ax,Ax
                Jnz     Mov_seg
                Mov     Si,Readsize
                or      si,si
                jz      short ndsi
                dec     si              ;mov     si,1
ndsi:
                Mov     Es,Fileseg
                Mov     BYTE PTR Es:[Si],1ah    ; �ļ�β���������-1
Mov_seg:
                Mov     Ax,Filesl
                Mov     Dx,Filesh
                Sub     Ax,Readsize
                Sbb     Dx,0
                Mov     Filesl,Ax
                Mov     Filesh,Dx
                Xor     Si,Si
                Pop     Cx
                Or      Cl,Cl
                Jnz     Mvmv
                Mov     Cl,EditWide
                Inc     Cl
Mvmv:           Cmp     Cl,EditWide             ; ���ȹ�,
                Jbe     Mov_begin
Mov_start:
                Inc     FileRowt                ; ���ļ��м�1
Mov_begin:
                Mov     Es,Fileseg
                Mov     Al,Es:[Si]
                Cmp     Filesh,0
                Ja      n_fend
                Cmp     Si,Readsize
                Jb      n_fend
                mov     al,1ah
                jmp     short not_1a
n_fend:
                Cmp     Al,1ah
                Jnz     Not_1a
;                Cmp     Filesh,0
;                Ja      Yes_1a
;                Cmp     Si,Readsize
;                Jae     Not_1a
;Yes_1a:
                Mov     Al,0FFh ;0dh    ; ��δ�����볤��, 1Ah תΪ 0FFh
Not_1a:
                Cmp     Al,0ah
                Jnz     Exp0a
                Cmp     Fal0a,15
                Jnz     Exp0a2
No0aok2:        Jmp     Mov_next
Exp0a2:         Cmp     Dcont,0
                Jz      No0aok2
Exp0a:          Cmp     Al,Tab
                Jz      Noexptab
                Jmp     Mov2
NoExpTab:       Push    Cx
                Mov     Al,EditWide
                Inc     Al
                Xor     Ah,Ah
                Xor     Ch,Ch
                Sub     Ax,Cx
                Call    Caltab          ; ���� TAB �Ʊ�λ�� -> AL
                Cmp     Al,EditWide
                Jbe     Stsp
                Mov     Al,EditWide
Stsp:           Mov     Cl,Al
                Mov     Al,EditWide
                Inc     Al
                Xor     Ah,Ah
                Xor     Ch,Ch
                Sub     Ax,Cx
                Pop     Cx
Stnsp:          Call    CmpAbsAdress
                jae     short A0000_ret
                Jmp     Stnsp1
A0000_ret:      Push_si
                Mov     Es,FileSeg
                Mov     WORD PTR Es:[0fffeh],0
                Push    Ax
                Dec     FileRowt
                Mov     Ax,FileRowt
                Dec     Ax
                Mov     FileRow,Ax
                Mov     Col,0
                Call    GotoLineEnd
                Mov     BYTE PTR Es:[Si],1ah
                Mov     Ax,FileRowt
                Cmp     Block,2
                Jnz     A_r1
                Cmp     Ax,Blocke
                Ja      A_r1
                Mov     Blocke,Ax
                Cmp     Ax,Blockh
                Ja      A_r1
                Mov     Blockh,Ax
A_r1:
                Cmp     Pian,2
                Jnz     A_r
                Cmp     Ax,Pianer
                Ja      A_r
                Mov     Pianer,Ax
                Cmp     Ax,Pianhr
                Ja      A_r
                Mov     Pianhr,Ax
A_r:            Pop     Ax
                Pop_si
                Ret
Stnsp1:
                Mov     Es,Diseg
                Mov     BYTE PTR Es:[Di],20h
                Call    Di_inc
                Dec     Cl
                Cmp     Cl,Al
                Jb      jmn     ;Mov_next
                Jmp     Stnsp
jmn:
                jmp     mov_next

Mov2:
                Call    CmpAbsAdress
                Jb      N1_A0000
                Jmp     A0000_ret
N1_A0000:
                Or      Al,Al
                Jnz     Zero_No
                Mov     Al,7fh          ; 0 ת��Ϊ7FH
Zero_No:
                Mov     Es,Diseg
                Mov     Es:[Di],Al      ; ת�����༭��
                Dec     Cl
                Cmp     Al,0dh
                Jnz     Cont1
                Cmp     Fal0a,0
                Jz      Exp0a1
                Inc     Dcont
                Cmp     Dcont,15
                Jb      No0dokok
                Mov     Dcont,0
                Jmp     SHORT Exp0a1
No0dokok:       Inc     Cl
                Jmp     SHORT Mov_next
Exp0a1:         Call    StorEsp
                Jmp     SHORT Mov_next
Cont1:
                Cmp     Al,1ah
                Jnz     Mov_next1
dsin:
                call    di_dec
                inc     cl
                Cmp     Cl,EditWide             ; ���ȹ�,
                ja      cffok
                mov     es,diseg
                cmp     byte ptr es:[di],0FFh
                jz      short dsin
cffok:
                call    di_inc
                dec     cl
                mov     es,diseg
                mov     byte ptr es:[di],01Ah

                Push    Es
                Mov     Es,FileSeg
                Mov     WORD PTR Es:[0fffeh],0
                Pop     Es
StorEsp:        Call    CmpAbsAdress
                Jb      N_A0000
                Jmp     A0000_ret
N_A0000:
                Call    Di_inc
                Or      Cl,Cl
                Jz      Stor
                Mov     Es,Diseg
                Mov     BYTE PTR Es:[Di],0
                Dec     Cl
                Jmp     SHORT Storesp
Stor:
                Retn

Mov_next1:      Call    Di_inc
Mov_next:
                Inc     Si
                Cmp     Si,Readsize
                Jb      Mov_next2
                Jmp     Again
Mov_next2:      Or      Cl,Cl
                Jnz     Mvvv
                Mov     Cl,EditWide
                Inc     Cl
                Jmp     Mov_start
Mvvv:           Jmp     Mov_begin

TUIN:
                Dec     Di
                Cmp     Di,0ffffh
                Jz      Tuiok2          ; �˵�����ͷ, ��������
TUIN1:          Or      Di,Di
                Jz      Tuiok
                Dec     Di
                Mov     Dl,Es:[Di]
                Cmp     Dl,20h          ; �ص��س�ǰ��Ŀո��TAB
                Jz      Tuin1
                Cmp     Dl,TAB
                Jz      Tuin1
TUIOK1:         Cmp     Al,1ah          ; �ص��ļ�������ǰ�Ļس�
                Jnz     Tuiok2
                cmp     dl,1Ah
                Jz      Tuin1
                Cmp     Dl,0dh
                Jz      Tuin1
                Cmp     Dl,0ah
                Jz      Tuin1
TUIOK2:         Inc     Di
Tuiok:          cmp     al,1ah
                jnz     tuiok_ture
                mov     word ptr es:[di],0a0dh
                add     di,2
tuiok_ture:
                Mov     Es:[Di],Al      ; ���˵����һ���ַ�
                Retn
