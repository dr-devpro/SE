; ������������������������������������
; �� �ļ���   : SE_CALC.INC         ��
; �� �汾��   : 2.0                 ��
; �� ��  ��   : ��                ��
; �� ��  ��   : 1991 �� 12 ��       ��
; ��                                ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved in ShenZhen  ��
; ������������������������������������
;
; ���ļ��������ڼ��㷽����ӳ���, �����¼���:
; -------------------------------------------------------------------------
;  B10asb1    -  ����� [SI] ��ʼ, ����Ϊ BX ��һ���ַ�����, ����� Binval
;  CalcNum    -  �� AX ��ֵת��Ϊ�ַ�����, ���� Lines ��������; Ĭ�ϳ� 4 λ
;  Mzn        -  ͬ��, ����ָ���������� SI, ������ CL
;  CalTab     -  ���� AL ֵ���Ʊ�λ, ������ AL
;  Si_inc\ Si_dec\ Di_inc\ Di_dec\ Addsi\ Subsi\ Adddi\ Subdi\ Mov_di_to_si
;  CalPs1     -  ����� FileRow ��, �� Col �еı༭�ļ�ָ��; ������ Siseg:SI
;  CaLine     -  ����ָ�� Siseg:SI �ڵڼ���, �ڼ���; ������ AX:SI
;  LowCase    -  �� AL �е��ַ�ת��ΪСд; ������ AL
;  CalCircle  -  ����ÿ��Ȧ�����Ľ���
; -------------------------------------------------------------------------

B10ASB1:
                Mov     Binval,0
                Mov     Mult10,1
                Mov     Cx,10
B20:            Mov     Al,[Si+Bx-1]
                And     Ax,000fh
                Mul     Mult10
                Add     Binval,Ax
                Mov     Ax,Mult10
                Mul     Cx
                Mov     Mult10,Ax
                Dec     Bx
                Jnz     B20
                Ret

; ����ʱ, AX=��ת������
CALCNUM:
                Mov     Si,OFFSET lines
                Mov     Cl,4
MZN:            Mov     BYTE PTR [Si],30h
                Dec     Cl
                Inc     Si
                Or      Cl,Cl
                Jnz     Mzn     ; ���Ƚ� lines ������ȫ�� '0'
                Dec     Si
                Mov     Cx,10
C20:            Cmp     Ax,10
                Jb      C30     ; ��������λ, ����
                Xor     Dx,Dx
                Div     Cx
                Or      Dl,30h
                Mov     [Si],dl
                Dec     Si
                Jmp     SHORT C20
C30:            Or      Al,30h
                Mov     [Si],Al
                Ret

Chg_To_Tab:
                Cmp     Insdent,0
                Jnz     Cr_ind
                Cmp     Fal0a,0
                Jz      Chg_TT
Cr_ind:         Ret                     ; ������(��)���лس���ʽ, ��ת��
Chg_TT:         Push    Di
                Push_si
                Call    Caline
                Cmp     Block,2
                Jnz     Pi2
                Cmp     Ax,Blockh
                Jb      Pi2
                Cmp     Ax,Blocke
                Ja      Pi2
                Jmp     Chg_ret         ; �ڿ��м�, ��ת��
Pi2:            Cmp     Pian,2
                Jnz     Chg_ok          ; ��Ƭ����,ȫת��
                Cmp     Ax,Pianhr
                Jb      Chg_ok          ; Ƭ��������, ת��
                Cmp     Ax,Pianer
                Ja      Chg_ok          ; Ƭβ������, ת��
                Mov     Ax,Si
                Cmp     Al,Pianhc
                Jb      Chg_ok          ; Ƭ��������, ת��
                Cmp     Al,Pianec
                Jbe     Chg_ret         ; ��Ƭ�м�, ��ת��
Chg_ok:         Mov     Ax,Si           ; Ax=��
                Call    CalTab          ; �������TABλ
                Cmp     Al,EditWide
                Jbe     OKTab
                Mov     Al,EditWide     ; �����༭����,��ȡ�༭����ΪTABλ
OKTAB:          Xor     Ah,Ah
                Mov     Cx,Ax
                Sub     Cx,Si           ; ���� --> Cx
                Inc     Cl
                Pop_si
                Push_si
Nt_tab:         Call    Si_inc
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,' '
                Jnz     Nook_tab
                Loop    Nt_tab
Nook_tab:       Cmp     Cl,1
                Jbe     OKOK_tab        ; ����ת��ΪTAB, ת
Chg_ret:        Mov     Al,' '
                Pop_si                  ; �ָ�ԭSiseg:Si
                Pop di                  ; �ָ�ԭDiseg:Di
                Ret
OKOK_tab:       Mov     Al,TAB
                Call    Si_dec
                Pop_di                  ; ��������Siseg:Si
                Pop di                  ; �ָ�ԭDiseg:Di
                Ret

CALTAB:
                Xor     Ah,Ah
                Mov     Cl,8
                Div     Cl
                Xor     Ah,Ah
                Mov     Cl,8
                Mul     Cl
                Add     Al,Cl
                Dec     Al
                Ret

SI_INC:
                Inc     Si
                Jnz     Siir
SIIR1:          Add     Siseg,1000h      ; siseg = siseg+1000h
SIIR:           Ret

ADDSI:
                Clc
                Adc     Si,Bp           ; SI = SI + BP
                Jc      Siir1
                Ret

SI_DEC:
                Dec     Si
                Cmp     Si,0ffffh       ; SI = SI-1
                Jnz     Sidr
SIDR1:          Sub     Siseg,1000h      ; siseg = siseg-1000h
SIDR:           Ret

SUBSI:
                Clc
                Sbb     Si,Bp           ; SI = SI - BP
                Jc      Sidr1
                Ret

DI_INC:
                Inc     Di
                Jnz     Diir
DIIR1:          Add     Diseg,1000h      ; diseg = diseg+1000h
DIIR:           Ret

ADDDI:
                Clc
                Adc     Di,Bp           ; DI = DI + BP
                Jc      Diir1
                Ret

DI_DEC:
                Dec     Di
                Cmp     Di,0ffffh
                Jnz     Didr
DIDR1:          Sub     Diseg,1000h
DIDR:           Ret

SUBDI:
                Clc
                Sbb     Di,Bp           ; DI = DI - BP
                Jc      Didr1
                Ret

MOV_DI_TO_SI:
                Mov     Si,Diseg
                Mov     Siseg,Si        ; siseg = diseg
                Mov     Si,Di           ;    si = di
                Ret

EW_TO_BP:
                Push    Ax
                Mov     Al,EditWide
                Inc     Al
                Xor     Ah,Ah
                Mov     Bp,Ax
                Pop     Ax
                Ret

;------------------------------------------------------
; ���Ե�ַ = FileRow * ( EditWide + 1 ) + Col --> Dx:Ax
;            Dx ���� 12 λ, ����Ӧ�ε�ַ
;            Ax ��Ϊ�öε�ַƫ����
; Dx �ټ��� FileSto, ������նε�ַ
;------------------------------------------------------
CALPS1:
                Mov     Al,Col
                Xor     Ah,Ah
                Mov     Si,Ax           ; Si = Col
                Mov     Ax,FileRow      ; Ax = FileRow
CALPS:
                Push    Dx
                Push    Cx
                Mov     Cl,EditWide
                Inc     Cl
                Xor     Ch,Ch
                Xor     Dx,Dx
                Mul     Cx              ; FileRow*(EditWide+1) --> Dx:Ax
                Add     Ax,Si           ; Dx:Ax + Col (Si)
                Jnc     No_adx
                Inc     Dx
No_adx:         Mov     Cl,12
                Shl     Dx,Cl           ; Dx ��Ϊ�ε�ַ, Ax ��Ϊƫ����
                Mov     Si,Ax
                Add     Dx,FileSto      ; ��ַ+FileSto --> DX
                Mov     Siseg,Dx        ; --> Siseg
                Pop     Cx
                Pop     Dx
                Ret

;------------------------------------------------------------------------
; ��Ӧ Siseg:Si �ľ���ƫ����ָ�� Dx:Ax
;               = Shr ( Siseg - FileSto ), 12 --> Dx, Si --> Ax, �� Dx:Ax
; ��ָ�� Dx:Ax / (EditWide+1), ������ -- Ax, ���� --> Dx
; ���� Dx �����յ�ʣ������
;------------------------------------------------------------------------
CALINE:
                Push    Dx
                Push    Cx
                Mov     Ax,Si           ; Si --> Ax
                Mov     Dx,Siseg
                Sub     Dx,FileSto
                Mov     Cl,12
                Shr     Dx,Cl           ; Shr (Siseg-FileSto),12 --> Dx
                Mov     Cl,EditWide
                Inc     Cl
                Xor     Ch,Ch
                Div     Cx              ; Dx:Ax / (EditWide+1) --> Ax(����)
                Mov     Si,Dx           ; SI=Dx=����=��������
                Pop     Cx
                Pop     Dx
                Ret

LOWCASE:
                Cmp     Uorl,0
                Jnz     Lowok
LOWCASE1:
                Cmp     Al,'A'
                Jb      Lowok
                Cmp     Al,'Z'
                Ja      Lowok
                Add     Al,32
LOWOK:          Ret

lowctrl:
                cmp     al,20h
                jae     lowcok
                add     al,60h
lowcok:
                ret

CalCircle:
                Mov     Ax,0a000h
                Sub     Ax,FileSto
                Mov     Cx,27*2
                Xor     Dx,Dx
                Div     Cx
                Mov     EachCircle,Ax
                Ret

To_Binval:
                Xchg    Ch,Cl
                Mov     Bx,Cx
                Xor     Bh,Bh
                Call    B10asb1
                Dec     Binval
                Ret

Get_Num:
                Mov     Si,OFFSET Gotobuf
                Call    To_Binval
                Mov     Ax,Binval
                Inc     Ax
                Ret
