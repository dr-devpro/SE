; ������������������������������������
; �� �ļ���   : SE_DATA.INC         ��
; �� �汾��   : 2.3                 ��
; �� ��  ��   : ��                ��
; �� ��  ��   : 1991,2 - 92,8       ��
; ��                                ��
; �� ��Ȩ���� (��)Starsoft Corp,.   ��
; �� Allright reserved              ��
; ������������������������������������
;
; ���ļ������� SE ���õ���ȫ������
; --------------------------------

; ��ɫֵ
Lv1             DB      37h
Menuc           DB      3fh             ; �Ի�����ɫ
Onc             DB      3eh             ; ״̬����ɫ
Offc            DB      3bh             ; ״̬����ɫ
Linec           DB      3eh             ; �к���ɫ

; ����ָ��
VBUF            dw      0b800h
screenwide      db      80              ; ��ǰ��Ļ����
Work_Mark       DB      30      Dup(0)  ; 10��������,ÿ��ռ3�ֽ�(��/��)
CtrlQF          DB      0
CtrlQA          DB      0
Tab_Graph       DB      "����������������������������",0       ; ACIOS ���ֽ��Ʊ���
Tab_9           DB      '���������������ȩةЩ੥�����������ǩϩߩש况�������ө۩��'
Left_Mark       DB      0               ; ��߽�ֵ
F1_F10          DB      'F1',0,'F2',0,'F3',0,'F4',0,'F5',0
                DB      'F6',0,'F7',0,'F8',0,'F9',0,'F10',0
F1_F10_Col      DB      0,7,14,23,34,41,50,57,64,71
Se_Cfg          DB      'C:\XE.CFG',0   ; �����ļ���
Inswp           DB      1               ; ���뿪��
Insdent         DB      1               ; ��������
PrintLine       DW      0               ; ��ӡ��������
Chi             DB      0               ; �Կ���
Jia             DB      0               ; �ӿ���
MoXW            DB      ',.;:!?%)]}>',0                  ; �Ű�������ĩ�ַ�
MoHZ            DB      '���������������������������ݣ�' ; �Ű�������ĩ�ַ�
                DB      '��������������������',0
Move            DB      0
AltBIf          DB      0
Undelete        DW      0               ; �ָ�ɾ��ָ��
DLif            DB      0               ; �Ű�ʱ, ��
PaiBan          DB      0               ; �Ű�ʱ, ��1����ɾ�����ַ�����ɾ������
Dcont           DB      0               ; �лس�����
Iffast          DB      0               ; ����ȫ/���
Warning         DB      0               ; �ڴ治�������־
HanZi           DB      0
IfChr           DB      0               ; ����ȫ���б���
IfChr1          DB      0
Fal0a           DB      0               ; �з�ʽ�༭��� (1=��)
Fileseg         DW      0               ; �ļ���/д�������ε�ַ = Ds+1000h
Filesto         DW      0               ; �༭��������ʼ�ε�ַ = Ds+2000h
Handle          DW      0               ; ���ļ����
Filename        DB      30      DUP(0)  ; �༭�ļ���
Filesh          DW      0               ; �ļ����ȸ�16λ
Filesl          DW      0               ; �ļ����ȵ�16λ
Row             DB      0               ; ��Ļ��
Col             DB      0               ; ��Ļ��
FileRowt        DW      0               ; �ļ�ת����������
FileRowtOld     DW      0               ; ��ɾ��ʱ�ļ������е��ݴ洦
FileRow         DW      0               ; ��ǰ��������ļ���
WinRow          DW      0               ; ��Ļ���ڵ�һ�������ļ���
Blockh          DW      0               ; �����ļ���
Block           DB      0               ; ����
Blocke          DW      0               ; ��β�ļ���
Blockd          DB      0
Pianhr          DW      0               ; Ƭ���ļ���
Pianhc          DB      0               ; Ƭ����Ļ��
Pianer          DW      0               ; Ƭβ�ļ���
Pianec          DB      0               ; Ƭβ��Ļ��
Pian            DB      0               ; Ƭ���
Co              DB      0
Rw              DB      0
Attr            DB      0               ; �ַ����� ( To_Scr �ӳ�����)
Cr              DB      '<'
Cr1             DB      '\'
Eof             DB      'E'
FalOk           DB      '!'
Siseg           DW      0
Diseg           DW      0
Bpbuf           Dw      0
Readsize        DW      0               ; ��д���������ȴ��
Newf            DB      0               ; ���ļ����
Mode            DB      0               ; ��ǰ��ʾҳ
Chang           DB      0               ; �ļ��Ƿ��޸Ĺ����
Last            DB      0               ; ��ĩ�ַ��Ĵ�
ShWinr          DW      0
Crif            DB      0
Ll              DB      0
Crif1           DB      0
Lines           DB      4       DUP(30h); ��ʾ�кżĴ���
Binval          DW      0
Mult10          DW      1
Cap             DB      0               ; Ƭ����ѡ����ݴ�
IfFind          DB      0               ; ����ʱ�Ƿ��з���
IfrePl          DW      0               ; �滻ʱ�Ƿ����滻
Finded          DB      0               ; �������ҵ���, ����ҵ��ִ��ĳ���
RePled          DB      0               ; �������滻��, ����滻�ִ��ĳ���
Uorl            DB      0               ; �Ƿ���Դ�Сд
Ask             DB      0               ; �滻ʱ�Ƿ�����
Inblck          DB      0               ; �Ƿ���ڲ���
Inschr          DB      0               ; �����ַ��Ĵ�
Edind           DB      0               ; �����ո����
Editing         DB      0
Isnew           DB      1
FindBuf         DB      31      DUP(0)  ; �����ִ�������
ReplBuf         DB      31      DUP(0)  ; �滻�ִ�������
FanBuf          DB      6       DUP(0)  ; ���ҷ�ʽ�����뻺��
Gotobuf         DB      5       DUP(0)  ; ת���к����뻺��
PianFilename    DB      26      DUP(0)  ; Ƭ���̼�������ļ��������
HandlePian      DW      0               ; �����ļ����
Pianbuf         DB      0               ; Ƭ����һ�ֽڻ�����
Crsave          DB      0
EachCircle      DW      0               ; 27 ��������ÿ�������Ŀ��ÿռ����
Oldmem          DB      0               ; ÿ�αȽ�ǰ�ľɿռ����
if_SPT          db      0
no_P48          db      0
cure_top        dw      0

SPT_head   db   53h,75h,70h,65h,72h,2Dh,53h,74h,61h,72h,20h,46h,69h,6Ch,65h,1Ah
           db   00h,01h,00h,00h,00h,00h,00h,00h,00h,00h,00h,00h,61h,62h,63h,64h
           db   40h,00h
spt_wide   dw   0
spt_high   dw   0
           db   01h,00h,00h,00h,00h,00h,00h,00h,00h,00h
           db   00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00

; ���̰���
Ins             EQU     82
Del             EQU     83
Esca            EQU     27              ; Ctrl_[
Hmk             EQU     71
Edk             EQU     79
Page_Up         EQU     73
Page_Down       EQU     81
Cross_Up        EQU     72
Cross_Down      EQU     80
Cross_Left      EQU     75
Cross_Right     EQU     77
Enter           EQU     13
Tab             EQU     9
Shift_Tab       EQU     15
Backspace       EQU     8
Ctrl_Home       EQU     119
Ctrl_End        EQU     117
Ctrl_Pu         EQU     132
Ctrl_Pd         EQU     118
Ctrl_Left       EQU     115
Ctrl_Right      EQU     116
Ctrl_Up         EQU     8Dh
Ctrl_Down       EQU     91h
Ctrl_a          EQU     1
Ctrl_b          EQU     2
Ctrl_c          EQU     3
Ctrl_d          EQU     4
Ctrl_e          EQU     5
Ctrl_f          EQU     6
Ctrl_g          EQU     7
Ctrl_k          EQU     11
Ctrl_l          EQU     12
Ctrl_n          EQU     14
Ctrl_p          EQU     16
Ctrl_q          EQU     17
Ctrl_r          EQU     18
Ctrl_s          EQU     19
Ctrl_t          EQU     20
Ctrl_u          EQU     21
Ctrl_v          EQU     22
Ctrl_w          EQU     23
Ctrl_x          EQU     24
Ctrl_y          EQU     25
Ctrl_z          EQU     26
Ctrl_bs         EQU     28              ; Ctrl \
Ctrl_cb         EQU     29              ; Ctrl ]
Ctrl_Backspace  EQU     127
F10             EQU     68
F9              EQU     67
F8              EQU     66
F7              EQU     65
F6              EQU     64
F5              EQU     63
F4              EQU     62
F3              EQU     61
F2              EQU     60
F1              EQU     59
Shift_F9        EQU     92
Alt_B           EQU     48
Alt_C           EQU     46
Alt_G           EQU     34
Alt_M           EQU     50
Alt_Q           EQU     16
Alt_V           EQU     47
Alt_X           EQU     45
Alt_Y           EQU     21
Alt_sub         EQU     130             ; Alt -
Alt_equ         EQU     131             ; Alt =
Alt_Left        EQU     9Bh
Alt_Right       EQU     9Dh
Alt_Up          EQU     98h
Alt_Down        EQU     0A0h
Alt_Del         EQU     0A3h
Alt_Ins         EQU     0A2h
Alt_dp          EQU     029h

INCLUDE         Se_Masg.inc             ; ��ʾ��Ϣ
