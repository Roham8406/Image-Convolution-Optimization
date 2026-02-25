.section .note.GNU-stack,"",@progbits
.intel_syntax noprefix

.text
.global sum_matrix
sum_matrix:

	vpxor	ymm0,	ymm0,	ymm0		#صفر کردن ثبات به جهت انباشته‌گر
	vpxor	ymm1,	ymm1,	ymm1		#صفر کردن ثبات به جهت انباشته‌گر
	vpxor	ymm2,	ymm2,	ymm2		#صفرکردن ثبات
	vpxor	ymm3,	ymm3,	ymm3		#صفر کردن ثبات
loop:
	vmovdqu	ymm2,	[rdi]				#تعداد 8 عدد ۳۲ بیتی در رجیستر بارگذاری میگردد
	vmovdqu	ymm3,	[rdi+32]			#تعداد ۸ عدد ۳۲ بیتی بعدی در رجیستر بعدی بارگذاری میگردد
	vpaddd	ymm0,	ymm0,	ymm2		#انباشته کردن
	vpaddd	ymm1,	ymm1,	ymm3		#انباشته کردن
	add		rdi,	64					#نشانه‌گر حافظه 64 بایت معادل 16 عدد ۳۲ بیتی جلو می‌رود
	sub		esi,	16					#تعداد اعداد باقی‌مانده 16 تا کم می‌شود
	jg		loop						#تا به هنگامی که رجیستر ای‌اس‌آی نامنفی است حلقه ادامه می‌یابد

	vpaddd	ymm0,	ymm0,	ymm1		#انباشته‌گرها جمع می‌شوند

	vextracti128	xmm1,	ymm0,	1	#ذخیرهٔ 128 بیت بالایی در اس‌اس‌ای
	vpaddd	xmm0,	xmm0,	xmm1		#جمع انباشته‌گرها، هم‌اکنون 4 صف مجموع در ایکس‌ام‌ام‌صفر وجود دارد

	movhlps	xmm1,	xmm0				#ذخیرهٔ ۲ عدد بالایی در رجیستر دوم
	paddd	xmm0,	xmm1				#انباشته‌کردن دو رجیستر
	pshufd	xmm1,	xmm0,	1			#ذخیرهٔ عدد دوم در رجیستر دوم
	paddd	xmm0,	xmm1				#انباشته کردن دو رجیستر
	movd	eax,	xmm0				#انتقال
	test	eax,	eax
	jg		positive					#اگر مثبت بود خودش را برگدن
	mov		eax,	1					#در غیر این صورت ۱ را برگردان
positive:
	ret
