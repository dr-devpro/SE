; ������������������������������������
; �� �ļ���   : SE_SHOW.INC         ��
; �� �汾��   : 2.1                 ��
; �� ��  ��   : ��                ��
; �� ��  ��   : 91,12 - 92,5        ��
; ��                                ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved              ��
; ������������������������������������
;
; ���ļ����������ַ���ʾ����Ļ�仯���ӳ���, �����¼���:
; -----------------------------------------------------------
; Putc         -  ��ʾһ�ַ��ڵ�ǰ���λ��
; SetColor     -  Ϊ Attr ����ɫ
; To_Scr       -  ͬ Putc, ���������ַ��ķ�Χ, ������ͬ����ɫ
; DispAtr      -  ��ʾһ�ִ�, �� 0 ����
; ShowLine     -  ��ʾ FileRow ���ڱ༭��Ļ
; Show         -  ��ʾ������ǰ�༭��Ļ
; NewSh        -  ����/ɾ���ַ���, ������ʾ�µ�����
; Cls          -  ��ȫ��Ļ
; Earse        -  ��Ի���
; Scroll       -  ������Ļ�����ӳ���
; ScrollDown1  -  �¹���Ļ
; ScrollUp1    -  �Ϲ���Ļ
; ShIns        -  ��ʾ״̬����
; SeeLine      -  ��ʾ�кű���
; Window       -  ��ʾ�༭����
; -----------------------------------------------------------

vbuf_offset     dw      0
if_color        db      0
d_video         db      0               ; ֱ��д�����(1=Y)

PUTC:
                push    es
                push    di
                mov     di,VBUF         ;0b800h
                mov     es,di
                mov     di,vbuf_offset
                mov     ah,bl
                cmp     word ptr cs:VBUF,0B000h
                jz      short st_ok
                cmp     byte ptr psp_color,0
                jnz     short isbk      ;st_ok
                cmp     byte ptr if_color,0
                jz      short isbk      ;st_ok
                cmp     al,0a0h
                ja      short add_8
                cmp     al,'"'
                jz      short add_8
                cmp     al,"'"
                jz      short add_8
                cmp     al,'('
                jz      short add_8
                cmp     al,')'
                jz      short add_8
                cmp     al,'['
                jz      short add_8
                cmp     al,']'
                jz      short add_8
                cmp     al,'{'
                jz      short add_8
                cmp     al,'}'
                jz      short add_8
                cmp     al,'0'
                jb      short st_ok
                cmp     al,'9'
                ja      short st1_
                add     ah,3
                jmp     short st_ok
add_8:
                add     ah,8
                jmp     short st_ok
st1_:
                cmp     al,'z'
                ja      short st_ok
                cmp     al,'a'
                jb      short st2_
                add     ah,4
                jmp     short st_ok
st2_:
                cmp     al,'Z'
                ja      short st_ok
                cmp     al,'A'
                jb      short st_ok
                add     ah,7
                jmp     short st_ok
st_ok:
                cmp     byte ptr to_bjpm,0
                jnz     short isbk
                mov     bh,ah
                and     bh,0F0h
                cmp     bh,40h
                jz      short isbk
                cmp     bh,60h
                jz      short isbk
                mov     bh,lv
                and     bh,0F0h
                and     ah,0Fh
                or      ah,bh
isbk:
                cmp     byte ptr d_video,0
                jnz     short putc_dv
                push    bx
                Mov     Bh,mode         ; AL = ASCII ��
                mov     bl,ah           ; BL = ASCII ����
                Mov     Cx,1
                Mov     Ah,9
                Int     10h
                pop     bx
putc_ret:
                pop     di
                pop     es
                retn
putc_dv:
                stosw
                jmp     short putc_ret

SETCOLOR:
; ----------------------------------------------
; ����: ������ɫ -> Attr, �ṩ TO_SCR �ӳ�����
; ����: DL  : ǰ��ɫ
;       BL  : ����ɫ
; ����: Attr: �������
; ---------------------------------------------
                Mov     Al,Dh           ; DH = ����ɫ
                Mov     Cl,4
                Shl     Al,Cl           ; DL = ǰ��ɫ
                Or      Al,Dl
                Mov     Attr,Al         ; ������ Attr ��
                Ret

GetCtrlColor:
                cmp     word ptr cs:VBUF,0B000h
                jz      short gcc_r
                Cmp     Block,2
                Jnz     Nsok
                Mov     Bx,FileRow
                Cmp     Bx,Blockh
                Jb      Nsok
                Cmp     Bx,Blocke
                Ja      Nsok
                Mov     Bl,Attr ;87
gcc_r:
                Ret
Nsok:
                Mov     Bl,Lv
                And     Bl,0f0h ; ȡ����
                Or      Bl,0ch  ; ����
                Ret

to_bjpm         db      0

TO_SCR:
; -----------------------------------------------------------------------
; ����: ��ʾ�༭��Ļ�е�һ���ַ�, ���ݲ�ͬ���ַ��ò�ͬ����ɫ, ��:�����ַ�
; ����: DL  : Ҫ��ʾ���ַ�
;       Attr: ��ű༭��Ļɫ
; ����:
; -----------------------------------------------------------------------
                Mov     Bl,Attr         ; Ԥ����ʾ����Ϊ�༭��Ļɫ
                Mov     Al,Dl
                Cmp     Al,0ah
                Jz      Crokok
                Cmp     Al,0dh
                Jnz     Cont3           ; �ǻس���ת
Crokok:         Call    GetCtrlColor
                Mov     Al,Cr
                Cmp     Dl,0ah
                Jnz     Ns2
                Mov     Al,Cr1
Ns2:            Cmp     ShowCr,0
                Jz      Contt
                Mov     Al,20h
                Cmp     Dl,1ah
                Jnz     EofN
                Call    GetCtrlColor
                Mov     Al,26
                Jmp     Putc
EofN:           Cmp     Dl,0ah
                Jnz     Contt
                Mov     Al,0ah
Contt:
                cmp     al,0FFh
                jnz     short nsh1a
                mov     al,1Ah
                Call    GetCtrlColor
nsh1a:
                Cmp     Al,7fh
                Jnz     No@
                Call    GetCtrlColor
                Mov     Al,'@'
No@:
                Jmp     Putc            ; ��ʾ���ַ�
Cont3:
                Cmp     Al,20h
                Jae     Contt
;                Cmp     Al,1ah
;                Jnz     Cont4
;                Mov     Al,eof
;                Mov     Bl,30h
;                Jmp     SHORT Ns2
;Cont4:
                Call    GetCtrlColor
                Cmp     ShowCr,0
                Jnz     Contt
                Add     Al,40h
                Jmp     SHORT Contt

DISPATR:
; -----------------------------------------------------------
; ����: �ӵ�ǰ���λ������ʾһ�ַ���, ���س������������ִ��
;       ��ִ���������Ʒ�, ������
; ����: SI: �ַ��������ʼ������, �ִ��� 0 ����
;       BL: �ַ�����ʾ����
;       DX: ��ʼ���λ��, ����ǰ��Ҫ�����ú�
; ����: ��
; -----------------------------------------------------------
                Mov     Al,[Si]
                Or      Al,Al
                Jz      Dispatrr        ; �ִ��������
                Cmp     Al,0dh
                Jz      CtSh1
                Cmp     Al,0ah
                Jz      CtSh
                Cmp     Al,7
                Jz      CtSh
                Call    Putc
Dnxt:           Inc     Si
                Inc     Dl
                Call    Setpos
                Jmp     SHORT Dispatr
CtSh1:          Inc     Dh
                Mov     Dl,0ffh
CtSh:           Mov     Ah,2
                Push    Dx
                Mov     Dl,Al
                Int     21h             ; ��ʾ�����ַ�(�س���)
                Pop     Dx
                Jmp     SHORT Dnxt
Dispatrr:
                call    set_bios_pos
                Ret

SHOWLINE:
; ---------------------------------------------------------------
; ����: ��ʾ��굱ǰ���ڱ༭��Ļ, �ɾݴ���λ�þ��� Attr �е���ɫ
; ����: FileRow: ��ǰ������ڵ��ļ���
;       Editing: �����Ƿ�����ʾ������������°���Ļ (1=��)
; ����: DL     : 0   (�н���ʱ)
;              : 1Ah (�ļ�����ʱ)
; ---------------------------------------------------------------
                Mov     Co,0
                Xor     Si,Si
                Mov     Ax,FileRow
                Call    Calps
                Mov     Al,Left_Mark
                Xor     Ah,Ah
                Mov     Bp,Ax
                Call    Addsi
                Call    CorectSi        ; ָ���Ƿ��׼����
                Jbe     Shown1          ; ��, ת
                Mov     Dh,Rw           ; ��, ָ�� Siseg:Si+1
                Xor     Dl,Dl
                Call    Setpos
                Mov     Al,20h
                Mov     Bl,Lv
                Call    Putc
                Mov     Co,1
Shown1:         Mov     Ax,FileRow
                Cmp     Block,2
                Jnz     Nobl
                Cmp     Ax,Blockh
                Jb      Nobl
                Cmp     Ax,Blocke
                Ja      Nobl
                Mov     Dx,0407h        ; ���װ���
                Jmp     SHORT Set1
Nobl:           Mov     Dh,0
                Mov     Dl,Lv
Set1:           Cmp     Pian,2
                Jnz     Setco
                Cmp     Ax,Pianhr
                Jb      Setco
                Cmp     Ax,Pianer
                Ja      Setco
                Mov     Al,Co
                Add     Al,Left_Mark    ; ������߽�->Co
                Cmp     Al,Pianhc
                Jb      Setco
                Cmp     Al,Pianec
                Ja      Setco
                Mov     Dx,0607h        ; Ƭ�صװ���
Setco:          Call    Setcolor
                Mov     Dl,Co
                Mov     Dh,Rw
                Call    Setpos
                Mov     Es,Siseg
                Mov     Dl,Es:[Si]
                Or      Dl,Dl
                Jz      Shlrt           ; �������
                push    ax
                mov     al,screenwide
                cmp     co,al
                pop     ax
                Jb      Srt
                Call    CorectSi
                Jnz     Srt
                Call    Si_dec
                Mov     Dl,20h
Srt:            Call    To_Scr
                Inc     Co
                push    ax
                mov     al,screenwide
                cmp     co,al
                pop     ax
                Ja      Shlrt80         ; �����һ�������
                Cmp     Dl,1ah
                Jz      Shlrt
                Call    Si_inc
                Jmp     Shown1
Shlrt:          Push    Dx
                Mov     Dl,Co
                Mov     Dh,Rw
                Call    Setpos          ; ����س��������
                Mov     Al,20h          ; Al = ' '
                Mov     ah,Attr
                cmp     ah,47h          ; �ǿ���ɫ,������ʾ����ɫ
                jz      short blckok
                mov     ah,Lv
blckok:
                xor     ch,ch
                mov     cl,screenwide
                inc     cl
                Sub     Cl,Co
                call    stosw_sub
                Pop     Dx
Shlrt80:        Mov     Ax,FileRow
                Inc     Ax
                Cmp     Ax,FileRowt
                Jb      Shlrt_80        ; ��ʾ�������з���
                Mov     Dl,1ah
Shlrt_80:       Cmp     Dl,1ah          ; ���ļ�β������²���Ļ
                Jnz     Shlrt1
                Cmp     Editing,1       ; ���Ǳ༭ʱ����ʾ,ת
                Jz      Shlrt1
                Mov     Ch,Rw
                Cmp     Ch,EditHigh
                Jae     Shlrt1
                Inc     Ch
                Xor     Cl,Cl
                mov     dl,screenwide
                Mov     Dh,EditHigh     ; Row=22, Col=79
                Mov     Ax,0600h
                Mov     Bh,Lv
                Int     10h
                Mov     Dl,1ah
Shlrt1:         Cmp     Dl,1ah
                Jz      Shlrt1_ok
                Xor     Dh,Dh
                Mov     Dl,EditWide
                Mov     Si,Dx
                Mov     Ax,FileRow
                Call    Calps
                Mov     Es,Siseg
                Mov     Dl,BYTE PTR Es:[Si]     ; Dl ȡ���༭������һ���ַ�
Shlrt1_ok:      Ret

SHOW:
; -------------------------------------------------------------------
; ����: ��ʾ��ǰ�����༭��Ļ;
; ����: WinRow: ��ǰ���ڶ������ڵ��ļ���
;       ShWinr: ��Ҫ��������п�ʼ��ʾʱ, �����ļ��к�; ���� SHOWCALL
;       CH    : ��Ҫ��������п�ʼ��ʾʱ, ������Ļ�к�; ���� SHOWCALL
; ����: ��
; -------------------------------------------------------------------
                Mov     Ax,WinRow
                Mov     ShWinr,Ax
                Mov     Ch,2
SHOWCALL:       Mov     Ax,FileRow
                PuSh    Ax
                Mov     Ax,ShWinr
                Mov     FileRow,Ax
                Mov     Al,Ch
Nextchr:        Mov     Rw,Al
                PuSh    Ax
                Call    Showline
                Pop     Ax
                Inc     Al
                Inc     FileRow
                Cmp     Dl,1ah
                Jz      Show_end
                Cmp     Al,EditHigh     ; ��ʾ������з���
                Jbe     Nextchr
Show_end:       Pop     Ax
                Mov     FileRow,Ax
                Ret

NEWSH:
; -------------------------------------------------------------------
; ����: �����ɾ���������������ʾ����һ�л����
; ����: Crif  : ���й�����, ����1, Ӧ������ʾ����
;       Row   : ��ʾ����Ļ��
; ����: ��
; -------------------------------------------------------------------
                Cmp     Crif,0
                Jnz     Showall
                Mov     Editing,1
                Mov     Al,Row
                Mov     Rw,al
                Mov     Ax,FileRow
                Push    Ax
Nxnoend:        Call    Showline
                Or      Dl,Dl
                Jz      Allretok        ; ������ĩȡ��0,����
                Cmp     Dl,0dh
                Jz      Allretok
                Cmp     Dl,1ah
                Jz      Allretok
                Inc     Rw
                Inc     FileRow
                Mov     Al,Rw
                Cmp     Al,EditHigh
                Jbe     Nxnoend
Allretok:       Pop     Ax
                Mov     FileRow,Ax
                Ret
SHOWALL:        Mov     Ax,FileRow
                Mov     Ch,Row
                Mov     ShWinr,Ax
                Mov     Editing,0
                Jmp     SHORT Showcall

CLS:
; -------------------------------------
; ����: ���������Ļ, ��굽��Ļ���Ͻ�
; ����: ��
; ����: ��
; -------------------------------------
                Mov     Al,' '
                Mov     Bl,7
                Call    Putc
                Mov     Ax,0600h
                Mov     Bh,7
                Xor     Cx,Cx
                mov     dl,screenwide
                Mov     Dh,EditHigh
                Add     Dh,2
                Int     10h
                Mov     Ah,2
                Mov     Bh,Mode
                Xor     Dx,Dx
                Int     10h
                Ret

EARSE:
; -----------------
; ����: ����Ի���
; ����: ��
; ����: ��
; -----------------
                Pushf
                Push    Ax
                Push    Bx
                Push    Cx
                Push    Dx
                Call    Setpos_dh
                Mov     Cx,41
                Mov     Al,20h
                mov     ah,bl
                call    stosw_sub
                Pop     Dx
                Pop     Cx
                Pop     Bx
                Pop     Ax
                Popf
                Ret

stosw_sub:
                cmp     byte ptr d_video,0
                jnz     short sts_d
                mov     bl,ah
                mov     bh,mode
                mov     ah,9
                int     10h
                retn
sts_d:
                push    es
                push    di
                mov     di,VBUF         ;0b800h
                mov     es,di
                mov     di,vbuf_offset
                repz    stosw
                pop     di
                pop     es
                ret

SCROLL:
                Mov     Al,1
                Mov     Bh,Lv
                Mov     Cx,0200h
                mov     dl,screenwide
                Mov     Dh,EditHigh
                Int     10h
                Ret

SCROLLDOWN1:
                Mov     Ah,7
                Call    Scroll
                Mov     Rw,2
                Call    Showline
                Dec     WinRow
                Ret

SCROLLUP1:
                Mov     Ah,6
                Call    Scroll
                Mov     Ah,EditHigh
                Mov     Rw,Ah
                Call    Showline
                Inc     WinRow
                Ret

ShA&Hhz:
                mov     byte ptr to_bjpm,1
                Mov     Dx,014dh
                Call    Setpos
                Mov     Si,OFFSET AllHz
                Mov     Bl,Onc
                Cmp     IfFast,0
                Jz      Dis2
                Mov     Si,OFFSET HalfHz
                Mov     Bl,Offc
                Jmp     SHORT Dis2

SHINS:
                mov     byte ptr to_bjpm,1
                Mov     Dx,003dh
                Call    Setpos
                Mov     Si,OFFSET Insertoff
                Mov     Bl,Offc
                push    ds
                xor     cx,cx
                mov     ds,cx
                mov     cx,ds:[485h]
                pop     ds
                Cmp     Inswp,0
                Jz      Dis1
                Mov     Si,OFFSET Inserton
                Mov     Bl,Onc
                Mov     Cx,0b0fh
                cmp     VBUF,0b800h
                jnz     short Dis1
                mov     ch,0dh
Dis1:           Mov     Ah,01
                Int     10h
                Call    Dispatr
                Mov     Dx,013dh
                Call    Setpos
                Mov     Si,OFFSET Inserton
                Mov     Bl,onc
                Cmp     Insdent,0
                Jnz     Dis2
                Mov     Si,OFFSET Insertoff
                Mov     Bl,offc
Dis2:           Call    Dispatr
                mov     byte ptr to_bjpm,0
                Ret

SEELINE:
                mov     byte ptr to_bjpm,1
                mov     byte ptr if_color,0
                Mov     Al,Col
                Inc     Al
                Xor     Ah,Ah
                Call    Calcnum         ; �������ǰ�к� -> lines ������
                Mov     Al,linec
                Mov     Attr,Al
                Mov     Bp,76           ; �к���ʾ��ʼ��
                Mov     Si,OFFSET Lines+1
SeeCol:         Mov     Dx,Bp
                Xor     Dh,Dh           ; �� 0 ��
                Call    Setpos
                Mov     Dl,[Si]
                Call    To_Scr
                Inc     Si
                Inc     Bp
                Cmp     Bp,78
                Jbe     SeeCol
                Mov     Ax,FileRow
                Inc     Ax
                Call    Calcnum         ; �������ǰ�к� -> lines ������
                Mov     Bp,68           ; �к���ʾ��ʼ��
                Mov     Si,OFFSET Lines
Seenext:        Mov     Dx,Bp
                Xor     Dh,Dh           ; �� 0 ��
                Call    Setpos
                Mov     Dl,[Si]
                Call    To_Scr
                Inc     Si
                Inc     Bp
                Cmp     Bp,71
                Jbe     Seenext
                Xor     Ax,Ax
                Call    Calcnum         ; �������� '0000'
                Mov     Si,OFFSET Lines
                Xor     Bp,Bp
                Mov     Ax,FileRow      ; ȡ��ǰ��
                Inc     Ax
Muln:           Xor     Dx,Dx
                Mov     Cx,FileRowt
                Div     Cx              ; ����������
                Add     Al,30h
                Mov     [Si+Bp],Al
                Inc     Bp
                Cmp     Bp,4
                Jz      Clok
                Or      Dx,Dx
                Jz      Clok
                Mov     Cx,10
                Mov     Ax,Dx
                Xor     Dx,Dx
                Mul     Cx
                Jmp     SHORT Muln
Clok:           Mov     Bp,70           ; ����ֵ��ʾ��ʼ��
                Mov     Si,OFFSET lines
Seenext1:       Mov     Dx,Bp
                Mov     Dh,1            ; �� 1 ��
                Call    Setpos
                Mov     Dl,[Si]
                Call    To_Scr
                Inc     Si
                Inc     Bp
                Cmp     Bp,72
                Jbe     Seenext1
                Mov     Dx,014ah
                Call    Setpos          ; С�����һλ
                Mov     Dl,[Si]
                Call    To_Scr
                Mov     Al,EditWide
                Inc     Al
                Xor     Ah,Ah
                Xor     Dx,Dx
                Mul     FileRowt
                Mov     Cx,16
                Div     Cx              ; ��ʹ�ÿռ� = FileRowt*(EditWide+1)/16
                Mov     CX,EachCircle
                Xor     Dx,Dx
                Div     Cx
                Mov     Cx,Ax           ; ��ԲȦ����
                Or      Cl,Cl
                Jz      Nowarning
                Mov     Warning,0
                Cmp     Cl,27*2
                Jb      Nowarning
                Mov     Ah,2            ; �ڴ治������
                Mov     Dl,7
                Int     21h
                Mov     Cl,27*2
                Mov     Warning,1
Nowarning:      Cmp     Cl,Oldmem
                Jz      Shccok
                Mov     Oldmem,Cl
                Push    Cx
                Call    Showmenu4
                Pop     Cx
SeeC:           Or      Cl,Cl
                Jz      Shccok
                Mov     Dx,0100h
                Xor     Ch,Ch
Nxtcc:          Call    Setpos
                Mov     Bl,Lv1
                shl     bl,4            ; And     Bl,0f0h
                Or      Bl,0Fh
                Mov     Si,OFFSET Circle
                Push    Dx
                Push    Cx
                Call    Dispatr         ; ��ʾ�������(�ڴ����)
                Pop     Cx
                Pop     Dx
                Inc     Dl
                Loop    Nxtcc
Shccok:         mov     byte ptr if_color,1
                mov     byte ptr to_bjpm,0
                Ret

WINDOW:
                mov     byte ptr to_bjpm,1
                mov     byte ptr if_color,0
                Xor     Dx,Dx
                Call    Setpos
                Mov     Si,OFFSET Menu1
                Mov     Bl,Menuc
                Call    Dispatr
                Mov     Dx,0137h
                Call    Setpos
                Mov     Bl,Menuc
                Mov     Si,OFFSET Menu5
                Call    Dispatr
                Mov     Dx,0037h
                Call    Setpos
                Mov     Bl,Menuc
                Mov     Si,OFFSET Menu6
                Call    Dispatr
                Call    Showmenu4
                Mov     Dx,0036h
                Call    Setpos
                Mov     Al,20h  ; ' '
                Mov     Bl,Lv1
                shl     bl,4
                Call    Putc
                Call    Shins
                Call    ShA&Hhz
                Mov     Dh,EditHigh
                Add     Dh,2
                Xor     Dl,Dl
                Call    Setpos
                Mov     Si,OFFSET Menu2
                Mov     Bl,70h
                Call    Dispatr
                Mov     Cx,10
                Mov     Si,OFFSET F1_F10
                Mov     Di,OFFSET F1_F10_Col
F_Nxt:          Mov     Dl,[Di]
                Mov     Dh,EditHigh
                Add     Dh,2
                Call    Setpos
                Mov     Bl,74h
;                Or      Bl,4
                Push    Di
                Push    Cx
                Call    Dispatr
                Pop     Cx
                Pop     Di
                Inc     Di
                Inc     Si
                Loop    F_Nxt
                Mov     Bl,Menuc
                Mov     Dh,EditHigh
                Inc     Dh
                Xor     Dl,Dl
                Call    Setpos
                xor     ch,ch
                mov     cl,screenwide
                inc     cl
                Mov     ah,Menuc
                Mov     Al,20h
                call    stosw_sub        ; ��Ի���
                Xor     Dl,Dl
                Mov     Dh,EditHigh
                Inc     Dh
                Call    Setpos
                Cmp     Newf,0
                Jz      Old
                Mov     Si,OFFSET Nfileasc
                Mov     Bl,Menuc
                Call    Dispatr
Old:            Mov     Si,OFFSET Fileasc
                Call    Dispatr
                Mov     Si,OFFSET Filename
                call    Dispatr
                cmp     screenwide,90
                jae     short se100
                mov     byte ptr if_color,1
                mov     byte ptr to_bjpm,0
                ret
se100:
                mov     dh,edithigh
                add     dh,2
                mov     dl,80
                call    setpos
                mov     si,offset china_dragon
                mov     bl,0ceh
                call    dispatr
                mov     dx,80
                call    setpos
                mov     si,offset address
                mov     bl,08eh
                call    dispatr
                mov     dx,0150h
                call    setpos
                mov     si,offset tele
                mov     bl,08fh
                call    dispatr
                mov     byte ptr if_color,1
                mov     byte ptr to_bjpm,0
                ret

Showmenu4:
                Mov     Dx,0100h
                Call    Setpos
                Mov     Al,20h  ;' '
                Mov     ah,Lv1
                shl     ah,4
                Mov     Cx,55
                call    stosw_sub
                Ret

Sold:
                Mov     Ax,WinRow
                Mov     Dl,Row
                Xor     Dh,Dh
                Add     Ax,Dx
                Dec     Ax
                Dec     Ax
                Cmp     Ax,FileRowt
                Jb      Shed_ok         ; ���δ���������, ת
                Mov     Ax,FileRowt
                Dec     Ax              ; FileRow = FileRowt - 1
                Push    Ax
                Sub     Ax,WinRow
                Inc     Ax
                Inc     Ax
                Mov     Row,Al          ; �µĹ����=(Filerowt-1)-Winrow+2
                Pop     Ax
Shed_ok:        Mov     FileRow,Ax
                Mov     Editing,0
                Call    Show
                Ret

Show_Num:
                mov     byte ptr if_color,0
                Push    Bp              ; Bp = ��ʼ��ʾ��
                Push    Ax
                Mov     Al,linec
                Mov     Attr,Al         ; ����ɫ
                Mov     Dx,Bp
                Call    Setpos1_dh
                Mov     Dl,'['
                Call    To_Scr
                Mov     Dx,Bp
                Add     Dl,5
                Call    Setpos1_dh
                Mov     Dl,']'
                Call    To_Scr
                Pop     Ax
                Call    Calcnum         ; ת�� AX ����ֵ
                Mov     Si,OFFSET lines
                Pop     Bp
                Inc     Bp
                Mov     Cx,4            ; ��ʾ 4 λ��
See_num:        Mov     Dx,Bp
                Push    Cx
                Call    Setpos1_dh
                Mov     Dl,[Si]
                Call    To_Scr          ; ��ʾһλ����
                Inc     Si
                Inc     Bp
                Pop     Cx
                Loop    See_num
                Mov     Dx,Bp
                Call    Setpos1_dh      ; ����������ֺ�
                mov     byte ptr if_color,1
                Ret
