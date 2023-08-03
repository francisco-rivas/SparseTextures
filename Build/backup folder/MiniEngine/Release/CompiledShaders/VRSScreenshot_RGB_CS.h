#if 0
;
; Input signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; no parameters
;
; Output signature:
;
; Name                 Index   Mask Register SysValue  Format   Used
; -------------------- ----- ------ -------- -------- ------- ------
; no parameters
; shader hash: 57c4b3f2f59a0f360e41f54b24eeb479
;
; Pipeline Runtime Information: 
;
;
;
; Buffer Definitions:
;
;
; Resource Bindings:
;
; Name                                 Type  Format         Dim      ID      HLSL Bind  Count
; ------------------------------ ---------- ------- ----------- ------- -------------- ------
; SrcImage                              UAV     u32          2d      U0             u0     1
; DstImage                              UAV     f32          2d      U1             u1     1
;
target datalayout = "e-m:e-p:32:32-i1:32-i8:32-i16:32-i32:32-i64:64-f16:32-f32:32-f64:64-n8:16:32:64"
target triple = "dxil-ms-dx"

%dx.types.Handle = type { i8* }
%dx.types.ResRet.i32 = type { i32, i32, i32, i32, i32 }
%"class.RWTexture2D<unsigned int>" = type { i32 }
%"class.RWTexture2D<vector<float, 4> >" = type { <4 x float> }

define void @main() {
  %1 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 1, i32 1, i32 1, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %2 = call %dx.types.Handle @dx.op.createHandle(i32 57, i8 1, i32 0, i32 0, i1 false)  ; CreateHandle(resourceClass,rangeId,index,nonUniformIndex)
  %3 = call i32 @dx.op.threadId.i32(i32 93, i32 0)  ; ThreadId(component)
  %4 = call i32 @dx.op.threadId.i32(i32 93, i32 1)  ; ThreadId(component)
  %5 = call %dx.types.ResRet.i32 @dx.op.textureLoad.i32(i32 66, %dx.types.Handle %2, i32 undef, i32 %3, i32 %4, i32 undef, i32 undef, i32 undef, i32 undef)  ; TextureLoad(srv,mipLevelOrSampleCount,coord0,coord1,coord2,offset0,offset1,offset2)
  %6 = extractvalue %dx.types.ResRet.i32 %5, 0
  %7 = shl i32 %6, 4
  %8 = and i32 %7, 32752
  %9 = call float @dx.op.legacyF16ToF32(i32 131, i32 %8)  ; LegacyF16ToF32(value)
  %10 = lshr i32 %6, 7
  %11 = and i32 %10, 32752
  %12 = call float @dx.op.legacyF16ToF32(i32 131, i32 %11)  ; LegacyF16ToF32(value)
  %13 = lshr i32 %6, 17
  %14 = and i32 %13, 32736
  %15 = call float @dx.op.legacyF16ToF32(i32 131, i32 %14)  ; LegacyF16ToF32(value)
  %16 = fcmp fast olt float %9, 0x3F69A5C380000000
  %17 = fcmp fast olt float %12, 0x3F69A5C380000000
  %18 = fcmp fast olt float %15, 0x3F69A5C380000000
  %19 = fmul fast float %9, 0x4029D70A40000000
  %20 = fmul fast float %12, 0x4029D70A40000000
  %21 = fmul fast float %15, 0x4029D70A40000000
  %22 = call float @dx.op.unary.f32(i32 23, float %9)  ; Log(value)
  %23 = call float @dx.op.unary.f32(i32 23, float %12)  ; Log(value)
  %24 = call float @dx.op.unary.f32(i32 23, float %15)  ; Log(value)
  %25 = fmul fast float %22, 0x3FDAAAAAA0000000
  %26 = fmul fast float %23, 0x3FDAAAAAA0000000
  %27 = fmul fast float %24, 0x3FDAAAAAA0000000
  %28 = call float @dx.op.unary.f32(i32 21, float %25)  ; Exp(value)
  %29 = call float @dx.op.unary.f32(i32 21, float %26)  ; Exp(value)
  %30 = call float @dx.op.unary.f32(i32 21, float %27)  ; Exp(value)
  %31 = fmul fast float %28, 0x3FF0E147A0000000
  %32 = fmul fast float %29, 0x3FF0E147A0000000
  %33 = fmul fast float %30, 0x3FF0E147A0000000
  %34 = fadd fast float %31, 0xBFAC28F5C0000000
  %35 = fadd fast float %32, 0xBFAC28F5C0000000
  %36 = fadd fast float %33, 0xBFAC28F5C0000000
  %37 = select i1 %16, float %19, float %34
  %38 = select i1 %17, float %20, float %35
  %39 = select i1 %18, float %21, float %36
  call void @dx.op.textureStore.f32(i32 67, %dx.types.Handle %1, i32 %3, i32 %4, i32 undef, float %37, float %38, float %39, float 1.000000e+00, i8 15)  ; TextureStore(srv,coord0,coord1,coord2,value0,value1,value2,value3,mask)
  ret void
}

; Function Attrs: nounwind readnone
declare i32 @dx.op.threadId.i32(i32, i32) #0

; Function Attrs: nounwind
declare void @dx.op.textureStore.f32(i32, %dx.types.Handle, i32, i32, i32, float, float, float, float, i8) #1

; Function Attrs: nounwind readonly
declare %dx.types.ResRet.i32 @dx.op.textureLoad.i32(i32, %dx.types.Handle, i32, i32, i32, i32, i32, i32, i32) #2

; Function Attrs: nounwind readnone
declare float @dx.op.legacyF16ToF32(i32, i32) #0

; Function Attrs: nounwind readnone
declare float @dx.op.unary.f32(i32, float) #0

; Function Attrs: nounwind readonly
declare %dx.types.Handle @dx.op.createHandle(i32, i8, i32, i32, i1) #2

attributes #0 = { nounwind readnone }
attributes #1 = { nounwind }
attributes #2 = { nounwind readonly }

!llvm.ident = !{!0}
!dx.version = !{!1}
!dx.valver = !{!2}
!dx.shaderModel = !{!3}
!dx.resources = !{!4}
!dx.entryPoints = !{!10}

!0 = !{!"clang version 3.7 (tags/RELEASE_370/final)"}
!1 = !{i32 1, i32 2}
!2 = !{i32 1, i32 6}
!3 = !{!"cs", i32 6, i32 2}
!4 = !{null, !5, null, null}
!5 = !{!6, !8}
!6 = !{i32 0, %"class.RWTexture2D<unsigned int>"* undef, !"", i32 0, i32 0, i32 1, i32 2, i1 false, i1 false, i1 false, !7}
!7 = !{i32 0, i32 5}
!8 = !{i32 1, %"class.RWTexture2D<vector<float, 4> >"* undef, !"", i32 0, i32 1, i32 1, i32 2, i1 false, i1 false, i1 false, !9}
!9 = !{i32 0, i32 9}
!10 = !{void ()* @main, !"main", null, !4, !11}
!11 = !{i32 4, !12}
!12 = !{i32 8, i32 8, i32 1}

#endif

const unsigned char g_pVRSScreenshot_RGB_CS[] = {
  0x44, 0x58, 0x42, 0x43, 0x5d, 0xe5, 0xd4, 0x5b, 0x10, 0xd2, 0x48, 0xa3,
  0x24, 0x34, 0x8b, 0x0e, 0x9d, 0x2a, 0x7a, 0x77, 0x01, 0x00, 0x00, 0x00,
  0x5c, 0x0e, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x50, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
  0xf0, 0x00, 0x00, 0x00, 0x3c, 0x01, 0x00, 0x00, 0x48, 0x07, 0x00, 0x00,
  0x64, 0x07, 0x00, 0x00, 0x53, 0x46, 0x49, 0x30, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x53, 0x47, 0x31,
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x4f, 0x53, 0x47, 0x31, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x50, 0x53, 0x56, 0x30, 0x78, 0x00, 0x00, 0x00,
  0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x52, 0x54, 0x53, 0x30, 0x44, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x2c, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0x53, 0x54, 0x41, 0x54, 0x04, 0x06, 0x00, 0x00,
  0x62, 0x00, 0x05, 0x00, 0x81, 0x01, 0x00, 0x00, 0x44, 0x58, 0x49, 0x4c,
  0x02, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xec, 0x05, 0x00, 0x00,
  0x42, 0x43, 0xc0, 0xde, 0x21, 0x0c, 0x00, 0x00, 0x78, 0x01, 0x00, 0x00,
  0x0b, 0x82, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00,
  0x07, 0x81, 0x23, 0x91, 0x41, 0xc8, 0x04, 0x49, 0x06, 0x10, 0x32, 0x39,
  0x92, 0x01, 0x84, 0x0c, 0x25, 0x05, 0x08, 0x19, 0x1e, 0x04, 0x8b, 0x62,
  0x80, 0x14, 0x45, 0x02, 0x42, 0x92, 0x0b, 0x42, 0xa4, 0x10, 0x32, 0x14,
  0x38, 0x08, 0x18, 0x4b, 0x0a, 0x32, 0x52, 0x88, 0x48, 0x90, 0x14, 0x20,
  0x43, 0x46, 0x88, 0xa5, 0x00, 0x19, 0x32, 0x42, 0xe4, 0x48, 0x0e, 0x90,
  0x91, 0x22, 0xc4, 0x50, 0x41, 0x51, 0x81, 0x8c, 0xe1, 0x83, 0xe5, 0x8a,
  0x04, 0x29, 0x46, 0x06, 0x51, 0x18, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x1b, 0x8c, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x07, 0x40, 0x02, 0xa8, 0x0d,
  0x84, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x03, 0x20, 0x6d, 0x30, 0x86, 0xff,
  0xff, 0xff, 0xff, 0x1f, 0x00, 0x09, 0xa8, 0x00, 0x49, 0x18, 0x00, 0x00,
  0x03, 0x00, 0x00, 0x00, 0x13, 0x82, 0x60, 0x42, 0x20, 0x4c, 0x08, 0x06,
  0x00, 0x00, 0x00, 0x00, 0x89, 0x20, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
  0x32, 0x22, 0x48, 0x09, 0x20, 0x64, 0x85, 0x04, 0x93, 0x22, 0xa4, 0x84,
  0x04, 0x93, 0x22, 0xe3, 0x84, 0xa1, 0x90, 0x14, 0x12, 0x4c, 0x8a, 0x8c,
  0x0b, 0x84, 0xa4, 0x4c, 0x10, 0x70, 0x23, 0x00, 0x25, 0x00, 0x14, 0xe6,
  0x08, 0xc0, 0xa0, 0x0c, 0x63, 0x0c, 0x22, 0x73, 0x04, 0x08, 0x99, 0x7b,
  0x86, 0xcb, 0x9f, 0xb0, 0x87, 0x90, 0xfc, 0x10, 0x68, 0x86, 0x85, 0x40,
  0xc1, 0x99, 0x01, 0x28, 0x0b, 0x18, 0x68, 0x8c, 0x91, 0x52, 0x4a, 0x86,
  0xd4, 0x4d, 0xc3, 0xe5, 0x4f, 0xd8, 0x43, 0x48, 0xfe, 0x4a, 0x48, 0x2b,
  0x31, 0xf9, 0xc8, 0x6d, 0xa3, 0x62, 0x8c, 0x31, 0x46, 0x51, 0xd8, 0x40,
  0x63, 0x8c, 0x31, 0xc6, 0xa0, 0x56, 0x86, 0x34, 0x06, 0xbd, 0x32, 0xa4,
  0x91, 0x28, 0xce, 0x11, 0x04, 0xc5, 0x40, 0xc3, 0x8c, 0x31, 0x89, 0x0e,
  0x04, 0xcc, 0xf4, 0x8d, 0x03, 0x3b, 0x84, 0xc3, 0x3c, 0xcc, 0x83, 0x1b,
  0xc8, 0xc2, 0x2d, 0xd0, 0x42, 0x39, 0xe0, 0x03, 0x3d, 0xd4, 0x83, 0x3c,
  0x94, 0x83, 0x1c, 0x90, 0x02, 0x1f, 0xd4, 0x83, 0x3b, 0xcc, 0x43, 0x3a,
  0x9c, 0x83, 0x3b, 0x94, 0x03, 0x39, 0x80, 0x41, 0x3a, 0xb8, 0x03, 0x3d,
  0xf8, 0x01, 0x0a, 0x06, 0xdd, 0x61, 0x04, 0x22, 0x99, 0x89, 0x0c, 0xc6,
  0x81, 0x1d, 0xc2, 0x61, 0x1e, 0xe6, 0xc1, 0x0d, 0x64, 0xe1, 0x16, 0x68,
  0xa1, 0x1c, 0xf0, 0x81, 0x1e, 0xea, 0x41, 0x1e, 0xca, 0x41, 0x0e, 0x48,
  0x81, 0x0f, 0xec, 0xa1, 0x1c, 0xc6, 0x81, 0x1e, 0xde, 0x41, 0x1e, 0xf8,
  0xc0, 0x1c, 0xd8, 0xe1, 0x1d, 0xc2, 0x81, 0x1e, 0xd8, 0x00, 0x0c, 0xe8,
  0xc0, 0x0f, 0xc0, 0xc0, 0x0f, 0x50, 0x90, 0x49, 0x03, 0x00, 0x00, 0x00,
  0x13, 0x14, 0x72, 0xc0, 0x87, 0x74, 0x60, 0x87, 0x36, 0x68, 0x87, 0x79,
  0x68, 0x03, 0x72, 0xc0, 0x87, 0x0d, 0xaf, 0x50, 0x0e, 0x6d, 0xd0, 0x0e,
  0x7a, 0x50, 0x0e, 0x6d, 0x00, 0x0f, 0x7a, 0x30, 0x07, 0x72, 0xa0, 0x07,
  0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e, 0x71, 0xa0, 0x07, 0x73, 0x20, 0x07,
  0x6d, 0x90, 0x0e, 0x78, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e,
  0x71, 0x60, 0x07, 0x7a, 0x30, 0x07, 0x72, 0xd0, 0x06, 0xe9, 0x30, 0x07,
  0x72, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e, 0x76, 0x40, 0x07,
  0x7a, 0x60, 0x07, 0x74, 0xd0, 0x06, 0xe6, 0x10, 0x07, 0x76, 0xa0, 0x07,
  0x73, 0x20, 0x07, 0x6d, 0x60, 0x0e, 0x73, 0x20, 0x07, 0x7a, 0x30, 0x07,
  0x72, 0xd0, 0x06, 0xe6, 0x60, 0x07, 0x74, 0xa0, 0x07, 0x76, 0x40, 0x07,
  0x6d, 0xe0, 0x0e, 0x78, 0xa0, 0x07, 0x71, 0x60, 0x07, 0x7a, 0x30, 0x07,
  0x72, 0xa0, 0x07, 0x76, 0x40, 0x07, 0x43, 0x9e, 0x00, 0x08, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86, 0x3c, 0x04, 0x10, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x79, 0x14, 0x20,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xf2, 0x34,
  0x40, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0xe4,
  0x79, 0x80, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60,
  0xc8, 0x13, 0x01, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xc0, 0x90, 0x87, 0x02, 0x02, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x2c, 0x10, 0x0b, 0x00, 0x00, 0x00, 0x32, 0x1e, 0x98, 0x14,
  0x19, 0x11, 0x4c, 0x90, 0x8c, 0x09, 0x26, 0x47, 0xc6, 0x04, 0x43, 0x1a,
  0x25, 0x50, 0x04, 0x23, 0x00, 0xc5, 0x50, 0x0a, 0x25, 0x51, 0x08, 0x05,
  0x51, 0x2a, 0x25, 0x45, 0x73, 0x04, 0x80, 0xf0, 0x0c, 0x00, 0xed, 0x19,
  0x00, 0x00, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00, 0x61, 0x00, 0x00, 0x00,
  0x1a, 0x03, 0x4c, 0x90, 0x46, 0x02, 0x13, 0x44, 0x35, 0x18, 0x63, 0x0b,
  0x73, 0x3b, 0x03, 0xb1, 0x2b, 0x93, 0x9b, 0x4b, 0x7b, 0x73, 0x03, 0x99,
  0x71, 0xb9, 0x01, 0x41, 0xa1, 0x0b, 0x3b, 0x9b, 0x7b, 0x91, 0x2a, 0x62,
  0x2a, 0x0a, 0x9a, 0x2a, 0xfa, 0x9a, 0xb9, 0x81, 0x79, 0x31, 0x4b, 0x73,
  0x0b, 0x63, 0x4b, 0xd9, 0x10, 0x04, 0x13, 0x84, 0xe1, 0x98, 0x20, 0x0c,
  0xc8, 0x06, 0x61, 0x20, 0x26, 0x08, 0x43, 0xb2, 0x41, 0x30, 0x0c, 0x0a,
  0x63, 0x73, 0x13, 0x84, 0x41, 0xd9, 0x30, 0x20, 0x09, 0x31, 0x41, 0xc0,
  0x24, 0x22, 0x53, 0x72, 0x63, 0x49, 0x6d, 0x61, 0x67, 0x65, 0x13, 0x84,
  0x29, 0x9a, 0x20, 0x0c, 0xcb, 0x06, 0xc1, 0x70, 0x36, 0x2c, 0xc6, 0xc2,
  0x18, 0xc6, 0x40, 0x34, 0x4d, 0xf3, 0x4c, 0x10, 0xb6, 0x89, 0x08, 0xd1,
  0x1c, 0x5d, 0x52, 0x5b, 0xd8, 0x59, 0xd9, 0x04, 0x61, 0x60, 0x36, 0x08,
  0xc6, 0xb4, 0x61, 0x19, 0x22, 0xc9, 0x18, 0x06, 0xa2, 0x69, 0x1a, 0x6a,
  0x83, 0x00, 0x55, 0x1b, 0x08, 0xc0, 0x02, 0x80, 0x09, 0x82, 0x00, 0x6c,
  0x00, 0x36, 0x0c, 0x46, 0x96, 0x6d, 0x08, 0xb4, 0x0d, 0xc3, 0x80, 0x6d,
  0x24, 0xda, 0xc2, 0xd2, 0xdc, 0x26, 0x08, 0x43, 0x33, 0x41, 0x18, 0x9c,
  0x0d, 0xc3, 0xf7, 0x0d, 0x1b, 0x04, 0x0f, 0x0c, 0x36, 0x14, 0x58, 0x07,
  0x5c, 0x61, 0x40, 0xc3, 0x8c, 0xed, 0x2d, 0x8c, 0x6e, 0x6e, 0x82, 0x30,
  0x3c, 0x2c, 0xd2, 0xdc, 0xe6, 0xe8, 0xe6, 0x26, 0x08, 0x03, 0x44, 0x84,
  0xae, 0x0c, 0xef, 0x8b, 0xed, 0x2d, 0x8c, 0x8c, 0x09, 0x5d, 0x19, 0xde,
  0xd7, 0x1c, 0xdd, 0x9b, 0x5c, 0x19, 0x8b, 0xba, 0x34, 0x37, 0xba, 0xb9,
  0x0d, 0xca, 0x18, 0x90, 0x41, 0x19, 0x98, 0xc1, 0x19, 0x0c, 0x68, 0x30,
  0xa4, 0x41, 0x52, 0x85, 0x8d, 0xcd, 0xae, 0xcd, 0x25, 0x8d, 0xac, 0xcc,
  0x8d, 0x6e, 0x4a, 0x10, 0x54, 0x21, 0xc3, 0x73, 0xb1, 0x2b, 0x93, 0x9b,
  0x4b, 0x7b, 0x73, 0x9b, 0x12, 0x10, 0x4d, 0xc8, 0xf0, 0x5c, 0xec, 0xc2,
  0xd8, 0xec, 0xca, 0xe4, 0xa6, 0x04, 0x46, 0x1d, 0x32, 0x3c, 0x97, 0x39,
  0xb4, 0x30, 0xb2, 0x32, 0xb9, 0xa6, 0x37, 0xb2, 0x32, 0xb6, 0x29, 0x41,
  0x52, 0x86, 0x0c, 0xcf, 0x45, 0xae, 0x6c, 0xee, 0xad, 0x4e, 0x6e, 0xac,
  0x6c, 0x6e, 0x4a, 0x60, 0x55, 0x22, 0xc3, 0x73, 0xa1, 0xcb, 0x83, 0x2b,
  0x0b, 0x72, 0x73, 0x7b, 0xa3, 0x0b, 0xa3, 0x4b, 0x7b, 0x73, 0x9b, 0x9b,
  0x12, 0x6c, 0x75, 0xc8, 0xf0, 0x5c, 0xca, 0xdc, 0xe8, 0xe4, 0xf2, 0xa0,
  0xde, 0xd2, 0xdc, 0xe8, 0xe6, 0xa6, 0x04, 0x61, 0xd0, 0x85, 0x0c, 0xcf,
  0x65, 0xec, 0xad, 0xce, 0x8d, 0xae, 0x4c, 0x6e, 0x6e, 0x4a, 0x90, 0x06,
  0x00, 0x00, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00,
  0x33, 0x08, 0x80, 0x1c, 0xc4, 0xe1, 0x1c, 0x66, 0x14, 0x01, 0x3d, 0x88,
  0x43, 0x38, 0x84, 0xc3, 0x8c, 0x42, 0x80, 0x07, 0x79, 0x78, 0x07, 0x73,
  0x98, 0x71, 0x0c, 0xe6, 0x00, 0x0f, 0xed, 0x10, 0x0e, 0xf4, 0x80, 0x0e,
  0x33, 0x0c, 0x42, 0x1e, 0xc2, 0xc1, 0x1d, 0xce, 0xa1, 0x1c, 0x66, 0x30,
  0x05, 0x3d, 0x88, 0x43, 0x38, 0x84, 0x83, 0x1b, 0xcc, 0x03, 0x3d, 0xc8,
  0x43, 0x3d, 0x8c, 0x03, 0x3d, 0xcc, 0x78, 0x8c, 0x74, 0x70, 0x07, 0x7b,
  0x08, 0x07, 0x79, 0x48, 0x87, 0x70, 0x70, 0x07, 0x7a, 0x70, 0x03, 0x76,
  0x78, 0x87, 0x70, 0x20, 0x87, 0x19, 0xcc, 0x11, 0x0e, 0xec, 0x90, 0x0e,
  0xe1, 0x30, 0x0f, 0x6e, 0x30, 0x0f, 0xe3, 0xf0, 0x0e, 0xf0, 0x50, 0x0e,
  0x33, 0x10, 0xc4, 0x1d, 0xde, 0x21, 0x1c, 0xd8, 0x21, 0x1d, 0xc2, 0x61,
  0x1e, 0x66, 0x30, 0x89, 0x3b, 0xbc, 0x83, 0x3b, 0xd0, 0x43, 0x39, 0xb4,
  0x03, 0x3c, 0xbc, 0x83, 0x3c, 0x84, 0x03, 0x3b, 0xcc, 0xf0, 0x14, 0x76,
  0x60, 0x07, 0x7b, 0x68, 0x07, 0x37, 0x68, 0x87, 0x72, 0x68, 0x07, 0x37,
  0x80, 0x87, 0x70, 0x90, 0x87, 0x70, 0x60, 0x07, 0x76, 0x28, 0x07, 0x76,
  0xf8, 0x05, 0x76, 0x78, 0x87, 0x77, 0x80, 0x87, 0x5f, 0x08, 0x87, 0x71,
  0x18, 0x87, 0x72, 0x98, 0x87, 0x79, 0x98, 0x81, 0x2c, 0xee, 0xf0, 0x0e,
  0xee, 0xe0, 0x0e, 0xf5, 0xc0, 0x0e, 0xec, 0x30, 0x03, 0x62, 0xc8, 0xa1,
  0x1c, 0xe4, 0xa1, 0x1c, 0xcc, 0xa1, 0x1c, 0xe4, 0xa1, 0x1c, 0xdc, 0x61,
  0x1c, 0xca, 0x21, 0x1c, 0xc4, 0x81, 0x1d, 0xca, 0x61, 0x06, 0xd6, 0x90,
  0x43, 0x39, 0xc8, 0x43, 0x39, 0x98, 0x43, 0x39, 0xc8, 0x43, 0x39, 0xb8,
  0xc3, 0x38, 0x94, 0x43, 0x38, 0x88, 0x03, 0x3b, 0x94, 0xc3, 0x2f, 0xbc,
  0x83, 0x3c, 0xfc, 0x82, 0x3b, 0xd4, 0x03, 0x3b, 0xb0, 0xc3, 0x0c, 0xc4,
  0x21, 0x07, 0x7c, 0x70, 0x03, 0x7a, 0x28, 0x87, 0x76, 0x80, 0x87, 0x19,
  0xd1, 0x43, 0x0e, 0xf8, 0xe0, 0x06, 0xe4, 0x20, 0x0e, 0xe7, 0xe0, 0x06,
  0xf6, 0x10, 0x0e, 0xf2, 0xc0, 0x0e, 0xe1, 0x90, 0x0f, 0xef, 0x50, 0x0f,
  0xf4, 0x00, 0x00, 0x00, 0x71, 0x20, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00,
  0x66, 0x20, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x10, 0x11, 0xc0, 0x44, 0x84,
  0x40, 0x33, 0x2c, 0x84, 0x11, 0x50, 0xc3, 0xe5, 0x3b, 0x8f, 0xbf, 0x10,
  0x06, 0x20, 0x60, 0x5f, 0xad, 0xb7, 0xce, 0x77, 0xdb, 0x36, 0x50, 0x0d,
  0x97, 0xef, 0x3c, 0xfe, 0x44, 0x5c, 0x13, 0x15, 0x11, 0xa5, 0x03, 0x0c,
  0x3e, 0x72, 0xdb, 0x26, 0x60, 0x0d, 0x97, 0xef, 0x3c, 0xfe, 0x44, 0x5c,
  0x13, 0x15, 0x11, 0xec, 0xe4, 0x44, 0x84, 0x5f, 0xdc, 0xb6, 0x05, 0x48,
  0xc3, 0xe5, 0x3b, 0x8f, 0x3f, 0x1d, 0x11, 0x01, 0x0c, 0xe2, 0xe0, 0x23,
  0xb7, 0x6d, 0x05, 0xcf, 0x70, 0xf9, 0xce, 0xe3, 0x53, 0x0d, 0x10, 0x61,
  0x7e, 0x71, 0xdb, 0x06, 0x40, 0x30, 0x00, 0xd2, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x48, 0x41, 0x53, 0x48, 0x14, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x57, 0xc4, 0xb3, 0xf2, 0xf5, 0x9a, 0x0f, 0x36,
  0x0e, 0x41, 0xf5, 0x4b, 0x24, 0xee, 0xb4, 0x79, 0x44, 0x58, 0x49, 0x4c,
  0xf0, 0x06, 0x00, 0x00, 0x62, 0x00, 0x05, 0x00, 0xbc, 0x01, 0x00, 0x00,
  0x44, 0x58, 0x49, 0x4c, 0x02, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0xd8, 0x06, 0x00, 0x00, 0x42, 0x43, 0xc0, 0xde, 0x21, 0x0c, 0x00, 0x00,
  0xb3, 0x01, 0x00, 0x00, 0x0b, 0x82, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x13, 0x00, 0x00, 0x00, 0x07, 0x81, 0x23, 0x91, 0x41, 0xc8, 0x04, 0x49,
  0x06, 0x10, 0x32, 0x39, 0x92, 0x01, 0x84, 0x0c, 0x25, 0x05, 0x08, 0x19,
  0x1e, 0x04, 0x8b, 0x62, 0x80, 0x14, 0x45, 0x02, 0x42, 0x92, 0x0b, 0x42,
  0xa4, 0x10, 0x32, 0x14, 0x38, 0x08, 0x18, 0x4b, 0x0a, 0x32, 0x52, 0x88,
  0x48, 0x90, 0x14, 0x20, 0x43, 0x46, 0x88, 0xa5, 0x00, 0x19, 0x32, 0x42,
  0xe4, 0x48, 0x0e, 0x90, 0x91, 0x22, 0xc4, 0x50, 0x41, 0x51, 0x81, 0x8c,
  0xe1, 0x83, 0xe5, 0x8a, 0x04, 0x29, 0x46, 0x06, 0x51, 0x18, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x1b, 0x8c, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x07,
  0x40, 0x02, 0xa8, 0x0d, 0x84, 0xf0, 0xff, 0xff, 0xff, 0xff, 0x03, 0x20,
  0x6d, 0x30, 0x86, 0xff, 0xff, 0xff, 0xff, 0x1f, 0x00, 0x09, 0xa8, 0x00,
  0x49, 0x18, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x13, 0x82, 0x60, 0x42,
  0x20, 0x4c, 0x08, 0x06, 0x00, 0x00, 0x00, 0x00, 0x89, 0x20, 0x00, 0x00,
  0x3c, 0x00, 0x00, 0x00, 0x32, 0x22, 0x48, 0x09, 0x20, 0x64, 0x85, 0x04,
  0x93, 0x22, 0xa4, 0x84, 0x04, 0x93, 0x22, 0xe3, 0x84, 0xa1, 0x90, 0x14,
  0x12, 0x4c, 0x8a, 0x8c, 0x0b, 0x84, 0xa4, 0x4c, 0x10, 0x70, 0x23, 0x00,
  0x25, 0x00, 0x14, 0xe6, 0x08, 0xc0, 0xa0, 0x0c, 0x63, 0x0c, 0x22, 0x73,
  0x04, 0x08, 0x99, 0x7b, 0x86, 0xcb, 0x9f, 0xb0, 0x87, 0x90, 0xfc, 0x10,
  0x68, 0x86, 0x85, 0x40, 0xc1, 0x99, 0x01, 0x28, 0x0b, 0x18, 0x68, 0x8c,
  0x91, 0x52, 0x4a, 0x86, 0xd4, 0x4d, 0xc3, 0xe5, 0x4f, 0xd8, 0x43, 0x48,
  0xfe, 0x4a, 0x48, 0x2b, 0x31, 0xf9, 0xc8, 0x6d, 0xa3, 0x62, 0x8c, 0x31,
  0x46, 0x51, 0xd8, 0x40, 0x63, 0x8c, 0x31, 0xc6, 0xa0, 0x56, 0x86, 0x34,
  0x06, 0xbd, 0x32, 0xa4, 0x91, 0x28, 0xce, 0x11, 0x04, 0xc5, 0x40, 0xc3,
  0x8c, 0x31, 0x89, 0x0e, 0x04, 0xcc, 0xf4, 0x8d, 0x03, 0x3b, 0x84, 0xc3,
  0x3c, 0xcc, 0x83, 0x1b, 0xc8, 0xc2, 0x2d, 0xd0, 0x42, 0x39, 0xe0, 0x03,
  0x3d, 0xd4, 0x83, 0x3c, 0x94, 0x83, 0x1c, 0x90, 0x02, 0x1f, 0xd4, 0x83,
  0x3b, 0xcc, 0x43, 0x3a, 0x9c, 0x83, 0x3b, 0x94, 0x03, 0x39, 0x80, 0x41,
  0x3a, 0xb8, 0x03, 0x3d, 0xf8, 0x01, 0x0a, 0x06, 0xdd, 0x61, 0x04, 0x22,
  0x99, 0x89, 0x0c, 0xc6, 0x81, 0x1d, 0xc2, 0x61, 0x1e, 0xe6, 0xc1, 0x0d,
  0x64, 0xe1, 0x16, 0x68, 0xa1, 0x1c, 0xf0, 0x81, 0x1e, 0xea, 0x41, 0x1e,
  0xca, 0x41, 0x0e, 0x48, 0x81, 0x0f, 0xec, 0xa1, 0x1c, 0xc6, 0x81, 0x1e,
  0xde, 0x41, 0x1e, 0xf8, 0xc0, 0x1c, 0xd8, 0xe1, 0x1d, 0xc2, 0x81, 0x1e,
  0xd8, 0x00, 0x0c, 0xe8, 0xc0, 0x0f, 0xc0, 0xc0, 0x0f, 0x50, 0x90, 0x49,
  0x03, 0x00, 0x00, 0x00, 0x13, 0x14, 0x72, 0xc0, 0x87, 0x74, 0x60, 0x87,
  0x36, 0x68, 0x87, 0x79, 0x68, 0x03, 0x72, 0xc0, 0x87, 0x0d, 0xaf, 0x50,
  0x0e, 0x6d, 0xd0, 0x0e, 0x7a, 0x50, 0x0e, 0x6d, 0x00, 0x0f, 0x7a, 0x30,
  0x07, 0x72, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e, 0x71, 0xa0,
  0x07, 0x73, 0x20, 0x07, 0x6d, 0x90, 0x0e, 0x78, 0xa0, 0x07, 0x73, 0x20,
  0x07, 0x6d, 0x90, 0x0e, 0x71, 0x60, 0x07, 0x7a, 0x30, 0x07, 0x72, 0xd0,
  0x06, 0xe9, 0x30, 0x07, 0x72, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d, 0x90,
  0x0e, 0x76, 0x40, 0x07, 0x7a, 0x60, 0x07, 0x74, 0xd0, 0x06, 0xe6, 0x10,
  0x07, 0x76, 0xa0, 0x07, 0x73, 0x20, 0x07, 0x6d, 0x60, 0x0e, 0x73, 0x20,
  0x07, 0x7a, 0x30, 0x07, 0x72, 0xd0, 0x06, 0xe6, 0x60, 0x07, 0x74, 0xa0,
  0x07, 0x76, 0x40, 0x07, 0x6d, 0xe0, 0x0e, 0x78, 0xa0, 0x07, 0x71, 0x60,
  0x07, 0x7a, 0x30, 0x07, 0x72, 0xa0, 0x07, 0x76, 0x40, 0x07, 0x43, 0x9e,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86,
  0x3c, 0x04, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0c, 0x79, 0x14, 0x20, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x18, 0xf2, 0x34, 0x40, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x30, 0xe4, 0x79, 0x80, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x60, 0xc8, 0x13, 0x01, 0x01, 0x10, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xc0, 0x90, 0x87, 0x02, 0x02, 0x60, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x2c, 0x10, 0x0a, 0x00, 0x00, 0x00,
  0x32, 0x1e, 0x98, 0x14, 0x19, 0x11, 0x4c, 0x90, 0x8c, 0x09, 0x26, 0x47,
  0xc6, 0x04, 0x43, 0x1a, 0x25, 0x50, 0x04, 0xc5, 0x30, 0x02, 0x50, 0x0a,
  0x25, 0x51, 0x08, 0x05, 0x41, 0x73, 0x04, 0x80, 0xf0, 0x0c, 0x00, 0xed,
  0x19, 0x00, 0x00, 0x00, 0x79, 0x18, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x1a, 0x03, 0x4c, 0x90, 0x46, 0x02, 0x13, 0x44, 0x35, 0x18, 0x63, 0x0b,
  0x73, 0x3b, 0x03, 0xb1, 0x2b, 0x93, 0x9b, 0x4b, 0x7b, 0x73, 0x03, 0x99,
  0x71, 0xb9, 0x01, 0x41, 0xa1, 0x0b, 0x3b, 0x9b, 0x7b, 0x91, 0x2a, 0x62,
  0x2a, 0x0a, 0x9a, 0x2a, 0xfa, 0x9a, 0xb9, 0x81, 0x79, 0x31, 0x4b, 0x73,
  0x0b, 0x63, 0x4b, 0xd9, 0x10, 0x04, 0x13, 0x84, 0xe1, 0x98, 0x20, 0x0c,
  0xc8, 0x06, 0x61, 0x20, 0x26, 0x08, 0x43, 0xb2, 0x41, 0x18, 0x0c, 0x0a,
  0x63, 0x73, 0x1b, 0x06, 0xc4, 0x20, 0x26, 0x08, 0x83, 0x32, 0x41, 0xc0,
  0x20, 0x02, 0x13, 0x84, 0xe9, 0x99, 0x20, 0x0c, 0xcb, 0x06, 0x41, 0x71,
  0x36, 0x2c, 0xca, 0xc2, 0x28, 0xca, 0x40, 0x34, 0x4d, 0xf3, 0x4c, 0x10,
  0xb6, 0x68, 0x82, 0x30, 0x30, 0x1b, 0x04, 0x45, 0xda, 0xb0, 0x0c, 0x11,
  0xa3, 0x0c, 0x03, 0xd1, 0x34, 0xcd, 0xb4, 0x41, 0x80, 0xa8, 0x0d, 0x04,
  0x50, 0x01, 0xc0, 0x04, 0x41, 0x00, 0x48, 0xb4, 0x85, 0xa5, 0xb9, 0x4d,
  0x10, 0x86, 0x66, 0x82, 0x30, 0x38, 0x1b, 0x06, 0x4d, 0x1b, 0x36, 0x08,
  0xd9, 0xb6, 0xa1, 0xb8, 0x30, 0xc0, 0xe2, 0xaa, 0xb0, 0xb1, 0xd9, 0xb5,
  0xb9, 0xa4, 0x91, 0x95, 0xb9, 0xd1, 0x4d, 0x09, 0x82, 0x2a, 0x64, 0x78,
  0x2e, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x53, 0x02, 0xa2, 0x09,
  0x19, 0x9e, 0x8b, 0x5d, 0x18, 0x9b, 0x5d, 0x99, 0xdc, 0x94, 0xc0, 0xa8,
  0x43, 0x86, 0xe7, 0x32, 0x87, 0x16, 0x46, 0x56, 0x26, 0xd7, 0xf4, 0x46,
  0x56, 0xc6, 0x36, 0x25, 0x40, 0xca, 0x90, 0xe1, 0xb9, 0xc8, 0x95, 0xcd,
  0xbd, 0xd5, 0xc9, 0x8d, 0x95, 0xcd, 0x4d, 0x09, 0xaa, 0x3a, 0x64, 0x78,
  0x2e, 0x65, 0x6e, 0x74, 0x72, 0x79, 0x50, 0x6f, 0x69, 0x6e, 0x74, 0x73,
  0x53, 0x02, 0x0e, 0x00, 0x79, 0x18, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00,
  0x33, 0x08, 0x80, 0x1c, 0xc4, 0xe1, 0x1c, 0x66, 0x14, 0x01, 0x3d, 0x88,
  0x43, 0x38, 0x84, 0xc3, 0x8c, 0x42, 0x80, 0x07, 0x79, 0x78, 0x07, 0x73,
  0x98, 0x71, 0x0c, 0xe6, 0x00, 0x0f, 0xed, 0x10, 0x0e, 0xf4, 0x80, 0x0e,
  0x33, 0x0c, 0x42, 0x1e, 0xc2, 0xc1, 0x1d, 0xce, 0xa1, 0x1c, 0x66, 0x30,
  0x05, 0x3d, 0x88, 0x43, 0x38, 0x84, 0x83, 0x1b, 0xcc, 0x03, 0x3d, 0xc8,
  0x43, 0x3d, 0x8c, 0x03, 0x3d, 0xcc, 0x78, 0x8c, 0x74, 0x70, 0x07, 0x7b,
  0x08, 0x07, 0x79, 0x48, 0x87, 0x70, 0x70, 0x07, 0x7a, 0x70, 0x03, 0x76,
  0x78, 0x87, 0x70, 0x20, 0x87, 0x19, 0xcc, 0x11, 0x0e, 0xec, 0x90, 0x0e,
  0xe1, 0x30, 0x0f, 0x6e, 0x30, 0x0f, 0xe3, 0xf0, 0x0e, 0xf0, 0x50, 0x0e,
  0x33, 0x10, 0xc4, 0x1d, 0xde, 0x21, 0x1c, 0xd8, 0x21, 0x1d, 0xc2, 0x61,
  0x1e, 0x66, 0x30, 0x89, 0x3b, 0xbc, 0x83, 0x3b, 0xd0, 0x43, 0x39, 0xb4,
  0x03, 0x3c, 0xbc, 0x83, 0x3c, 0x84, 0x03, 0x3b, 0xcc, 0xf0, 0x14, 0x76,
  0x60, 0x07, 0x7b, 0x68, 0x07, 0x37, 0x68, 0x87, 0x72, 0x68, 0x07, 0x37,
  0x80, 0x87, 0x70, 0x90, 0x87, 0x70, 0x60, 0x07, 0x76, 0x28, 0x07, 0x76,
  0xf8, 0x05, 0x76, 0x78, 0x87, 0x77, 0x80, 0x87, 0x5f, 0x08, 0x87, 0x71,
  0x18, 0x87, 0x72, 0x98, 0x87, 0x79, 0x98, 0x81, 0x2c, 0xee, 0xf0, 0x0e,
  0xee, 0xe0, 0x0e, 0xf5, 0xc0, 0x0e, 0xec, 0x30, 0x03, 0x62, 0xc8, 0xa1,
  0x1c, 0xe4, 0xa1, 0x1c, 0xcc, 0xa1, 0x1c, 0xe4, 0xa1, 0x1c, 0xdc, 0x61,
  0x1c, 0xca, 0x21, 0x1c, 0xc4, 0x81, 0x1d, 0xca, 0x61, 0x06, 0xd6, 0x90,
  0x43, 0x39, 0xc8, 0x43, 0x39, 0x98, 0x43, 0x39, 0xc8, 0x43, 0x39, 0xb8,
  0xc3, 0x38, 0x94, 0x43, 0x38, 0x88, 0x03, 0x3b, 0x94, 0xc3, 0x2f, 0xbc,
  0x83, 0x3c, 0xfc, 0x82, 0x3b, 0xd4, 0x03, 0x3b, 0xb0, 0xc3, 0x0c, 0xc4,
  0x21, 0x07, 0x7c, 0x70, 0x03, 0x7a, 0x28, 0x87, 0x76, 0x80, 0x87, 0x19,
  0xd1, 0x43, 0x0e, 0xf8, 0xe0, 0x06, 0xe4, 0x20, 0x0e, 0xe7, 0xe0, 0x06,
  0xf6, 0x10, 0x0e, 0xf2, 0xc0, 0x0e, 0xe1, 0x90, 0x0f, 0xef, 0x50, 0x0f,
  0xf4, 0x00, 0x00, 0x00, 0x71, 0x20, 0x00, 0x00, 0x1b, 0x00, 0x00, 0x00,
  0x66, 0x20, 0x0d, 0x97, 0xef, 0x3c, 0xbe, 0x10, 0x11, 0xc0, 0x44, 0x84,
  0x40, 0x33, 0x2c, 0x84, 0x11, 0x50, 0xc3, 0xe5, 0x3b, 0x8f, 0xbf, 0x10,
  0x06, 0x20, 0x60, 0x5f, 0xad, 0xb7, 0xce, 0x77, 0xdb, 0x36, 0x50, 0x0d,
  0x97, 0xef, 0x3c, 0xfe, 0x44, 0x5c, 0x13, 0x15, 0x11, 0xa5, 0x03, 0x0c,
  0x3e, 0x72, 0xdb, 0x26, 0x60, 0x0d, 0x97, 0xef, 0x3c, 0xfe, 0x44, 0x5c,
  0x13, 0x15, 0x11, 0xec, 0xe4, 0x44, 0x84, 0x5f, 0xdc, 0xb6, 0x05, 0x48,
  0xc3, 0xe5, 0x3b, 0x8f, 0x3f, 0x1d, 0x11, 0x01, 0x0c, 0xe2, 0xe0, 0x23,
  0xb7, 0x6d, 0x05, 0xcf, 0x70, 0xf9, 0xce, 0xe3, 0x53, 0x0d, 0x10, 0x61,
  0x7e, 0x71, 0xdb, 0x06, 0x40, 0x30, 0x00, 0xd2, 0x00, 0x00, 0x00, 0x00,
  0x61, 0x20, 0x00, 0x00, 0x5b, 0x00, 0x00, 0x00, 0x13, 0x04, 0x41, 0x2c,
  0x10, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x34, 0x66, 0x00, 0xca,
  0x50, 0xa0, 0x5c, 0x4a, 0xa5, 0xe4, 0x4a, 0x37, 0xa0, 0xc0, 0xff, 0x07,
  0x8a, 0x30, 0xa0, 0x1c, 0x4a, 0xa4, 0x80, 0xff, 0x07, 0xca, 0x30, 0x80,
  0x4c, 0x09, 0x94, 0x07, 0xa5, 0x31, 0x02, 0x1e, 0x5e, 0x75, 0xba, 0x1b,
  0x23, 0x90, 0x45, 0xb7, 0xa7, 0xc1, 0x60, 0x8c, 0xa0, 0x56, 0x6b, 0xb5,
  0xfd, 0xc6, 0x08, 0xfa, 0x58, 0x74, 0xf1, 0x6f, 0x8c, 0xc0, 0xed, 0x63,
  0xd1, 0xf6, 0x85, 0x31, 0x02, 0x10, 0x04, 0x41, 0xfc, 0x03, 0x00, 0x00,
  0x23, 0x06, 0x09, 0x00, 0x82, 0x60, 0x40, 0x81, 0x01, 0x84, 0x7c, 0xdf,
  0x35, 0x62, 0x90, 0x00, 0x20, 0x08, 0x06, 0x54, 0x18, 0x44, 0x49, 0xd7,
  0x61, 0x23, 0x06, 0x06, 0x00, 0x82, 0x60, 0x40, 0x9c, 0x41, 0xe4, 0x8d,
  0x18, 0x18, 0x00, 0x08, 0x82, 0x01, 0x81, 0x06, 0x92, 0x18, 0x8c, 0x18,
  0x34, 0x00, 0x08, 0x82, 0x41, 0x73, 0x06, 0xd1, 0x80, 0x09, 0x01, 0x86,
  0x61, 0xd8, 0x68, 0x42, 0x00, 0x54, 0xf0, 0x5d, 0x05, 0x95, 0x8e, 0x18,
  0x18, 0x00, 0x08, 0x82, 0xc1, 0xa3, 0x06, 0x5b, 0x50, 0x44, 0x85, 0x15,
  0x60, 0x3a, 0x62, 0x60, 0x00, 0x20, 0x08, 0x06, 0x4f, 0x1b, 0x78, 0x41,
  0x1d, 0x17, 0x56, 0x70, 0xe9, 0x88, 0x81, 0x01, 0x80, 0x20, 0x18, 0x3c,
  0x70, 0x10, 0x06, 0xc1, 0x70, 0xc4, 0x51, 0x11, 0xdf, 0x70, 0x44, 0x61,
  0x11, 0xdf, 0x70, 0xc4, 0x70, 0x11, 0x9f, 0x29, 0x97, 0x7c, 0x0c, 0xc1,
  0xe4, 0x63, 0x46, 0x26, 0x9f, 0x11, 0x03, 0x03, 0x00, 0x41, 0x30, 0x88,
  0xec, 0xe0, 0x0c, 0x9a, 0x11, 0x03, 0x03, 0x00, 0x41, 0x30, 0x88, 0xee,
  0x00, 0x0d, 0x96, 0x11, 0x03, 0x03, 0x00, 0x41, 0x30, 0x88, 0xf0, 0x20,
  0x0d, 0x12, 0x1b, 0x38, 0xf9, 0xd8, 0xd0, 0xc9, 0xc7, 0x06, 0x4f, 0x3e,
  0x23, 0x06, 0x06, 0x00, 0x82, 0x60, 0x10, 0xf1, 0x01, 0x1b, 0x0c, 0x23,
  0x06, 0x06, 0x00, 0x82, 0x60, 0x10, 0xf5, 0x41, 0x1b, 0x0c, 0x23, 0x06,
  0x06, 0x00, 0x82, 0x60, 0x10, 0xf9, 0x81, 0x1b, 0x0c, 0x36, 0x84, 0x81,
  0x7c, 0x6c, 0x10, 0x03, 0xf9, 0xd8, 0x30, 0x06, 0xf2, 0xb1, 0x61, 0x0c,
  0xe0, 0x63, 0x03, 0x19, 0xc0, 0xc7, 0x86, 0x32, 0x80, 0xcf, 0x74, 0x83,
  0x34, 0x54, 0xd3, 0x0d, 0xd2, 0x50, 0x4d, 0x37, 0x48, 0x43, 0x35, 0x62,
  0xe0, 0x00, 0x20, 0x08, 0x06, 0xca, 0x2a, 0xc0, 0xc1, 0x19, 0x94, 0x01,
  0x19, 0xec, 0xc1, 0x20, 0x04, 0x68, 0xe0, 0x06, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};
