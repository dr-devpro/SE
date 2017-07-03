; File: SE_MTAB.INC
; =================
; Íê³ÉÊÖ¶¯ÖÆ±í¹ı³Ì, °üÀ¨µ¥Ë«Ïß. °´ ALT ¡ú¡û¡ü¡ı ½øĞĞ
; --------------------------------------------------


S&D_line        db      0       ; µ¥Ë«Ïß¿ª¹Ø
Last_ZF         db      0
Last_FX         db      0       ; 1,2,3,4
ZF_tab          db      ' €‚ƒˆ‰Œ'

PM_tab          db      'ŒŒŒŒ'
                db      'Œ ˆ‰ˆ‰ˆ‰ŒŒŒ'
                db      'ˆ€ˆŒˆŒŒŒ'
                db      '‰Œ‰Œ‰ŒŒ'
                db      'ˆˆŒ‚ˆŒŒŒ'
                db      '‰Œ‰ƒŒ‰ŒŒ'
                db      'ŒˆˆŒˆŒˆŒŒŒŒ'
                db      'Œ‰Œ‰Œ‰Œ‰ŒŒŒ'
                db      'ŒŒŒŒŒŒŒ'
                db      'ŒŒŒŒŒŒŒ'
                db      'ŒŒŒŒŒŒŒŒŒŒŒ'

Up_tab          db      '  ‚ƒ'
                db      '    '
                db      ' ˆ ƒ'
                db      ' ‰‚ '
                db      '   ƒ'
                db      '  ‚ '
                db      '   ƒ'
                db      '  ‚ '
                db      '  ‚ƒ'
                db      '  ‚ƒ'
                db      '  ‚ƒ'

Down_tab        db      '  €'
                db      '    '
                db      '   '
                db      '  € '
                db      'ˆ  '
                db      '‰ € '
                db      '   '
                db      '  € '
                db      '  €'
                db      '  €'
                db      '  €'

Left_tab        db      '  '
                db      'ƒ  '
                db      ' ƒ '
                db      ' ƒ '
                db      '  '
                db      '  '
                db      'ƒ '
                db      'ƒ '
                db      ' '
                db      ' ƒ'
                db      'ƒ'

Right_tab       db      '  '
                db      '€‚  '
                db      ' ‚ '
                db      ' ‚ '
                db      '€  '
                db      '€  '
                db      '€‚ '
                db      '€ˆ '
                db      '€ '
                db      ' ‚'
                db      '€‚'

ZF_tab2         db      'Ÿ„…†‡Š‹‘'

PM_tab2         db      '‘‘‘'
                db      'ŸŠ‹Š‹Š‹'
                db      'Š„ŠŠ'
                db      '‹…‹‹'
                db      '‘ŠŠ†‘Š‘'
                db      '‘‹‹‘‡‹‘'
                db      'ŠŠŠŠ'
                db      '‹‹‹‹'
                db      '‘‘‘‘'
                db      ''
                db      ''

Up_tab2         db      '  †‡'
                db      'ŸŸ  '
                db      ' Š ‡'
                db      ' ‹† '
                db      'Ÿ  ‡'
                db      'Ÿ † '
                db      'ŸŸ ‡'
                db      'ŸŸ† '
                db      'Ÿ †‡'
                db      ' Ÿ†‡'
                db      'ŸŸ†‡'

Down_tab2       db      '  „…'
                db      'ŸŸ  '
                db      ' Ÿ …'
                db      ' Ÿ„ '
                db      'Š  …'
                db      '‹ „ '
                db      'ŸŸ …'
                db      'ŸŸ„ '
                db      'Ÿ „…'
                db      ' Ÿ„…'
                db      'ŸŸ„…'

Left_tab2       db      '  '
                db      '…‡  '
                db      ' ‡ '
                db      ' ‡ '
                db      '…  '
                db      '…  '
                db      '…‡ '
                db      '…‡ '
                db      '… '
                db      ' ‡'
                db      '…‡'

Right_tab2      db      '  '
                db      '„†  '
                db      ' † '
                db      ' † '
                db      '„  '
                db      '„  '
                db      '„† '
                db      '„Š '
                db      '„ '
                db      ' †'
                db      '„†'

Ascok_call1:
                mov     cs:Last_ZF,al      ; ÉÏ»ØÊ¹ÓÃ×Ö·û
                mov     cs:Last_FX,ah      ; ÉÏ»Ø½áÊøÊ±·½Ïò
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
                mov     cs:Last_ZF,al      ; ÉÏ»ØÊ¹ÓÃ×îÖÕµÄ×Ö·û
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