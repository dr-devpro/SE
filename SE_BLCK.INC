; ������������������������������������
; �� �ļ���   : SE_BLCK.INC         ��
; �� �汾��   : 2.3                 ��
; �� ��  ��   : ��                ��
; �� ��  ��   : 1991,12 - 92, 8     ��
; ��                                ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved in ShenZhen  ��
; ������������������������������������
;
; ���ļ��������ڿ�������ӳ���, �����¼���:
; -----------------------------------------
; AddBlock      -  �鿽��ʱ�����ӳ���
; CalBlockHigh  -  �����ĸ߶��� BX
; BlockCopy     -  �鿽������ǰ��ǰ��
; BlockDel      -  ��ɾ��
; BlockDefin    -  �鶨��
; BlockMove     -  ���ƶ�����ǰ��ǰ��
; BlockRead     -  �����ļ�����ǰ��ǰ��
;------------------------------------------

ADDBLOCK:
                Add     Blockh,Bx
                Add     Blocke,Bx
                Inc     Blockh
                Inc     Blocke
                Ret

CALBLOCKHIGH:
                Mov     Ax,Blocke
                Sub     Ax,Blockh
                Mov     Bx,Ax           ; ��ĸ߶� = ��β-���� -> BX
                Mov     Ax,FileRow
                Cmp     Ax,Blockh
                Ret

BLOCKCOPY:
                Push    Bx              ; ����߶ȼ���
                Mov     Chang,1
                Add     Bx,FileRowt     ; The last new line
                Mov     Ax,FileRowt
                Sub     Ax,FileRow
                Or      Ax,Ax
                Jnz     Nxokk
                Mov     Ax,1
Nxokk:          Xor     Dx,Dx
                Mov     Cl,EditWide
                Inc     Cl
                Xor     Ch,Ch
                Mul     Cx              ; DX:AX �з�ȫ��Ҫ�ƶ�������
                PuSh    Ax
                PuSh    Dx
                Mov     Ax,Bx
                Call    EW_TO_BP
                Mov     Si,BP
                Dec     Si
                Call    Calps           ; calc the last new line's PS
                PuSh_si                 ; --> DI
                Mov     Ax,FileRowt
                Dec     Ax
                Call    EW_TO_BP
                Mov     Si,BP
                Dec     Si
                Call    Calps           ; cals the last old line's PS
                Pop_di                  ; --> SI
                Pop     Dx
                Pop     Cx
                Call    Mv_nx           ; Call to move it
                Mov     FileRowt,Bx
                Inc     FileRowt        ; FileRowt = BX+1
                Mov     Ax,FileRow
                Xor     Si,Si
                Call    Calps
                PuSh_si
                Mov     Ax,Blockh
                Xor     Si,Si
                Call    Calps
                Pop_di
                Pop     Cx              ; �ָ�����
Movnl:          Xor     Bp,Bp
Movnx:          Call    Addsi
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Call    CmpAbsAdress
                Jb      CopyMemok
                Inc     Cx
                Mov     Ax,FileRowt
                Sub     Ax,Cx
                Mov     FileRowt,Ax
                Mov     Ax,Blocke
                Sub     Ax,Cx
                Mov     Blocke,Ax
                Call    A0000_ret
                Jmp     SHORT Copyr
CopyMemok:      Call    Subsi
                Cmp     Al,1ah
                Jnz     Copyr2
                Call    Adddi
                Mov     Es,Diseg
                Mov     BYTE PTR Es:[Di],0dh
                Call    Subdi
                Call    Mov_di_to_si
                Call    Addsi
                Call    Si_inc
                PuSh_si
                Call    Caline
                Mov     Cx,Si
                Xor     Ch,Ch
                Pop_si
                Call    Jdnx
                Jmp     SHORT Copyr
Copyr2:         Call    Adddi
                Mov     Es,Diseg
                Mov     BYTE PTR Es:[Di],Al
                Call    Subdi
                Inc     Bp
                Mov     Al,EditWide
                Inc     Al
                Xor     Ah,Ah
                Cmp     Bp,Ax
                Jnb     Copyrrr
                Jmp     Movnx
Copyrrr:        Or      Cx,Cx
                Jz      Copyr
                Dec     Cx
                Call    Addsi
                Call    Adddi
                Jmp     Movnl
Copyr:          Mov     Crif,1
                Ret

BLOCKDEL:
                Mov     Chang,1
                Mov     Ax,FileRowt
                Mov     FileRowtOld,Ax
                Dec     Ax
                Sub     Ax,Blocke       ; �ļ���������ȥ��β=����Ƶ�����
                Mov     Bx,Blocke
                Sub     Bx,Blockh
                Inc     Bx
                Sub     FileRowt,Bx     ; �ļ�������������ĸ߶�
                Or      Ax,Ax
                Jnz     Nx111
                Mov     Ax,1
Nx111:          Xor     Dx,Dx
                Mov     Cl,EditWide
                Inc     Cl
                Xor     Ch,Ch
                Mul     Cx              ; Totel BYTEs of totle linEs
                PuSh    Ax              ; --> DX:AX
                PuSh    Dx
                Mov     Ax,Blockh
                Xor     Si,Si
                Call    Calps           ; Block head's PS --> DI
                PuSh_si
                Mov     Ax,Blocke
                Inc     Ax
                Cmp     Ax,FileRowtOld
                Jb      Nx333
                Dec     Ax
                Mov     Block,3
Nx333:          Xor     Si,Si
                Call    Calps           ; Block end+1's PS --> SI
                Pop_di
                Pop     Dx
                Pop     Cx
                Call    Mv_bx           ; Move (del) block
                Mov     Ax,Blockh
                Mov     FileRow,Ax
                call    midel_line      ; ����
                Mov     WinRow,Ax
                Cmp     Block,3
                Jnz     Nx444
                Mov     Col,0
                Call    Surelast
                Inc     FileRowt
Nx444:          Mov     Block,0
                Ret

BLOCKDEFIN:
                Mov     Ax,FileRow
                Cmp     Block,0
                Jnz     Haveblock       ; �Ѷ������, ת
                Mov     Block,2
                Mov     Blockh,Ax       ; û�������, ���ÿ��־
Blockok:        Mov     Blocke,Ax       ; ����,��β���� Filerow
                Jmp     SHORT Comp
Haveblock:      Cmp     Ax,Blockh       ; ��ԭ���׵�������? ��ת�����β
                Jae     Blockok
                Push    Ax              ; ������ԭ���׻���
                Mov     Ax,Blockh
                Mov     Blocke,Ax
                Pop     Ax
                Mov     blockh,Ax
Comp:           Inc     Ax
                Cmp     Ax,FileRowt     ; ��β���ļ����бȽ�
                Jb      Ckok            ; С��, ת����
                Xor     Si,Si
                Call    Calps
                Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],1ah
                Inc     FileRow
                Call    Calps1
                Dec     FileRow
                Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],0dh
                Call    Si_inc
                Mov     Cl,Col
                Inc     Cl
                Xor     Ch,Ch
                Call    Jdnx
Ckok:           Mov     Ax,FileRow
                Push    Ax
                Mov     Al,Col
                Push    Ax
                Mov     Ax,Blockh
                Mov     FileRow,Ax
                Call    GotoLineBegin
                Call    StFileRow&Col
                Mov     Ax,FileRow
                Mov     Blockh,Ax
                Mov     Ax,Blocke
                Mov     FileRow,Ax
                Call    GotoLineEnd
                Call    StFileRow&Col
                Mov     Ax,FileRow
                Mov     Blocke,Ax
                Pop     Ax
                Mov     Col,Al
                Pop     Ax
                Mov     FileRow,Ax
                Call    Show
                Ret

BLOCKMOVE:
                Call    CalBlockHigh    ; �����߶� -> BX
                Mov     Isnew,0
                Jae     Nx80            ; ��ǰ���ڿ�֮��, ת
                Call    AddBlock        ; �������������
                Mov     Isnew,1
                Jmp     SHORT Nx81
Nx80:           Cmp     Ax,Blocke
                Jbe     Ckokr1          ; �ڿ��м��෵��
Nx81:           Push    Bx
                Mov     Ax,Filerow
                Push    Ax
                Mov     Ax,WinRow
                Push    Ax
                Mov     Al,Col
                Mov     Ah,Row
                Push    Ax
                Call    BlockCopy       ; ������
                Call    BlockDel        ; ɾ��ԭ��
                Pop     Bx
                Mov     Col,Bl
                Mov     Row,Bh
                Pop     Bx
                Mov     WinRow,Bx
                Pop     Bx
                Mov     FileRow,Bx
                Mov     Blockh,Bx       ; ���¿����ֹ���
                Mov     Blocke,Bx
                Pop     Bx
                Add     Blocke,Bx
                Cmp     Isnew,1
                Jz      Adok            ; ����ǰ����ԭ��֮��, ��
                Mov     Ax,Blockh       ; ����Ӧ�ڼ����߶�֮���һ��
                Dec     Ax
                Inc     Bx
                Mov     Blocke,Ax
                Sub     Blockh,Bx
Adok:           Mov     Block,2
                Mov     Editing,0
                Call    Show
Ckokr1:         Ret

BLOCKREAD:
                Mov     BYTE PTR [Si],0 ; �ļ����� 0 ����
                Mov     Ax,3d02h
                Mov     Dx,OFFSET PianFileName
                Int     21h             ; �򿪸��ļ�
                Jnc     ReadOpenOk
                Jmp     PianErr
ReadOpenOk:     Mov     Chang,1
                Mov     Bx,Handle
                Push    Bx              ; ����ԭ�ļ���
                Mov     Handle,Ax
                Mov     Bx,FileRow
                Push    Bx              ; ����ԭ�ļ���
                Mov     Bl,Col
                Mov     Bh,Row
                Push    Bx              ; ����ԭ�������
                Call    InsLastLine     ; ���һ�б�Ϊ�հ���, �õ������ַ
                Mov     Ax,FileRowt
                Mov     Blockh,Ax
                Call    AllRight        ; ���벢ת���ļ�, Ȼ����Ϊ��
                Mov     Ax,FileRowt
                Dec     Ax
                Mov     Blocke,Ax
                Mov     Block,2
                Mov     Ah,3eh
                Mov     Bx,Handle
                Int     21h             ; �ر��ļ�
                Pop     Bx
                Mov     Col,Bl
                Mov     Row,Bh
                Pop     Bx
                Mov     FileRow,Bx
                Pop     Bx
                Mov     Handle,Bx
                Mov     Bx,FileRow
                Push    Bx
                Mov     Bx,WinRow
                Push    Bx
                Mov     Bl,Col
                Mov     Bh,Row
                Push    Bx
                Mov     Bx,Blocke
                Sub     Bx,Blockh
                Push    Bx              ; �߶���ջ
                Call    AddBlock
                Call    BlockCopy       ; ������
                Cmp     Isnew,1
                Jnz     Nodecline       ; ���ڵ�һ��, ��Ӧ��������֮����
                Dec     Blockh          ; ���ӵ�����, һ��ɾ��
Nodecline:      Call    BlockDel        ; ɾ����
                Pop     Ax              ; �߶ȳ�ջ -> AX
                Pop     Bx
                Mov     Col,Bl
                Mov     Row,Bh
                Pop     Bx
                Mov     WinRow,Bx
                Pop     Bx
                Mov     FileRow,Bx
                Mov     Blockh,Bx       ; ����Ϊ�ļ���
                Mov     Blocke,Bx
                Add     Blocke,Ax       ; ��βΪ�ļ��� + ��߶�
                Mov     Block,2         ; �ÿ���
                Mov     Editing,0
                Mov     Undelete,0      ; ��ָ����
                Jmp     Shed_old        ; ����

PaiBlock:
                Mov     PaiBan,1
                Mov     DLIf,1
                Mov     Al,Inswp
                Mov     Ah,Insdent
                Push    Ax
                Mov     Ax,FileRow      ; Alt B ȡ�ļ���ǰ��
                Mov     Inswp,1
                Cmp     AltBIf,0
                Jnz     PaiNLOk
                Mov     Insdent,0
                Mov     Ax,Blockh       ; Ctrl B ���ļ���Ϊ����
PaiNLOk:        Mov     FileRow,Ax
                Push    Ax
                Mov     Col,0
                Call    Calps1
                Mov     Cl,File_Wide    ; �Ű����(����)
                Shl     Cl,1            ; �� 2 �����Ŀ���
                Xor     Ch,Ch
SkipNextChr:    Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],0dh
                Jnz     SNC1
                Jmp     PaiNextLine1
SNC1:           Cmp     BYTE PTR Es:[Si],1ah
                Jnz     SNC2
                Jmp     PaiNextLine
SNC2:           Call    Si_inc
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],0dh
                Jnz     SNC3
                Jmp     PaiNextLine1
SNC3:           Cmp     BYTE PTR Es:[Si],1ah
                Jz      PaiNextLine
                Loop    SkipNextChr
                Call    StFileRow&Col
                Mov     Ax,FileRow
                Push    Ax
                Call    CorectCol       ; ��׼���λ��
                Call    Calps1
                Mov     Es,Siseg
                Mov     Al,Es:[Si]      ; �ж�ĩβ������
                Cmp     Al,0a1h
                Jae     PaiHZ
                Mov     Di,OFFSET MoXW  ; ����ĩ�ַ�
NxMXW:          Cmp     BYTE PTR [Di],0
                Jz      PaiBiaook
                Cmp     Al,[Di]
                Jz      Colaok1
                Inc     Di
                Jmp     SHORT NxMXW
PaiHZ:          Call    Si_inc
                Mov     Es,Siseg
                Mov     Ah,Es:[Si]
                Call    Si_dec          ; �˻�ԭ��λ��
                Mov     Di,OFFSET MoHZ  ; ����ĩ�ַ�
NxMHZ:          Cmp     BYTE PTR [Di],0
                Jz      PaiBiaook
                Cmp     Ax,[Di]
                Jz      Colaok
                Inc     Di
                Inc     Di
                Jmp     SHORT NxMHZ
Colaok:         Call    Si_inc
Colaok1:        Call    Si_inc
                Call    StFileRow&Col
                Pop     Ax
                Mov     Ax,FileRow
                Push    Ax
PaiBiaook:      Call    InsEnter
                Pop     Ax
                Pop     Bx
                Cmp     AltBIf,0
                Jz      NoAltB
                Inc     Ax              ; Alt B ����һ��
                Jmp     PaiNLOk
NoAltB:         Push    Ax
PaiNextLine:    Pop     Ax
                Cmp     AltBIf,0
                Jnz     ABEnd
                Inc     Ax
                Cmp     Ax,Blocke
                Jae     ABEnd
                Jmp     PaiNLOk
ABEnd:          Pop     Ax
                Cmp     AltBIf,0
                Jnz     ABEnd1
                Mov     Insdent,Ah
ABEnd1:         Mov     Inswp,Al
                Mov     PaiBan,0
                Mov     DLIf,0
                Ret
PaiNextLine1:   Push_si
                Call    StFilerow&Col
                Inc     FileRow
                Mov     Col,0
                Call    Calps1
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Pop_si
                Cmp     Al,' '
                Jnz     ABok
                Cmp     AltBIf,0
                Jz      PaiNextLine
                Push_si
                Call    DelCurChr
                Pop_si
                Jmp     SHORT PaiNextLine1
ABok:           Cmp     Al,0dh
                Jnz     DBOKOK
                Pop     Ax
                Inc     Ax
                Push    Ax
                Jmp     PaiNextLine
DBOKOK:         Call    StFileRow&Col
                Call    DelCurChr
                Pop     Ax
                Jmp     PaiNLOk

Make_Tab:
                Mov     Chang,1
                Mov     Ax,FileRow
                Push    Ax
                Mov     Al,Col
                Mov     Ah,Inswp
                Push    Ax
                Mov     Inswp,1                 ; �ò���״̬��
                Mov     Ax,Blockh
Next_Block_Lin: Mov     FileRow,Ax
                Xor     Si,Si
                Call    Calps
                Mov     Cl,EditWide
                Inc     Cl
                Xor     Ch,Ch
Next_Blk_Chr:   Mov     Es,Siseg
                Mov     Al,Es:[Si]
;                Cmp     Al,' '
;                Jbe     Next_Tab_Chg            ; �������Ʒ�
                Cmp     Al,0a1h
                Jae     Next_Tab_Chg            ; ��������
                Mov     Di,OFFSET Tab_Graph
Next_Block_Chr: Cmp     Al,[Di]
                Jz      Tab_ok
                Inc     Di
                Cmp     BYTE PTR [Di],0
                Jnz     Next_Block_Chr
Next_Tab_Chg:   Call    Si_inc
                Loop    Next_Blk_Chr
                Mov     Ax,FileRow
                Inc     Ax
                Cmp     Ax,Blocke
                Jbe     Next_Block_Lin
                Pop     Ax
                Mov     Col,Al
                Mov     Inswp,Ah
                Pop     Ax
                Mov     FileRow,Ax
                Ret
Tab_ok:         Push_si
                Cmp     Al,16h                  ;'='
                Jz      Qtcr
                Cmp     Al,1Ch                  ;'-'
                Jnz     FQt
Qtcr:           Call    Si_inc
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],16h    ;'='
                Jz      FQt
                Cmp     BYTE PTR Es:[Si],1Ch    ;'-'
                Jz      FQt
                Pop_si
                Mov     Es,Siseg
                Mov     BYTE PTR Es:[Si],1Ch    ;'-'    ; �� '-' ���϶��Ѵ�
                Jmp     SHORT Next_Tab_Chg
FQt:            Mov     Si,OFFSET Tab_Graph
                Sub     Di,Si
                Shl     Di,1
                Mov     Si,OFFSET Tab_9
                Add     Di,Si
                Pop_si
                Mov     Al,[Di]
                Mov     Es,Siseg
                Mov     Es:[Si],Al
                Call    Si_inc
                Mov     Al,[Di+1]
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],16H    ;'='    ; ���� '='��'_' �� ' ' ������
                Jz      No_HHZZ
                Cmp     BYTE PTR Es:[Si],1Ch    ;'-'
                Jz      No_HHZZ
                Cmp     BYTE PTR Es:[Si],' '
                Jz      No_HHZZ
                Mov     Inschr,Al               ; �����������
                Push_si
                Push    Cx
                Call    Insert_rep
                Pop     Cx
                Pop_si
                Jmp     SHORT N_1111
No_HHZZ:        Mov     Es:[Si],Al
N_1111:         Cmp     Cl,1
                Jbe     J_N_T
                Dec     Cl
J_N_T:          Jmp     Next_Tab_Chg
