section .note.GNU-stack
section .text
global convolve_asm

; System V AMD64 ABI:
; rdi=mat, rsi=in, rdx=out, rcx=w, r8=ch, r9=deg
; Stack: [rbp+16]=sum, [rbp+24]=centre, [rbp+32]=i, [rbp+40]=j

convolve_asm:
	push	rbp							;حفظ ثبات ذخیره‌خوانده
	mov		rbp,	rsp					;انتقال ثبات برای خواندن داده‌های  ذخیره شده در پشته
	push	rbx							;حفظ ثبات ذخیره‌خوانده
	push	r12							;حفظ ثبات ذخیره‌خوانده
	push	r13							;حفظ ثبات ذخیره‌خوانده
	push	r14							;حفظ ثبات ذخیره‌خوانده
	push	r15							;حفظ ثبات ذخیره‌خوانده

	mov		r11d,	[rbp+24]			;مرکز ماتریس
	mov		r12d,	[rbp+32]			;i
	mov		r13d,	[rbp+40]			;j

	vxorps	ymm0,	ymm0,	ymm0		;صفر کردن ثبات

	mov		r14d,	r11d				;مقدار اولیه حلقهٔ عمودی کانولوشن
	neg		r14d						;از منفی مرکز تا مرکز
.loop_ki:
	mov		r15d,	r11d				;مقدار اولیه حلقهٔ افقی کانولوشن
	neg		r15d						;از منفی مرکز تا مرکز
.loop_kj:

	; Matrix Index: (centre + ki) * deg + (centre + kj)			;
	movsxd	rax,	r11d				;طبق فرمول مرکز را با گسترش بیت انتقال بده
	movsxd	rbx,	r15d				;طبق فرمول تغییرات را با گسترش بیت انتقال بده
	add		rax,	rbx					;طبق فرمول جمع بزن
	imul	rax,	r9					;طبق فرمول ضرب بزن

	movsxd	rbx,	r11d				;طبق فرمول مرکز را با گسترش بیت انتقال بده
	movsxd	r10,	r14d				;طبق فرمول تغییراات را با گسترش بیت انتقال بده
	add		rbx,	r10					;طبق فرمول جمع بزن
	add		rax,	rbx					;طبق فرمول جمع بزن

	mov		eax,	[rdi + rax*4]  		;درایه ماتریس را با فرض اینکه هر عدد ۴ بایت است بخوان

	; Image Pointer: ((j + kj) * w + (i + ki)) * ch			;
	movsxd	rbx,	r13d				;طبق فرمول جی را با گسترش بیت انتقال بده
	movsxd	r10,	r15d				;طبق فرمول تغییرات را با گسترش بیت انتقال بده
	add		rbx,	r10					;طبق فرمول جمع بزن
	imul	rbx,	rcx					;طبق فرمول ضرب در عرض کن

	movsxd	r10,	r12d				;طبق فرمول آی را با گسترش بیت انتقال بده
	add		rbx,	r10					;طبق فرمول جمع بزن
	movsxd	r10,	r14d				;طبق فرمول تغییرات را با گسترش بیت انتقال بده
	add		rbx,	r10					;طبق فرمول جمع بزن

	imul	rbx,	r8					;در تعداد کانال ضرب کن

	; SIMD Math
	vmovq	xmm4,	[rsi + rbx]			;از حافظهٔ محاسبه شده 8 بایت بخوام
	vpmovzxbd		ymm4,	xmm4		;آن ۸ بایت را  به 8 عدد 4 بایتی تبدیل کن
	vmovd	xmm9,	eax					;درایهٔ ماتریس را در اس‌اس‌ای بریز
	vpbroadcastd	ymm9,	xmm9		;درایهٔ ماتریس را ۸ بار منتشر کن
	vpmulld	ymm4,	ymm4,	ymm9		;درایهٔ ماتریس را در بایت‌ها ضرب کن
	vpaddd	ymm0,	ymm0,	ymm4		;نتایج را انباشته کن

	inc		r15d						;پلهٔ حلقهٔ افقی
	cmp		r15d,	r11d				;شرط حلقهٔ افقی
	jle		.loop_kj					

	inc		r14d						;پلهٔ حلقهٔ عمودی
	cmp		r14d,	r11d				;شرط حلقهٔ عمودی
	jle		.loop_ki					

	; Store
	mov		r10d,	[rbp+16]			;مجموع درایه‌ها را برای هنجارسازی بارگذاری کن
	vmovd	xmm4,	r10d				;جمع درایه‌ها را به اس‌اس‌ای ببر
	vpbroadcastd	ymm4,	xmm4		;جمع درایه‌ها را ۸ بار منتشر کن
	vcvtdq2ps		ymm4,	ymm4		;تبدیل به عدد اعشاری برای تقسیم
	vcvtdq2ps		ymm0,	ymm0		;تبدیل به عدد اعشاری برای تقسیم
	vdivps	ymm0,	ymm0,	ymm4		;هنجارسازی
	vcvtps2dq		ymm0,	ymm0		;تبدیل به عدد صحیح

	vpackusdw		ymm0,	ymm0,	ymm0 ;تبدیل ۸ عدد ۳۲ بیتی به ۱۶ بیتی به صورت بدون علامت. پس اعداد منفی صفر می‌شوند
	vextracti128	xmm1,	ymm0,	1	;انتقال نیمه بالایی ای‌وی‌ایکس به اس‌اس‌ای
	vpackuswb		xmm0,	xmm0,	xmm1 ;تبدیل ۸ عدد به نسخه ۸ بیتی، محدود کردن به 255

	; Output: (j * w + i) * ch
	movsxd	rax,	r13d				;طبق فرمول جی را با گسترش بیت بارگذاری کن
	imul	rax,	rcx					;طبق فرمول در عرض ضرب کن
	movsxd	rbx,	r12d				;طبق فرمول آی را با گسترش بیت اضافه کن
	add		rax,	rbx					;طبق فرمول جمع کن
	imul	rax,	r8					;طبق فرمول در تعداد کانال جمع کن

	vpextrd	[rdx + rax],	xmm0,	0	;ابتدا ۴ بایت نخست را از ۴ بایت اول اس‌اس‌ای ذخیره کن
	add		rax,	4
	vpextrd	[rdx + rax],	xmm0,	2	;سپس ۴ بایت دوم را از ۴ بایت سوم اس‌اس‌ای ذخیره کن

	pop		r15							;ثبات ذخیره‌خوانده را برگردان
	pop		r14							;ثبات ذخیره‌خوانده را برگردان
	pop		r13							;ثبات ذخیره‌خوانده را برگردان
	pop		r12							;ثبات ذخیره‌خوانده را برگردان
	pop		rbx							;ثبات ذخیره‌خوانده را برگردان
	pop		rbp							;ثبات ذخیره‌خوانده را برگردان
	ret