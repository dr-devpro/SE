; ┏━━━━━━━━━━━━━━━━┓
; ┃ 文件名   :	SE_ERRO.INC	    ┃
; ┃ 版本号   :	2.0		    ┃
; ┃ 作	 者   :	简晶		    ┃
; ┃ 日	 期   :	1991 年	12 月	    ┃
; ┃				    ┃
; ┃ 版权所有 (★)Starsoft Corp,.   ┃
; ┃ Allright reserved in ShenZhen  ┃
; ┗━━━━━━━━━━━━━━━━┛
;
; 本文件包括关于错误处理的子程序, 有以下几个:
; ---------------------------------------------------------------
; GetErrMsg  -	根据 AL	中的 DOS 错误代码, 取得相应的错误提示信息
; PianErr    -	取相应的错误信息后, 转按任意键返回
; ---------------------------------------------------------------

GETERRMSG:
		Mov	Si,OFFSET Err3	; 路径不对
		Cmp	Al,3
		Jz	Errok
		Mov	Si,OFFSET Err5	; 属性不对 ( 只读/系统 )
		Cmp	Al,5
		Jz	Errok
		Mov	Si,OFFSET Err	; 其他出错
ErrOk:		Ret

PIANERR:	Call	Geterrmsg
		Jmp	Anykeyret
