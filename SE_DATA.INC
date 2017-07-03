; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   : SE_DATA.INC         ┃
; ┃ 版本号   : 2.3                 ┃
; ┃ 作  者   : 简晶                ┃
; ┃ 日  期   : 1991,2 - 92,8       ┃
; ┃                                ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved              ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括了 SE 所用到的全部数据
; --------------------------------

; 颜色值
Lv1             DB      37h
Menuc           DB      3fh             ; 对话栏颜色
Onc             DB      3eh             ; 状态开颜色
Offc            DB      3bh             ; 状态关颜色
Linec           DB      3eh             ; 行号颜色

; 各种指针
VBUF            dw      0b800h
screenwide      db      80              ; 当前屏幕宽度
Work_Mark       DB      30      Dup(0)  ; 10个工作点,每个占3字节(列/行)
CtrlQF          DB      0
CtrlQA          DB      0
Tab_Graph       DB      "爛亗儓墣帉焺厗噴嫅弽敃枟槞搾",0       ; ACIOS 单字节制表符
Tab_9           DB      '─│┌┐└┘├┤┴┬┼━┃┏┓┗┛┣┫┻┳╋┍┑┕┙┯┷┿╂'
Left_Mark       DB      0               ; 左边界值
F1_F10          DB      'F1',0,'F2',0,'F3',0,'F4',0,'F5',0
                DB      'F6',0,'F7',0,'F8',0,'F9',0,'F10',0
F1_F10_Col      DB      0,7,14,23,34,41,50,57,64,71
Se_Cfg          DB      'C:\XE.CFG',0   ; 配置文件名
Inswp           DB      1               ; 插入开关
Insdent         DB      1               ; 缩进开关
PrintLine       DW      0               ; 打印行数计数
Chi             DB      0               ; 吃空数
Jia             DB      0               ; 加空数
MoXW            DB      ',.;:!?%)]}>',0                  ; 排版西文行末字符
MoHZ            DB      '，。；：、．？！’”％）＞］｝' ; 排版中文行末字符
                DB      '〗】〉〕》」』°′″℃‰',0
Move            DB      0
AltBIf          DB      0
Undelete        DW      0               ; 恢复删除指针
DLif            DB      0               ; 排版时, 不
PaiBan          DB      0               ; 排版时, 置1不将删除的字符放入删除缓冲
Dcont           DB      0               ; 列回车计数
Iffast          DB      0               ; 汉字全/半角
Warning         DB      0               ; 内存不够警告标志
HanZi           DB      0
IfChr           DB      0               ; 汉字全角判别标记
IfChr1          DB      0
Fal0a           DB      0               ; 列方式编辑标记 (1=是)
Fileseg         DW      0               ; 文件读/写缓冲区段地址 = Ds+1000h
Filesto         DW      0               ; 编辑缓冲区起始段地址 = Ds+2000h
Handle          DW      0               ; 主文件句柄
Filename        DB      30      DUP(0)  ; 编辑文件名
Filesh          DW      0               ; 文件长度高16位
Filesl          DW      0               ; 文件长度低16位
Row             DB      0               ; 屏幕行
Col             DB      0               ; 屏幕列
FileRowt        DW      0               ; 文件转换后总行数
FileRowtOld     DW      0               ; 块删除时文件旧总行的暂存处
FileRow         DW      0               ; 当前光标所在文件行
WinRow          DW      0               ; 屏幕窗口第一行所在文件行
Blockh          DW      0               ; 块首文件行
Block           DB      0               ; 块标记
Blocke          DW      0               ; 块尾文件行
Blockd          DB      0
Pianhr          DW      0               ; 片首文件行
Pianhc          DB      0               ; 片首屏幕列
Pianer          DW      0               ; 片尾文件行
Pianec          DB      0               ; 片尾屏幕列
Pian            DB      0               ; 片标记
Co              DB      0
Rw              DB      0
Attr            DB      0               ; 字符属性 ( To_Scr 子程序用)
Cr              DB      '<'
Cr1             DB      '\'
Eof             DB      'E'
FalOk           DB      '!'
Siseg           DW      0
Diseg           DW      0
Bpbuf           Dw      0
Readsize        DW      0               ; 读写缓冲区长度存放
Newf            DB      0               ; 新文件标记
Mode            DB      0               ; 当前显示页
Chang           DB      0               ; 文件是否修改过标记
Last            DB      0               ; 行末字符寄存
ShWinr          DW      0
Crif            DB      0
Ll              DB      0
Crif1           DB      0
Lines           DB      4       DUP(30h); 显示行号寄存区
Binval          DW      0
Mult10          DW      1
Cap             DB      0               ; 片操作选择号暂存
IfFind          DB      0               ; 查找时是否有发现
IfrePl          DW      0               ; 替换时是否有替换
Finded          DB      0               ; 若曾经找到过, 存放找到字串的长度
RePled          DB      0               ; 若曾经替换过, 存放替换字串的长度
Uorl            DB      0               ; 是否忽略大小写
Ask             DB      0               ; 替换时是否提问
Inblck          DB      0               ; 是否块内查找
Inschr          DB      0               ; 插入字符寄存
Edind           DB      0               ; 缩进空格个数
Editing         DB      0
Isnew           DB      1
FindBuf         DB      31      DUP(0)  ; 查找字串缓冲区
ReplBuf         DB      31      DUP(0)  ; 替换字串缓冲区
FanBuf          DB      6       DUP(0)  ; 查找方式字输入缓冲
Gotobuf         DB      5       DUP(0)  ; 转向行号输入缓冲
PianFilename    DB      26      DUP(0)  ; 片存盘及块读盘文件名存放区
HandlePian      DW      0               ; 交换文件句柄
Pianbuf         DB      0               ; 片存盘一字节缓冲区
Crsave          DB      0
EachCircle      DW      0               ; 27 个方块中每个代表的可用空间节数
Oldmem          DB      0               ; 每次比较前的旧空间节数
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

; 键盘按键
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

INCLUDE         Se_Masg.inc             ; 提示信息
