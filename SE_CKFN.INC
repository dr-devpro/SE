; ©³©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©·
; ©§ ÎÄ¼þÃû   : SE_CKFN.INC         ©§
; ©§ °æ±¾ºÅ   : 2.0                 ©§
; ©§ ×÷  Õß   : ¼ò¾§                ©§
; ©§ ÈÕ  ÆÚ   : 1991 Äê 12 ÔÂ       ©§
; ©§                                ©§
; ©§ °æÈ¨ËùÓÐ (¡ï)Starsoft Corp,.   ©§
; ©§ Allright reserved in ShenZhen  ©§
; ©»©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¿

                include se_mtab.inc     ; ÊÖ¶¯ÖÆ±í×Ó³ÌÐò

Ctrl_key:
                cmp     al,Ctrl_Up
                jnz     short New_j1
                jmp     up_scrl
New_j1:
                cmp     al,Ctrl_Down
                jnz     short New_j2
                jmp     dn_scrl
New_j2:
                cmp     al,Alt_Up
                jnz     short New_j3
                cmp     Last_FX,0
                jz      short ZU1
                lea     si,Up_tab
                cmp     S&D_line,0
                jz      short Sj2ok
                lea     si,Up_tab2
Sj2ok:
                call    PD_FX
                jmp     short FX1
ZU1:
                mov     al,160d         ;' '
                cmp     S&D_line,0
                jz      short FX1
                mov     al,'Ÿ'
FX1:
                mov     ah,1            ; ·½ÏòÉÏ
                call    Ascok_call1
                jmp     C_Up_Call
New_j3:
                cmp     al,Alt_Down
                jnz     short New_j4
                cmp     Last_FX,0
                jz      short ZU2
                lea     si,Down_tab
                cmp     S&D_line,0
                jz      short Sj3ok
                lea     si,Down_tab2
Sj3ok:
                call    PD_FX
                jmp     short FX2
ZU2:
                mov     al,160d         ;' '
                cmp     S&D_line,0
                jz      short FX2
                mov     al,'Ÿ'
FX2:
                mov     ah,2            ; ·½ÏòÏÂ
                call    Ascok_call1
                jmp     C_Dn_Call
New_j4:
                cmp     al,Alt_Left
                jnz     short New_j5
                cmp     Last_FX,0
                jz      short ZU3
                lea     si,Left_tab
                cmp     S&D_line,0
                jz      short Sj4ok
                lea     si,Left_tab2
Sj4ok:
                call    PD_FX
                jmp     short FX3
ZU3:
                mov     al,''          ;'-'
                cmp     S&D_line,0
                jz      short FX3
                mov     al,''          ;'='
FX3:
                mov     ah,3            ; ·½Ïò×ó
                call    Ascok_call1
                jmp     Ctrl_C_l
New_j5:
                cmp     al,Alt_Right
                jnz     short New_j6
                cmp     Last_FX,0
                jz      short ZU4
                lea     si,Right_tab
                cmp     S&D_line,0
                jz      short Sj5ok
                lea     si,Right_tab2
Sj5ok:
                call    PD_FX
                jmp     short FX4
ZU4:
                mov     al,''          ;'-'
                cmp     S&D_line,0
                jz      short FX4
                mov     al,''          ;'='
FX4:
                mov     ah,4            ; ·½ÏòÓÒ
                call    Ascok_call1
                jmp     Goright
New_j6:
                cmp     al,Alt_Del
                jnz     short New_j7
                jmp     short al_d_c
New_j7:
                cmp     al,Alt_Ins
                jnz     short New_j8
                xor     cs:S&D_line,0ffh
al_d_c:
                mov     cs:Last_ZF,0    ; ÉÏ»ØÊ¹ÓÃ×Ö·û
                mov     cs:Last_FX,0    ; ÉÏ»Ø½áÊøÊ±·½Ïò
                jmp     editbegin
New_j8:
                Cmp     Al,Cross_Up
                Jnz     Nextk2
                call    test_shift
                jz      short C_Up_Call
                cmp     Block,2
                jz      short hv_block
                mov     Block,2
newblock:
                mov     ax,FileRow
                or      ax,ax
                jz      short C_Up_Call
                dec     ax
                mov     Blockh,ax
                mov     Blocke,ax
shift_blb:
                call    ckok
                jmp     C_Up_Call
hv_block:
                mov     ax,Blocke
                inc     ax
                cmp     ax,FileRow
                jnz     short notupb
                mov     ax,FileRow
                dec     ax
                cmp     ax,Blockh
                jbe     short desb
                dec     ax
                mov     Blocke,ax
                jmp     shift_blb
desb:
                mov     block,0
                jmp     shift_blb
notupb:
                mov     ax,Blockh
                cmp     ax,FileRow
                jnz     short newblock
                mov     ax,FileRow
                or      ax,ax
                jz      short C_Up_Call
                dec     ax
                mov     Blockh,ax
                jmp     shift_blb
C_Up_Call:      Mov     Move,1
                Call    Upc
                Mov     Move,0
                Jmp     Editbegin
Nextk2:
                Cmp     Al,Cross_Down
                Jnz     Nextk3
                call    test_shift
                jz      short C_Dn_Call
                cmp     Block,2
                jz      hv_block1
                mov     ax,FileRow
                inc     ax
                cmp     ax,FileRowt
                jae     short C_Dn_Call
                mov     Block,2
new_kbl:
                mov     ax,FileRow
                mov     Blockh,ax
                mov     Blocke,ax
shift_blk:
                call    ckok
                jmp     C_Dn_Call
hv_block1:
                mov     ax,FileRow
                cmp     ax,Blockh
                jnz     short notdnb
                inc     ax
                cmp     ax,Blocke
                ja      short desk
                mov     Blockh,ax
                jmp     shift_blk
desk:
                mov     block,0
                jmp     short shift_blk
notdnb:
                mov     ax,FileRow
                inc     ax
                cmp     ax,FileRowt
                jae     short C_Dn_Call
                dec     ax
                dec     ax
                cmp     ax,Blocke
                jnz     short new_kbl
                inc     ax
                mov     Blocke,ax
                jmp     shift_blk
C_Dn_Call:
                Mov     Move,1
                Call    Downc
                Mov     Move,0
                Jmp     Editbegin
Nextk3:
                Cmp     Al,Cross_Left
                Jnz     Nextk4
Ctrl_C_l:       Call    Scif
                Cmp     Iffast,1
                Jz      Englok
                Call    CmpCurasc
                Jb      Englok
                Call    CorectSi
                Jbe     Englok
                Call    Si_dec
                Call    Scif
Englok:         Jmp     Editbegin
Nextk4:
                Cmp     Al,Cross_Right
                Jnz     Nextk5
Goright:        Mov     Al,Col
                Cmp     Al,EditWide
                Jz      Downline
                Inc     Al
Nt6:            Mov     Col,Al
                Cmp     Iffast,0
                Jnz     Engrok
                Cmp     IfChr1,0        ; ÈôÊÇ×Ö·ûÏÔÊ¾ºóµÄ¹â±êÓÒÒÆ, ²»´¦Àí
                Jnz     Engrok
                Push    Ax
                Call    CmpCurasc
                Pop     Ax
                Jb      Engrok
                Cmp     BYTE PTR Es:[Si-1],0a1h
                Jb      Engrok
                Cmp     Al,EditWide
                Jz      Downline
                Inc     Al
                Mov     Col,Al
Engrok:         Jmp     SHORT Rightr
Downline:       Call    Retnspc
                Mov     Ax,FileRow
                Inc     Ax
                Cmp     Ax,FileRowt
                Jae     Rightr
                Mov     Ah,Row
                Cmp     Ah,EditHigh
                Jb      Donn
                Mov     Col,0
                Call    Downc
                Jmp     Editbegin
Donn:           Inc     Ah
                Inc     FileRow
                Xor     Al,Al
                Mov     Row,Ah
                Jmp     SHORT Nt6
Rightr:         Jmp     Editbegin
Nextk5:
                Cmp     Al,Page_up
                Jnz     Nextk6
Pg_Up_Call:     Call    RetnspcNosh
                Mov     Ax,WinRow
                Cmp     Ax,0
                Jz      Rightr
                Mov     Cl,EditHigh
                Sub     Cl,4
                Xor     Ch,Ch
                Cmp     Ax,Cx
                Ja      Nxt1
                Xor     Ax,Ax
                Jmp     SHORT Nxt2
Nxt1:           Sbb     Ax,Cx
Nxt2:           Mov     WinRow,Ax
                Jmp     Shed_old
Pupr:           Jmp     Editbegin
Nextk6:
                Cmp     Al,Page_down
                Jnz     Nextk7
Pg_Dn_Call:     Call    RetnspcNosh
                Mov     Ax,FileRowt
                Sbb     Ax,WinRow
                Mov     Cl,EditHigh
                Xor     Ch,Ch
                Cmp     Ax,Cx
                Jb      Pupr
                Mov     Ax,WinRow
                Sub     Cl,4
                Add     Ax,Cx
                Jmp     SHORT Nxt2
Nextk7:
                Cmp     Al,Ins
                Jnz     Nextk8
                call    test_shift
                jz      short noshiftins
                mov     al,ALT_C
                jmp     Nextk8
noshiftins:
                Mov     Al,1
                Sbb     Al,inswp
                Mov     Inswp,Al
                mov     byte ptr if_color,0
                Call    Shins
                mov     byte ptr if_color,1
                Jmp     Editbegin
Nextk8:
                Cmp     Al,Del
                Jnz     Nextk9
                call    test_shift
                jz      short nodelblk
                mov     al,ALT_Y        ; SHIFT+DEL ¿éÉ¾³ý
                jmp     Nextk9
nodelblk:
                Mov     Dlif,1
                Call    Calps1
                Mov     Es,Siseg
Dinds:          Mov     Al,Es:[Si]
                Push    Ax
                Call    DelCurChr
                Pop     Ax
                Cmp     Al,0dh          ; É¾³ý»Ø³µÊÇ
                Jz      Sojinok
                Cmp     Al,' '          ; É¾³ý¿Õ¸ñÊÇ
                Jnz     Delrok          ; ÆäËû·µ»Ø
Sojinok:        Cmp     Insdent,0
                Jz      Delrok
                Call    Calps1
                Mov     Es,Siseg
                Cmp     BYTE PTR Es:[Si],' '
                Jz      Dinds
Delrok:         Xor     Al,Al
                Call    StorDelChr
                Mov     Dlif,0
                Jmp     GoNs&Ed
Nextk9:
                Cmp     Al,Hmk
                Jnz     Nextk10
Hmk_Call:       Mov     Ax,FileRow
                Push    Ax
                Call    GotoLineBegin
                Call    StFileRow&Col
                Pop     Bx
                Call    RowBackUp
                Jmp     Editbegin
Nextk10:
                Cmp     Al,Edk
                Jnz     Nextk11
Edk_Call:
                Mov     Ax,FileRow
                Push    Ax
                Call    GotoLineEnd
                Call    StFileRow&Col
                Pop     Bx
                Call    RowForwDn
                Mov     Al,EditWide
                Mov     Col,Al
                Call    Nxcuc           ; Ö¸Ïò±¾ÐÐ×îºóÒ»¸ö·Ç¿Õ¸ñ×Ö·û
EdOOO:          Jmp     Editbegin
Nextk11:
                Cmp     Al,Ctrl_pu
                Jnz     Nextk12
Ctrl_pu_Call:   Cmp     WinRow,0
                Jz      EdOOO
                Call    RetnspcNosh
                Mov     WinRow,0
                Jmp     Shed_old
Nextk12:
                Cmp     Al,Ctrl_pd
                Jnz     Nextk13
Ctrl_pd_Call:   Call    RetnspcNosh
                Mov     Ax,FileRowt
                Sub     Ax,WinRow
                Mov     Cl,EditHigh
                Xor     Ch,Ch
                Cmp     Ax,Cx
                Jb      Stbr
                Mov     Ax,FileRowt
                Dec     Cl
                Sbb     Ax,Cx
                Mov     WinRow,Ax
;                Call    To_endl         ; ÈôÖ´ÐÐ´ËÐÐ,Ôò¹â±êµ½×îÄ©ÐÐ
                Jmp     Shed_old
Nextk13:
                Cmp     Al,Shift_tab
                Jnz     Nextk14
                Mov     Al,Col
                Mov     Col,0
                Cmp     Al,9
                Jb      Stbr
                Sub     Al,9
                Call    Caltab          ; ¼ÆËã TAB ÖÆ±íÎ»ÖÃ -> AL
                Mov     Col,Al
                Inc     Col
Stbr:           Jmp     Editbegin
Nextk14:
                Cmp     Al,Ctrl_home
                Jnz     Nextk15
Ctrl_Home_Call: Call    RetnspcNosh
                Mov     Row,2
                Mov     Ax,WinRow
                Mov     FileRow,Ax
                Jmp     Editbegin
Nextk15:
                Cmp     Al,Ctrl_end
                Jnz     Nextk16
Ctrl_End_Call:  Call    RetnspcNosh
                Call    To_endl
                Jmp     Editbegin
Nextk16:
                Cmp     Al,Ctrl_left
                Jnz     Nextk17
Ctrl_A_Call:    Call    RetnspcNosh
                Mov     Ax,FileRow
                Push    Ax
                Call    Calps1
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Call    GotoPrevWord
;PreCall:
                Call    StFileRow&Col
                Pop     Bx
                Call    RowBackUp
EdBeok:         Jmp     EditBegin
Nextk17:
                Cmp     Al,Ctrl_Right
                Jnz     Nextk18
Ctrl_F_Call:    Mov     Ax,FileRow
                Push    Ax
                Call    Calps1
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,0dh
                Jz      Jpns2r
                Cmp     Al,1ah
                Jz      Jpns1r
                Call    GotoNextWord
                Call    StFilerow&Col
Jpns1r:
                Pop     Bx
                Call    RowForwDn
                Jmp     EditBegin
Jpns2r:         Call    RetnspcNosh
                Inc     FileRow
                Mov     Col,0
                Pop     Bx
                Call    RowForwDn
                Mov     Ax,FileRow
                Push    Ax
                Call    Calps1
                Mov     Es,Siseg
                Mov     Al,Es:[Si]
                Cmp     Al,'?'
                Ja      Jpns1r
                Cmp     Al,'9'
                Ja      Ctrl_F_call
                Cmp     Al,'0'
                Jae     Jpns1r
                pop     bx
                Jmp     SHORT Ctrl_F_call
Nextk18:
                Cmp     Al,F10
                Jnz     Nextk19
                Mov     Al,1
                Sbb     Al,ShowCr
                Mov     ShowCr,Al
Noclss:         Mov     Ax,FileRow
                PuSh    Ax
                Mov     Ax,WinRow
                Mov     ShWinr,Ax
                Mov     Ch,2
                Call    Showcall
                Pop     Ax
                Mov     FileRow,Ax
                Jmp     Editbegin
Nextk19:
                Cmp     Al,Alt_sub
                Jnz     Nextk19a
                Mov     Al,Lv
                Inc     Al
                And     Al,0fh
                And     Lv,0f0h
                Or      Lv,Al
ChgColr:        Mov     Editing,0
                Jmp     SHORT Noclss
Nextk19a:
                Cmp     Al,Alt_equ
                Jnz     Nextk19b
                Mov     Al,Lv
                And     Al,0f0h
                Add     Al,10h
                And     Lv,0fh
                Or      Lv,Al
                Jmp     SHORT ChgColr
Nextk19b:
                cmp     al,Alt_dp
                jnz     short Nextk20
                xor     psp_color,0ffh
                jmp     short chgcolr
Nextk20:
                Cmp     Al,F4
                Jnz     Nextk21
                Mov     CtrlQA,0        ; ³õÊ¼»¯ CtrlQA ¼° CtrlQF Îª 0
                Mov     CtrlQF,0        ; ÒÔÃâÓ°Ïì F4 µÄ¹¦ÄÜ
                Jmp     Find&Replace
Nextk21:
                Cmp     Al,F5
                Jnz     Nextk22
                Call    PickDefin
Pikrt:          Jmp     Editbegin
Nextk22:
                Cmp     Al,F3
                Jnz     Nextk23
                Cmp     Block,2         ; ÎÞ¿é¶¨Òå,×ª¶ÁÈë¿é
                Jnz     ReadBlockFile
F3_Save:        Mov     Al,EditWide
                Mov     Pianec,Al       ; ½«¿é¶¨ÒåÎªÆ¬
                Mov     Pianhc,0
                Mov     Ax,Blockh
                Mov     Pianhr,Ax
                Mov     Ax,Blocke
                Mov     Pianer,Ax
                Mov     Pian,0
                Mov     Al,4           ; µ÷ÓÃÆ¬´æÅÌ
                Jmp     Pick_Call
ReadBlockFile:  Cmp     Warning,0
                Jnz     Pikrt
                Call    Setpos_dh
                Mov     Si,OFFSET Read_File_Name
                Call    Inputname
                Cmp     Al,Esca
                Jz      Pikrt
                Or      Ch,Ch
                JZ      Pikrt
                Jmp     BlockRead       ; ×ª¶ÁÈë¿é
Nextk23:
                Cmp     Al,F8
                Jnz     Nextk24
F8_Call:        Call    BlockDefin
                Jmp     Editbegin
Nextk24:
                Cmp     Al,F6
                Jnz     Nextk25
                Jmp     PickOpra
Nextk25:
                Cmp     Al,F9
                Jnz     Nextk26
                Mov     Ax,FileRow
                Push    Ax
                Mov     Al,Col
                Mov     Ah,Row
                Push    Ax
                Call    Print_File      ; ´òÓ¡ÎÄ¼þ
                Pop     Ax
                Mov     Col,Al
                Mov     Row,Ah
                Pop     Ax
                Mov     FileRow,Ax
                Jmp     Editbegin
Nextk26:
                Cmp     Al,F2
                Jnz     Nextk27
F2_Call:        Mov     Undelete,0
                Call    Setpos_dh
                Mov     Si,OFFSET F2save
                mov     byte ptr if_color,0
                Call    Dispatr
                PuSh_si
                Call    Sok
                mov     ah,3eh
                mov     bx,cs:Handle
                int     21h             ; ´æÅÌÍê±Ï¹Ø±ÕÎÄ¼þ
                Mov     Ax,3d02h
                Mov     Dx,OFFSET Filename
                Int     21h             ; ÔÙÒÔ¶ÁÐ´·½Ê½´ò¿ªÎÄ¼þ
                mov     cs:handle,ax
                Pop_si
                Call    Earse
                mov     byte ptr if_color,1
                Mov     Chang,0         ; ÖÃÎ´ÐÞ¸Ä¹ý
                Mov     Newf,0          ; ÖÃÒÑ·ÇÐÂÎÄ¼þ
                Jmp     Editbegin
Nextk27:
                Cmp     Al,Shift_F9
                Jnz     Nextk28
                Mov     Al,1
                Sub     Al,IfFast
                Mov     IfFast,Al
                mov     byte ptr if_color,0
                Call    ShA&Hhz
                mov     byte ptr if_color,1
                Jmp     Editbegin
Nextk28:
                Cmp     Al,Alt_b
                Jnz     Nextk29
                Cmp     Block,2
                Jnz     Altbr
                Call    Make_Tab
                Call    Show
Altbr:          Jmp     Editbegin
Nextk29:
                Cmp     Al,Alt_G
                Jnz     Nextk30
                Cmp     Block,0         ; ÊÇ·ñÓÐ¿é¶¨Òå?
                Jz      Altbr
                Call    Retnspc
                Call    Setpos_dh
                Mov     Si,OFFSET ToporEnd
                Call    Dispatr&inkey
                Jz      Altbr
                Cmp     Al,'1'
                Jz      Topok
                Cmp     Al,'2'
                Jnz     Altbr
Botok:          Mov     Ax,Blocke
                Jmp     SHORT Gtok
Topok:          Mov     Ax,Blockh
Gtok:           Push    Ax
                Call    RetnspcNosh
                Pop     Ax
                Mov     FileRow,Ax
                Push    Ax
                Call    Calps1
                Call    Caline
                Pop     Ax
                Jmp     Gotolinenum
Nextk30:
                Cmp     Al,Alt_X
                Jnz     Nextk31
                Jmp     Quit
Nextk31:
                Cmp     Al,Alt_C
                Jnz     Nextk32
Alt_C_Call:     Cmp     Warning,0
                Jnz     Blomr
                Cmp     Block,2
                Jnz     Blomr
                Call    CalBlockHigh
                Ja      Nx100           ; µ±Ç°ÐÐÔÚ¿éÊ×Ö®ºó×ª
                Call    AddBlock
                Jmp     SHORT Nx101
Nx100:          Je      Nx101           ; ÔÚ¿ìµÄµÚÒ»ÐÐÒà¶Ô
                Cmp     Ax,Blocke
                Jbe     Blomr
Nx101:          Call    BlockCopy
                Jmp     GoNs&Ed
Nextk32:
                Cmp     Al,Alt_Y
                Jnz     Nextk33
Alt_Y_Call:     Cmp     Block,2
                Jnz     Blomr
                Call    BlockSTD
                Call    BlockDel
                mov     al,Cure_find_pos
                mov     Row,al
                Jmp     Shed_old
Nextk33:
                Cmp     Al,Alt_V
                Jnz     Nextk34
Alt_V_Call:     Cmp     Warning,0
                Jnz     Blomr
                Cmp     Block,2
                Jnz     Blomr                   ; ÎÞ¿é¶¨Òå, ·µ»Ø
                Call    BlockMove
Blomr:          Jmp     Editbegin
Nextk34:
                Cmp     Al,Alt_Q
                Jnz     Nextk35
                Cmp     Block,2
                Jz      YesBlock        ; ÓÐ¿é¶¨Òå×ª
                Cmp     Pian,2
                Jnz     Cqr1            ; ÎÞ¿éÎÞÆ¬×ª
IsPian:         Mov     Pian,0
                Jmp     SHORT CtrQokr
YesBlock:       Cmp     Pian,2
                Jz      Pian&Block      ; ÓÐ¿éÓÐÆ¬×ª
IsBlock:        Mov     Block,0
CtrQokr:        Call    Show
Cqr1:           Jmp     Editbegin
Pian&Block:     Call    Setpos_dh
                Mov     Si,OFFSET Pian_or_Block
                Call    Dispatr&inkey
                Jz      Cqr1
                Cmp     Al,'1'
                Jz      IsPian
                Cmp     Al,'2'
                Jz      IsBlock
                Jmp     SHORT Cqr1
Nextk35:
                Cmp     Al,F7
                Jnz     Nextk36
                Jmp     Ctrl_U_Call
Nextk36:
                Cmp     Al,F1
                Jnz     Nextk37
                Call    Help
                Jmp     EditBegin
Nextk37:
                Jmp     EDITBEGIN
