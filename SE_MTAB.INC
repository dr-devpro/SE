; File: SE_MTAB.INC
; =================
; 完成手动制表过程, 包括单双线. 按 ALT →←↑↓ 进行
; --------------------------------------------------


S&D_line        db      0       ; 单双线开关
Last_ZF         db      0
Last_FX         db      0       ; 1,2,3,4
ZF_tab          db      '爛亗儓墣帉'

PM_tab          db      '寧帎悓寪帉'
                db      '尃垑垑垑寣�'
                db      '巿�巿寛寣帉'
                db      '帀巵寜寜寧�'
                db      '悎垖倫垖悓�'
                db      '悏寜悆寜悓�'
                db      '寛垖垖垖寣�'
                db      '寜寜寜寜寣�'
                db      '悓寣悙寣悓�'
                db      '帉帋寣寣寧�'
                db      '寣寣寣寣寣�'

Up_tab          db      '  們'
                db      '牋  '
                db      ' � �'
                db      ' 墏 '
                db      '�  �'
                db      '� � '
                db      '牋 �'
                db      '牋� '
                db      '� 們'
                db      ' 爞�'
                db      '牋們'

Down_tab        db      '  ��'
                db      '牋  '
                db      ' � �'
                db      ' 爛 '
                db      '�  �'
                db      '� � '
                db      '牋 �'
                db      '牋� '
                db      '� ��'
                db      ' 爛�'
                db      '牋��'

Left_tab        db      '  '
                db      '亙  '
                db      ' � '
                db      ' � '
                db      '�  '
                db      '�  '
                db      '亙 '
                db      '亙 '
                db      '� '
                db      ' �'
                db      '亙'

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

ZF_tab2         db      '焺厗噴嫅弽'

PM_tab2         db      '崗彂憤崙弽'
                db      '崯妺妺妺崓�'
                db      '強剰妽妽崗�'
                db      '弸弲崑崑崗�'
                db      '憡妽啈妽憤�'
                db      '憢崑憞崑憤�'
                db      '崐妽妽妽崓�'
                db      '崑崑崑崑崓�'
                db      '憤崓憫崓憤�'
                db      '弽弿崓崓崗�'
                db      '崓崓崓崓崓�'

Up_tab2         db      '  唶'
                db      '煙  '
                db      ' � �'
                db      ' 媶 '
                db      '�  �'
                db      '� � '
                db      '煙 �'
                db      '煙� '
                db      '� 唶'
                db      ' 焼�'
                db      '煙唶'

Down_tab2       db      '  剠'
                db      '煙  '
                db      ' � �'
                db      ' 焺 '
                db      '�  �'
                db      '� � '
                db      '煙 �'
                db      '煙� '
                db      '� 剠'
                db      ' 焺�'
                db      '煙剠'

Left_tab2       db      '  '
                db      '厙  '
                db      ' � '
                db      ' � '
                db      '�  '
                db      '�  '
                db      '厙 '
                db      '厙 '
                db      '� '
                db      ' �'
                db      '厙'

Right_tab2      db      '  '
                db      '剢  '
                db      ' � '
                db      ' � '
                db      '�  '
                db      '�  '
                db      '剢 '
                db      '剨 '
                db      '� '
                db      ' �'
                db      '剢'

Ascok_call1:
                mov     cs:Last_ZF,al      ; 上回使用字符
                mov     cs:Last_FX,ah      ; 上回结束时方向
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
                mov     cs:Last_ZF,al      ; 上回使用最终的字符
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