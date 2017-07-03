; ������������������������������������
; �� �ļ���   : SE_FIND.INC         ��
; �� �汾��   : 2.0                 ��
; �� ��  ��   : ��                ��
; �� ��  ��   : 1991 �� 12 ��       ��
; ��                                ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved in ShenZhen  ��
; ������������������������������������
;
; ���ļ����������ִ�����/�滻���ӳ���, �����¼���:
; ------------------------------------------------------
; Find&Replace  -  ����/�滻�����ĳ���
; AnykeyRet     -  ����������� (������ Find&Replace ��)
; Search        -  ��Ѱ�ӳ���
; ------------------------------------------------------

FIND&REPLACE:
                mov     byte ptr if_color,0
                Call    Calps1
                PuSh_si                 ; ��ǰָ����ջ
                Call    Setpos_dh
                Mov     Si,OFFSET Find
                Call    Dispatr
                Mov     Si,OFFSET Findbuf
                Mov     Cl,30
                Call    Input           ; ��������ִ�
                Cmp     Al,Esca         ; ESC ������
                Jnz     Finr
Findr:          Pop_si
                mov     byte ptr if_color,1
                Jmp     Editbegin
Finr:           Or      Ch,Ch           ; �ִ�����Ϊ��,�򷵻�
                Jz      Findr
                Mov     Finded,Ch       ; ���� -> Finded
                Mov     Si,OFFSET Replbuf
                Mov     BYTE PTR [Si],0dh
                Xor     Ch,Ch
                Cmp     CtrlQF,0
                Jnz     Repr            ; ���� ^QF ����, ����������ִ�
                Call    Setpos_dh
                Mov     Si,OFFSET Repl
                Call    Dispatr
                Mov     Si,OFFSET Replbuf
                Mov     Cl,30
                Call    Input           ; �����滻�ִ�
                Cmp     Al,Esca
                Jz      Findr
Repr:           Mov     RePled,Ch       ; �滻�ִ�����->RePled (Ϊ0���滻)
                Call    Setpos_dh
                Mov     Si,OFFSET FanShi
                Call    Dispatr
                Mov     Si,OFFSET Fanbuf
                Mov     Cl,5
                Call    Input           ; ������ҷ�ʽ
                mov     byte ptr if_color,1
                Pop_di                  ; ������ǰָ��
                Cmp     Al,Esca         ; ESC ����
                Jz      Findr1ok
                Mov     Uorl,0          ; ��ʼ��, Ԥ�ú��Դ�Сд,�ʴ�ʽ
                Mov     Ask,0           ; �ǿ���, ����һ��
                Mov     Inblck,0
                Mov     IfFind,0
                Mov     IfrePl,0
                Mov     Binval,0
                Or      Ch,Ch           ; ��������ʼ����
                Jnz     Findr55
                Jmp     Sbegin
Findr1ok:       Jmp     Findr1
Findr55:        Mov     Si,OFFSET Fanbuf
Findr5:         Mov     Al,[Si]
                Cmp     Al,0dh
                Jnz     HaveFanShi
                Jmp     Sbegin
HaveFanShi:     Cmp     Al,'G'          ; ȫ��
                Jz      Bbe
                Cmp     Al,'g'
                Jnz     Sbegin1         ; �� 'G' ѡ��ת
Bbe:            Mov     Di,Filesto      ; �ò�����ʼ��Ϊ FileSto
                Mov     Diseg,Di
                Xor     Di,Di
                Inc     Si
                Dec     Ch              ; ��һ��ʽ��
                Or      Ch,Ch
                Jz      Sbegin          ; û����,ת��ʼ��Ѱ
Sbegin1:        Mov     Al,[Si]
                Cmp     Al,'U'          ; ���Դ�Сд
                Jz      Nou_l
                Cmp     Al,'u'
                Jnz     Sbegin2         ; �� 'U' ѡ��ת
Nou_l:          Mov     Uorl,1          ; �ò�����
                Inc     Si
                Dec     Ch              ; ��һ��ʽ��
                Or      Ch,Ch
                Jz      Sbegin          ; û��,��ʼ��Ѱ
Sbegin2:        Mov     Al,[Si]
                Cmp     Al,'N'          ; �Զ�/�ʴ� 'N'
                Jz      Noask
                Cmp     Al,'n'
                Jz      Noask
                Cmp     Al,'I'          ; ���ڲ��� 'I'
                Jz      Inbok
                Cmp     Al,'i'
                Jnz     Segin
Inbok:          Cmp     Block,2
                Jnz     Nextchri
                Mov     Inblck,1
                Mov     Ax,Blockh
                Push    Si
                Xor     Si,Si
                Call    Calps           ; �ӿ��׿�ʼ
                Mov     Di,Siseg
                Mov     Diseg,Di
                Mov     Di,Si
                Pop     Si
                Jmp     SHORT Nextchri
Segin:          Cmp     Al,'A'
                Ja      Nextchri
                Jmp     SHORT Sbegin22  ; ����ĸѡ��ת�����ж�
Noask:          Mov     Ask,1           ; �ò��ʴ�
Nextchri:       Inc     Si
                Dec     Ch              ; ��һ��ʽ��
                Or      Ch,Ch
                Jz      Sbegin          ; û��,��ʼ��Ѱ
                Jmp     Findr5          ; ����һ����ʽ��
Sbegin22:       Xchg    Ch,Cl
                Mov     Bx,Cx
                Xor     Bh,Bh
                Call    B10asb1         ; ��������֮����
                Dec     Binval
Sbegin:                                 ; ^L ��ڴ�
                Call    Search          ; ��Ѱһ��
                Cmp     Al,0dh          ; ������Ϊ�س���? �������ѵ��ļ�β
                Jnz     Findr4
                Cmp     Binval,0
                Jz      Findr2          ; ����Ϊ��,����Ҫ��
                Dec     Binval
                Call    Di_inc
                Jmp     SHORT Sbegin
Findr4:         Mov     Si,OFFSET Nofind
                Cmp     IfFind,0
                Jz      Anykeyret
                Cmp     IfrePl,0        ; �滻����������ʾ��Ļ
                Jz      NorePlace
                Call    Show
NorePlace:      Mov     Si,OFFSET Eoffindok
                Mov     Dl,45
                Call    Setpos1_dh
                Jmp     SHORT Anykey1
Anykeyret:                              ; ����������ؽ���
                Call    Setpos_dh
Anykey1:        Call    Dispatr&inkey   ; �����������
                Jmp     Editbegin
Findr2:
                Mov     IfFind,1        ; ����Ѱ����־
                Mov     Si,Diseg        ; �޸ĵ�ǰָ��Ϊ���ҵ���
                Mov     Siseg,Si
                Mov     Si,Di
                Cmp     RePled,0
                Jnz     Haverep1
                Cmp     CtrlQA,0        ; ���� ^QA �滻, �����
                Jnz     Haverep1
                Jmp     Notaskok        ; ���滻�ִ�ת
Haverep1:       Cmp     Ask,1
                Jnz     Hav
                Jmp     Haverep         ; �Զ��滻ת
Hav:            Push_si
                Call    Caline          ; ������ڵڼ���
                Mov     FileRow,Ax
                call    midel_line
                Mov     WinRow,Ax       ; �޸Ĵ����м��ļ���
;                Mov     FileRow,Ax

;                Mov     Row,2
                mov     al,Cure_find_pos
                mov     Row,al

                Mov     Ax,Si
                Mov     Col,Al          ; ���ͣ�ڶ�Ӧָ�봦
                Mov     Editing,0
                Call    Show            ; ��ʾ���ִ�
                Mov     Si,OFFSET Findbuf
                Mov     Bl,Lv
                And     Bl,0fh          ; ���ո�λ
                Mov     Cl,4
                Shl     Bl,Cl           ; ������λ
                Mov     Bh,Lv
                Shr     Bh,Cl
                Or      Bl,Bh           ; ���� Lv �����Ҹ���λ, ��ǰ����ɫ����
                Mov     Cl,Finded       ; ��Ѱ�ִ��ĳ���
                Xor     Ch,Ch
                Mov     Dl,Col
;                Mov     Dh,2
                mov     dh,Cure_find_pos

Nrep:           Call    Setpos
                Mov     Al,[Si]
                Push    Cx
                Call    Putc
                Pop     Cx
                Inc     Dl
                Inc     Si
                Cmp     Dl,79
                Jbe     Nonextl
                Xor     Dl,Dl           ; ת��һ��
                Inc     Dh
Nonextl:        Loop    Nrep
                Call    Setpos_dh
                Mov     Si,OFFSET Ifreplace
                Call    Dispatr&inkey   ; �û�ѡ��
                Pop_si
                Jz      Endask
                Cmp     Al,'Y'          ; Ҫ�滻ת
                Jz      Haverep
                Cmp     Al,'y'
                Jz      Haverep
                cmp     al,Esca         ; �� ESC ����
                jz      short Endask
                jmp     ctrllok         ; ����,ִ�� Ctrl_L
Endask:         Jmp     Editbegin       ; ��Ѱ����,����
Haverep:        Push_si                 ; ����ԭָ��
                Mov     Di,OFFSET ReplBuf
                Mov     Cl,inswp
                PuSh    Cx              ; ���� INSERT ״̬
                Mov     Inswp,1         ; ��Ϊ����
RePlnext:       Mov     Al,[Di]
                Cmp     Al,0dh
                Jz      EndofrePl       ; �������ִ��Ļس������
                Mov     Inschr,Al
                PuSh    Di
                PuSh_si
                Call    Insert_rep      ; ���滻�ַ���һ����
                Pop_si
                Call    Si_inc
                Pop     Di
                Inc     Di
                Jmp     SHORT RePlnext
EndofrePl:      Pop     Cx              ; INSERT ��ջ
                Mov     Inswp,Cl        ; �ָ�
                Mov     Cl,Finded       ; ԭ�ִ�����
                Xor     Ch,Ch
Delrepnx:       PuSh    Cx
                PuSh_si                 ; ��ԭλ������
                Call    Delete_rep      ; ɾ��ԭ�ִ�
                Pop_si
                Pop     Cx
                Loop    Delrepnx
                Pop_di                  ; ����ԭָ��
                Call    Mov_di_to_si
                Inc     IfrePl          ; �滻������һ
                Cmp     Ask,1
                Jz      Delre           ; ���Զ���ʽ, �����
                Jmp     SHORT Notaskok
Delre:          Mov     Cl,Repled
                Or      Cl,Cl           ; �滻�ʳ���Ϊ0, �򲻱���
                Jz      No_Nextpos
Nextpos:        Call    Di_inc
                Loop    Nextpos         ; ָ�������滻��
No_Nextpos:     Mov     Ax,IfrePl
                Call    Calcnum         ; ת������ֵ
                Mov     Al,linec
                Mov     Attr,Al         ; ����ɫ
                Mov     Si,OFFSET lines
                Mov     Bp,40
Seencont:       Mov     Dx,Bp
                Call    Setpos1_dh
                mov     byte ptr if_color,0
                Mov     Dl,[Si]
                Call    To_Scr          ; ��ʾ����
                Inc     Si
                Inc     Bp
                Cmp     Bp,43
                Jbe     Seencont
                Call    Setpos_dh
                Mov     Dl,'('
                Call    To_Scr
                Mov     Dl,2ch
                Call    Setpos1_dh
                Mov     Dl,')'
                Call    To_Scr
                Mov     Dl,2eh
                Call    Setpos1_dh
                Mov     Si,OFFSET Pleaswait
                Mov     Bl,Menuc
                Call    Dispatr
                mov     byte ptr if_color,1
                Jmp     Sbegin          ; �Զ�ʱ������һ��
Notaskok:
                Call    Caline          ; ������ڵڼ���
Gotolinenum:
                Mov     FileRow,Ax
                call    midel_line
                Mov     WinRow,Ax       ; �޸Ĵ����м��ļ���
;                Mov     FileRow,Ax

;                Mov     Row,2
                mov     al,Cure_find_pos
                mov     Row,al

                Mov     Ax,Si
                Mov     Col,Al          ; ���ͣ�ڶ�Ӧָ�봦
                Mov     Editing,0
                Call    Show
                cmp     CtrlQA,0        ; ������ʱ, ת����
                jz      short Findr1
;                cmp     ask,1
;                jz      Findr1          ; �Զ��滻ʱ, ����
                jmp     ctrllok         ; ����ִ�� Ctrl_L
Findr1:
                mov     byte ptr if_color,1
                Jmp     Editbegin       ; ��Ѱ����,����

;-----------
; ��Ѱ�ӳ���
;-----------
SEARCH:
                Mov     Si,OFFSET Findbuf
SE2:
;
                Push    Si
                Call    Mov_di_to_si
                Call    Caline
                Pop     Si
                Cmp     Ax,FileRowt
                Jb      See2            ; �����ļ�����, �����
                Mov     Al,1ah
                Jmp     Searchr
;
See2:           Mov     Es,Diseg
                Mov     Al,Es:[Di]      ; ȡ��Ŀ����
                Cmp     Block,2
                Jnz     ContCmp
                Cmp     Inblck,0        ; �ǿ�����Ѱ��? ������
                Jz      ContCmp
                Push    Ax
                Push    Si
                Call    Mov_di_to_si
                Call    Caline          ; ��, ���㵱ǰ�� -> AX
                Pop     Si
                Pop     Bx
                Cmp     Ax,Blockh
                Jb      ZeroNcmp        ; �ڿ���ǰ, ���Ƚ�
                Cmp     Ax,Blocke
                Mov     Ax,Bx
                Jbe     ContCmp         ; �ѵ���β, �򷵻�
                Mov     Al,1ah
ContCmp:        Cmp     Al,1ah
                Jz      Searchr
                Or      Al,Al           ; ȡ����, ��һ��
                Jz      ZeroNcmp
                Call    Secomp
                Jz      Onok            ; ���ҵ���һ����ȵ��ַ�
ZeroNcmp:       Call    Di_inc          ; ����,Ŀ���һ
                Jmp     SHORT Se2
ONOK:           PuSh_di
ONOK1:          Mov     Al,[Si+1]
                Cmp     Al,0dh          ; �ִ��ҵ�(�Ƚϵ�����), ���� AL=0DH
                Jz      Searchok
                Call    Di_inc          ; Ŀ���һ
                Inc     Si              ; Դ��һ
                Mov     Es,Diseg
                Mov     Al,Es:[Di]
                Cmp     Al,1ah
                Jz      Searchok
                Call    Secomp
                Jz      Onok1
                Pop_di
                Call    Di_inc
                Jmp     Search
SEARCHOK:       Pop_di
SEARCHR:        Ret
SECOMP:         Call    Lowcase         ; ͳһСд
                Mov     Ah,Al
                Mov     Al,[Si]         ; ȡ��������
                Call    Lowcase
                Cmp     Al,Ah           ; ���߱Ƚ�
                Ret

Cure_find_pos   db      0

midel_line:
                mov     Cure_find_pos,2
                mov     dl,EditHigh
                shr     dl,1
                xor     dh,dh
ml_r1:
                cmp     ax,dx
                jae     short ml_r
                dec     dl
                jmp     short ml_r1
ml_r:
                add     Cure_find_pos,dl
                sub     ax,dx
                retn
